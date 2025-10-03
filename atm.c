#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "atm.h"

/* ================= Configuration ================= */
#define MAX_USERS           5
#define MAX_NAME            20
#define PIN_LEN             4
#define MAX_LOGIN_TRIES     2

#define MAX_TRANSACTIONS    10
#define TRANSACTION_LENGTH  100

/* ================== User Database ================= */
static char usernames[MAX_USERS][MAX_NAME] = { "mike", "judith", "ade", "ngozi", "omar" };
static char pins[MAX_USERS][PIN_LEN + 1]   = { "1234", "4321", "0007", "2468", "1111" };
static double balances[MAX_USERS]          = { 100000.00, 1250.50, 90000.00, 3000.00, 500.00 };

/* Per-user circular transaction logs */
static char transactions[MAX_USERS][MAX_TRANSACTIONS][TRANSACTION_LENGTH];
static int  tx_count[MAX_USERS] = {0};

/* ================== Internal Helpers ================== */
static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int get_user_choice(void) {
    int choice;
    while (scanf("%d", &choice) != 1) {
        printf("❌ Invalid input! Please enter a number: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    return choice;
}

static int find_user(const char *name) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (strcmp(name, usernames[i]) == 0) return i;
    }
    return -1;
}

static int is_pin_4digits(const char *p) {
    if ((int)strlen(p) != PIN_LEN) return 0;
    for (int i = 0; i < PIN_LEN; i++) {
        if (!isdigit((unsigned char)p[i])) return 0;
    }
    return 1;
}

static void get_current_time(char* time_str) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(time_str, 20, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
}

static void log_transaction(int user_idx, const char* type, double amount) {
    char time_str[20];
    char entry[TRANSACTION_LENGTH];

    get_current_time(time_str);

    if (strcmp(type, "BALANCE") == 0 || strcmp(type, "LOGIN") == 0) {
        snprintf(entry, sizeof(entry), "[%s] %s", time_str, type);
    } else {
        snprintf(entry, sizeof(entry), "[%s] %s: %.2f RWF", time_str, type, amount);
    }

    int idx = tx_count[user_idx] % MAX_TRANSACTIONS;
    strncpy(transactions[user_idx][idx], entry, TRANSACTION_LENGTH - 1);
    transactions[user_idx][idx][TRANSACTION_LENGTH - 1] = '\0';
    tx_count[user_idx]++;
}

static double get_amount(const char* operation) {
    double amount;
    printf("Enter amount to %s: ", operation);
    while (scanf("%lf", &amount) != 1) {
        printf("❌ Invalid input! Please enter a valid amount: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    return amount;
}

static void display_menu(void) {
    printf("=================================================\n");
    printf("                 ATM MAIN MENU                   \n");
    printf("=================================================\n");
    printf("1. Check Balance\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. View Transaction History (Last N)\n");
    printf("5. Log Out\n");
    printf("=================================================\n");
    printf("Please select an option (1-5): ");
}

static double action_check_balance(int user_idx) {
    return balances[user_idx];
}

static void action_deposit(int user_idx) {
    double amount = get_amount("deposit");
    if (amount <= 0) {
        printf("❌ Invalid amount! Deposit amount must be positive.\n\n");
        return;
    }
    balances[user_idx] += amount;
    printf("✅ Successfully deposited %.2f RWF\n", amount);
    printf("Your new balance is: %.2f RWF\n\n", balances[user_idx]);
    log_transaction(user_idx, "DEPOSIT", amount);
}

static void action_withdraw(int user_idx) {
    double amount = get_amount("withdraw");
    if (amount <= 0) {
        printf("❌ Invalid amount! Withdrawal amount must be positive.\n\n");
        return;
    }
    if (amount > balances[user_idx]) {
        printf("❌ Insufficient funds!\n");
        printf("Your current balance is: %.2f RWF\n", balances[user_idx]);
        printf("You cannot withdraw %.2f RWF\n\n", amount);
        log_transaction(user_idx, "WITHDRAWAL FAILED", amount);
        return;
    }
    balances[user_idx] -= amount;
    printf("✅ Successfully withdrew %.2f RWF\n", amount);
    printf("Your new balance is: %.2f RWF\n\n", balances[user_idx]);
    log_transaction(user_idx, "WITHDRAWAL", amount);
}

static void view_last_n_transactions(int user_idx, int N) {
    int count = tx_count[user_idx];
    int cap = count < MAX_TRANSACTIONS ? count : MAX_TRANSACTIONS;

    if (cap == 0) {
        printf("No transactions recorded yet.\n\n");
        return;
    }

    if (N > cap) N = cap;
    if (N < 1) N = 1;

    printf("\nShowing last %d transaction(s):\n\n", N);

    if (count <= MAX_TRANSACTIONS) {
        int start = count - 1;
        for (int i = 0; i < N; i++) {
            int idx = start - i;
            printf("%d. %s\n", i + 1, transactions[user_idx][idx]);
        }
    } else {
        int newest = (count - 1) % MAX_TRANSACTIONS;
        for (int i = 0; i < N; i++) {
            int idx = (newest - i + MAX_TRANSACTIONS) % MAX_TRANSACTIONS;
            printf("%d. %s\n", i + 1, transactions[user_idx][idx]);
        }
    }
    printf("\n");
}

/* ================== Public Functions ================== */
int atm_login(void) {
    char uname[MAX_NAME];
    char pinbuf[32];

    for (int attempt = 1; attempt <= MAX_LOGIN_TRIES; attempt++) {
        printf("\n=== LOGIN (attempt %d/%d) ===\n", attempt, MAX_LOGIN_TRIES);

        /* Username */
        printf("Username: ");
        if (!fgets(uname, sizeof(uname), stdin)) return -1;
        size_t n = strlen(uname);
        if (n && uname[n-1] == '\n') uname[n-1] = '\0';

        int u = find_user(uname);
        if (u < 0) {
            int left = MAX_LOGIN_TRIES - attempt;
            if (left > 0) {
                printf("Unknown user. You have %d attempt%s left.\n", left, left==1?"":"s");
                continue;
            } else {
                printf("Too many failed attempts. Access denied.\n");
                return -1;
            }
        }

        /* PIN */
        printf("PIN (4 digits): ");
        if (!fgets(pinbuf, sizeof(pinbuf), stdin)) return -1;
        n = strlen(pinbuf);
        if (n && pinbuf[n-1] == '\n') pinbuf[n-1] = '\0';

        if (!is_pin_4digits(pinbuf)) {
            int left = MAX_LOGIN_TRIES - attempt;
            if (left > 0) {
                printf("PIN must be exactly 4 digits. You have %d attempt%s left.\n",
                       left, left==1?"":"s");
                continue;
            } else {
                printf("Too many failed attempts. Access denied.\n");
                return -1;
            }
        }

        if (strcmp(pinbuf, pins[u]) != 0) {
            int left = MAX_LOGIN_TRIES - attempt;
            if (left > 0) {
                printf("Incorrect PIN. You have %d attempt%s left.\n", left, left==1?"":"s");
                continue;
            } else {
                printf("Too many failed attempts. Access denied.\n");
                return -1;
            }
        }

        /* Success */
        log_transaction(u, "LOGIN", 0.0);
        return u;
    }

    return -1; /* defensive */
}

void atm_session(int user_idx) {
    printf("\nHello, %s! Your current balance is: %.2f RWF\n\n",
           usernames[user_idx], balances[user_idx]);

    int choice;
    do {
        display_menu();
        choice = get_user_choice();

        switch (choice) {
            case 1:
                printf("\n--- BALANCE INQUIRY ---\n");
                printf("Your current balance is: %.2f RWF\n\n", action_check_balance(user_idx));
                log_transaction(user_idx, "BALANCE", 0.0);
                break;
            case 2:
                printf("\n--- DEPOSIT ---\n");
                action_deposit(user_idx);
                break;
            case 3:
                printf("\n--- WITHDRAWAL ---\n");
                action_withdraw(user_idx);
                break;
            case 4: {
                printf("\n--- TRANSACTION HISTORY ---\n");
                int maxN = tx_count[user_idx] < MAX_TRANSACTIONS ? tx_count[user_idx] : MAX_TRANSACTIONS;
                if (maxN == 0) {
                    printf("No transactions recorded yet.\n\n");
                    break;
                }
                int N;
                printf("How many recent entries to show (1-%d)? ", maxN);
                while (scanf("%d", &N) != 1) {
                    printf("Invalid input! Enter a number 1-%d: ", maxN);
                    clear_input_buffer();
                }
                clear_input_buffer();
                view_last_n_transactions(user_idx, N);
                printf("Current Balance: %.2f RWF\n\n", balances[user_idx]);
                break;
            }
            case 5:
                printf("\n--- LOG OUT ---\n");
                printf("Goodbye, %s!\n\n", usernames[user_idx]);
                break;
            default:
                printf("\n❌ Invalid choice! Please select 1-5.\n\n");
                break;
        }

        if (choice >= 1 && choice <= 4) {
            printf("Press Enter to continue...");
            getchar();
            printf("\n");
        }
    } while (choice != 5);
}

void atm_goodbye(void) {
    printf("Thank you for using our ATM service!\n");
}
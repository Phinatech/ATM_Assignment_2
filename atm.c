#include <stdio.h>
#include <string.h>
#include <time.h>

// Constants for transaction history
#define MAX_TRANSACTIONS 10
#define TRANSACTION_LENGTH 100

// Global variable to store account balance
double balance = 100000.00; // Initial balance of 100,000.00 RWF

// Transaction log - 2D array to store transaction history
char transactions[MAX_TRANSACTIONS][TRANSACTION_LENGTH];
int transaction_count = 0; // Tracks total number of transactions

// Function prototypes
void display_menu();
double check_balance();
void deposit();
void withdraw();
void view_transaction_history();
void log_transaction(const char* type, double amount);
void clear_input_buffer();
int get_user_choice();
double get_amount(const char* operation);
void get_current_time(char* time_str);

/**
 * Main function - Entry point of the ATM program
 * Implements the main loop and menu system
 */
int main() {
    int choice;
    
    printf("=================================================\n");
    printf("       WELCOME TO THE ATM MACHINE SIMULATOR     \n");
    printf("=================================================\n");
    printf("Your initial balance is: %.2f RWF\n\n", balance);
    
    // Main program loop - continues until user chooses to exit
    do {
        display_menu();
        choice = get_user_choice();
        
        switch (choice) {
            case 1:
                printf("\n--- BALANCE INQUIRY ---\n");
                printf("Your current balance is: %.2f RWF\n\n", check_balance());
                break;
                
            case 2:
                printf("\n--- DEPOSIT ---\n");
                deposit();
                break;
                
            case 3:
                printf("\n--- WITHDRAWAL ---\n");
                withdraw();
                break;
                
            case 4:
                printf("\n--- TRANSACTION HISTORY ---\n");
                view_transaction_history();
                break;
                
            case 5:
                printf("\n--- EXIT ---\n");
                printf("Thank you for using our ATM service!\n");
                printf("Have a great day!\n");
                break;
                
            default:
                printf("\n❌ Invalid choice! Please select a number between 1-5.\n\n");
                break;
        }
        
        // Pause before showing menu again (except on exit)
        if (choice >= 1 && choice <= 4) {
            printf("Press Enter to continue...");
            getchar();
            printf("\n");
        }
        
    } while (choice != 5);
    
    return 0;
}

/**
 * Displays the main ATM menu options
 */
void display_menu() {
    printf("=================================================\n");
    printf("                 ATM MAIN MENU                  \n");
    printf("=================================================\n");
    printf("1. Check Balance\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. View Transaction History\n");
    printf("5. Exit\n");
    printf("=================================================\n");
    printf("Please select an option (1-5): ");
}

/**
 * Returns the current account balance
 * @return Current balance as a double
 */
double check_balance() {
    return balance;
}

/**
 * Handles money deposit operation
 * Validates input, updates balance, and logs transaction
 */
void deposit() {
    double amount = get_amount("deposit");
    
    if (amount > 0) {
        balance += amount;
        printf("✅ Successfully deposited %.2f RWF\n", amount);
        printf("Your new balance is: %.2f RWF\n\n", balance);
        
        // Log the deposit transaction
        log_transaction("DEPOSIT", amount);
    } else {
        printf("❌ Invalid amount! Deposit amount must be positive.\n\n");
    }
}

/**
 * Handles money withdrawal operation
 * Validates input, checks for sufficient funds, and logs transaction
 */
void withdraw() {
    double amount = get_amount("withdraw");
    
    if (amount <= 0) {
        printf("❌ Invalid amount! Withdrawal amount must be positive.\n\n");
        return;
    }
    
    if (amount > balance) {
        printf("❌ Insufficient funds!\n");
        printf("Your current balance is: %.2f RWF\n", balance);
        printf("You cannot withdraw %.2f RWF\n\n", amount);
    } else {
        balance -= amount;
        printf("✅ Successfully withdrew %.2f RWF\n", amount);
        printf("Your new balance is: %.2f RWF\n\n", balance);
        
        // Log the withdrawal transaction
        log_transaction("WITHDRAWAL", amount);
    }
}

/**
 * Logs a transaction to the transaction history array
 * Uses circular buffer approach - overwrites oldest transaction when full
 * @param type Type of transaction (DEPOSIT or WITHDRAWAL)
 * @param amount Amount of the transaction
 */
void log_transaction(const char* type, double amount) {
    char time_str[20];
    char temp_transaction[TRANSACTION_LENGTH];
    
    // Get current timestamp
    get_current_time(time_str);
    
    // Format the transaction string
    snprintf(temp_transaction, TRANSACTION_LENGTH, 
             "[%s] %s: %.2f RWF", 
             time_str, type, amount);
    
    // Store in circular buffer fashion
    int index = transaction_count % MAX_TRANSACTIONS;
    strcpy(transactions[index], temp_transaction);
    
    transaction_count++;
}

/**
 * Displays transaction history
 * Shows the last N transactions (up to MAX_TRANSACTIONS)
 */
void view_transaction_history() {
    if (transaction_count == 0) {
        printf("No transactions recorded yet.\n\n");
        return;
    }
    
    printf("=================================================\n");
    printf("           TRANSACTION HISTORY                  \n");
    printf("=================================================\n");
    
    // Determine how many transactions to show
    int transactions_to_show = (transaction_count < MAX_TRANSACTIONS) 
                                ? transaction_count 
                                : MAX_TRANSACTIONS;
    
    printf("Showing last %d transaction(s):\n\n", transactions_to_show);
    
    // Display transactions in reverse chronological order (newest first)
    int start_index;
    if (transaction_count <= MAX_TRANSACTIONS) {
        // Haven't filled the buffer yet
        start_index = transaction_count - 1;
        for (int i = start_index; i >= 0; i--) {
            printf("%d. %s\n", (start_index - i + 1), transactions[i]);
        }
    } else {
        // Buffer is full, need to handle circular nature
        start_index = (transaction_count - 1) % MAX_TRANSACTIONS;
        for (int i = 0; i < MAX_TRANSACTIONS; i++) {
            int index = (start_index - i + MAX_TRANSACTIONS) % MAX_TRANSACTIONS;
            printf("%d. %s\n", (i + 1), transactions[index]);
        }
    }
    
    printf("\n=================================================\n");
    printf("Current Balance: %.2f RWF\n", balance);
    printf("=================================================\n\n");
}

/**
 * Gets current time and formats it as a string
 * @param time_str Buffer to store formatted time string
 */
void get_current_time(char* time_str) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    // Format: HH:MM:SS
    snprintf(time_str, 20, "%02d:%02d:%02d", 
             t->tm_hour, t->tm_min, t->tm_sec);
}

/**
 * Clears the input buffer to handle invalid input
 */
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Gets and validates user menu choice
 * @return Valid menu choice as integer
 */
int get_user_choice() {
    int choice;
    
    while (scanf("%d", &choice) != 1) {
        printf("❌ Invalid input! Please enter a number: ");
        clear_input_buffer();
    }
    clear_input_buffer(); // Clear any remaining characters
    
    return choice;
}

/**
 * Gets and validates monetary amount from user
 * @param operation String describing the operation (deposit/withdraw)
 * @return Amount entered by user as double
 */
double get_amount(const char* operation) {
    double amount;
    
    printf("Enter amount to %s: ", operation);
    
    while (scanf("%lf", &amount) != 1) {
        printf("❌ Invalid input! Please enter a valid amount: ");
        clear_input_buffer();
    }
    clear_input_buffer(); // Clear any remaining characters
    
    return amount;
}

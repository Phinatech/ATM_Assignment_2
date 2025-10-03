# ATM System

## Problem Statement

This project simulates an Automated Teller Machine (ATM) system using the C programming language. The system allows multiple predefined users to log in and perform basic banking operations such as checking balance, depositing, withdrawing, and viewing transaction history. The project demonstrates the use of fundamental C programming concepts such as variables, control structures, arrays, strings, and modular functions.

## Assumptions

* The system runs in a text-based console environment.
* Users are predefined and stored using arrays (usernames, PINs, balances).
* PINs are 4-digit numbers represented as strings for validation.
* Transaction history is stored in a string array for each user.
* No persistent file storage is used; data is lost once the program ends.
* Maximum number of users and maximum number of transactions per user are fixed.

## Features

1. **Login**: User enters a username and 4-digit PIN to access the system.
2. **Check Balance**: Displays the current balance of the logged-in user.
3. **Deposit Money**: Adds a specified amount to the balance and logs the transaction.
4. **Withdraw Money**: Subtracts a specified amount from the balance if sufficient funds exist and logs the transaction.
5. **View Transaction History**: Shows the last N transactions for the logged-in user.
6. **Logout**: Ends the current session and returns to the login screen.

## How to Compile and Run

1. Clone this repository:

   ```bash
   git clone https://github.com/Phinatech/ATM_Assignment_2.git
   cd ATM_Assignment_2
   ```
2. Compile the program using `gcc`:

   ```bash
   gcc atm.c -o atm
   ```
3. Run the executable:

   ```bash
   ./atm
   ```

## Sample Input/Output

```
Welcome to the ATM System
Enter username: mike
Enter PIN: 1234

Login successful!

1. Check Balance
2. Deposit
3. Withdraw
4. View Transaction History
5. Logout

Enter choice: 2
Enter amount to deposit: 5000
Deposit successful. New balance: 105000

Enter choice: 4
Transaction History:
- Deposited 5000

Enter choice: 5
Logging out...
```

## Key Functions and Design Choices

* `loginUser()` → Handles login authentication using username and PIN.
* `checkBalance()` → Displays current user’s balance.
* `deposit()` → Updates balance and logs deposit in transaction history.
* `withdraw()` → Validates funds, updates balance, and logs withdrawal.
* `viewTransactions()` → Prints last N transactions stored in a 2D string array.
* `logout()` → Ends session and returns to main login prompt.

The program follows a **modular design** where each operation is encapsulated in its own function, improving readability and maintainability. Arrays and strings are used to manage multiple users and their transaction histories.

## Contributors

* **Tamunotonye Briggs** – Check Balance, Deposit, Withdraw, Transaction History
* **Judith Ugbo** – User Login, User authentication, Logout

## License

This project is for academic purposes only.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct BankAccount {
    int accountNumber;
    char name[50];
    float balance;
};

struct Loan {
    int accountNumber;
    float loanAmount;
    float amountRepaid;
};

void createAccount() {
    struct BankAccount acc;
    FILE *fp = fopen("bank.txt", "a");

    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &acc.accountNumber);
    while (getchar() != '\n'); // clear buffer

    printf("Enter Name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = '\0';

    printf("Enter Initial Balance: ");
    scanf("%f", &acc.balance);

    fprintf(fp, "%d %s %.2f\n", acc.accountNumber, acc.name, acc.balance);
    fclose(fp);
    printf("Account created successfully.\n");
}

void viewAccount() {
    struct BankAccount acc;
    int accNum, found = 0;
    FILE *fp = fopen("bank.txt", "r");

    if (!fp) {
        printf("No accounts found.\n");
        return;
    }

    printf("Enter Account Number to View: ");
    scanf("%d", &accNum);

    while (fscanf(fp, "%d %s %f", &acc.accountNumber, acc.name, &acc.balance) == 3) {
        if (acc.accountNumber == accNum) {
            printf("\nAccount No: %d\nName: %s\nBalance: %.2f\n", acc.accountNumber, acc.name, acc.balance);
            found = 1;
            break;
        }
    }

    fclose(fp);
    if (!found) printf("Account not found.\n");
}

void depositAmount() {
    struct BankAccount acc;
    int accNum;
    float amount;
    int found = 0;

    FILE *fp = fopen("bank.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    printf("Enter Account Number to Deposit: ");
    scanf("%d", &accNum);
    printf("Enter Amount: ");
    scanf("%f", &amount);

    while (fscanf(fp, "%d %s %f", &acc.accountNumber, acc.name, &acc.balance) == 3) {
        if (acc.accountNumber == accNum) {
            acc.balance += amount;
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", acc.accountNumber, acc.name, acc.balance);
    }

    fclose(fp);
    fclose(temp);
    remove("bank.txt");
    rename("temp.txt", "bank.txt");

    if (found) printf("Deposit successful.\n");
    else printf("Account not found.\n");
}

void withdrawAmount() {
    struct BankAccount acc;
    int accNum;
    float amount;
    int found = 0;

    FILE *fp = fopen("bank.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    printf("Enter Account Number to Withdraw: ");
    scanf("%d", &accNum);
    printf("Enter Amount: ");
    scanf("%f", &amount);

    while (fscanf(fp, "%d %s %f", &acc.accountNumber, acc.name, &acc.balance) == 3) {
        if (acc.accountNumber == accNum) {
            if (acc.balance >= amount) {
                acc.balance -= amount;
                found = 1;
            } else {
                printf("Insufficient balance.\n");
            }
        }
        fprintf(temp, "%d %s %.2f\n", acc.accountNumber, acc.name, acc.balance);
    }

    fclose(fp);
    fclose(temp);
    remove("bank.txt");
    rename("temp.txt", "bank.txt");

    if (found) printf("Withdrawal successful.\n");
    else printf("Account not found or withdrawal failed.\n");
}

void deleteAccount() {
    struct BankAccount acc;
    int accNum;
    int found = 0;

    FILE *fp = fopen("bank.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    printf("Enter Account Number to Delete: ");
    scanf("%d", &accNum);

    while (fscanf(fp, "%d %s %f", &acc.accountNumber, acc.name, &acc.balance) == 3) {
        if (acc.accountNumber != accNum) {
            fprintf(temp, "%d %s %.2f\n", acc.accountNumber, acc.name, acc.balance);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("bank.txt");
    rename("temp.txt", "bank.txt");

    if (found) printf("Account deleted successfully.\n");
    else printf("Account not found.\n");
}

void listAccounts() {
    struct BankAccount acc;
    FILE *fp = fopen("bank.txt", "r");

    if (!fp) {
        printf("No accounts found.\n");
        return;
    }

    printf("\n--- All Bank Accounts ---\n");
    while (fscanf(fp, "%d %s %f", &acc.accountNumber, acc.name, &acc.balance) == 3) {
        printf("Account No: %d | Name: %s | Balance: %.2f\n", acc.accountNumber, acc.name, acc.balance);
    }
    fclose(fp);
}

// --- Loan Functions ---

void applyLoan() {
    struct Loan loan;
    FILE *fp = fopen("loan.txt", "a");

    printf("Enter Account Number: ");
    scanf("%d", &loan.accountNumber);
    printf("Enter Loan Amount: ");
    scanf("%f", &loan.loanAmount);
    loan.amountRepaid = 0.0;

    fprintf(fp, "%d %.2f %.2f\n", loan.accountNumber, loan.loanAmount, loan.amountRepaid);
    fclose(fp);
    printf("Loan Applied Successfully.\n");
}

void viewLoan() {
    struct Loan loan;
    int accNum, found = 0;
    FILE *fp = fopen("loan.txt", "r");

    printf("Enter Account Number: ");
    scanf("%d", &accNum);

    while (fscanf(fp, "%d %f %f", &loan.accountNumber, &loan.loanAmount, &loan.amountRepaid) == 3) {
        if (loan.accountNumber == accNum) {
            printf("Loan Amount: %.2f\nRepaid: %.2f\nRemaining: %.2f\n",
                   loan.loanAmount, loan.amountRepaid, loan.loanAmount - loan.amountRepaid);
            found = 1;
            break;
        }
    }
    fclose(fp);
    if (!found) printf("Loan not found.\n");
}

void repayLoan() {
    struct Loan loan;
    int accNum;
    float repay;
    int found = 0;

    FILE *fp = fopen("loan.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    printf("Enter Account Number: ");
    scanf("%d", &accNum);
    printf("Enter Repayment Amount: ");
    scanf("%f", &repay);

    while (fscanf(fp, "%d %f %f", &loan.accountNumber, &loan.loanAmount, &loan.amountRepaid) == 3) {
        if (loan.accountNumber == accNum) {
            if (repay <= (loan.loanAmount - loan.amountRepaid)) {
                loan.amountRepaid += repay;
                found = 1;
            } else {
                printf("Repayment exceeds remaining loan.\n");
            }
        }
        fprintf(temp, "%d %.2f %.2f\n", loan.accountNumber, loan.loanAmount, loan.amountRepaid);
    }

    fclose(fp);
    fclose(temp);
    remove("loan.txt");
    rename("temp.txt", "loan.txt");

    if (found) printf("Repayment successful.\n");
    else printf("Loan record not found.\n");
}

// --- Main Menu ---

int main() {
    int choice;

    do {
        printf("\n==== Bank Management System ====\n");
        printf("1. Create Account\n2. View Account\n3. Deposit Amount\n4. Withdraw Amount\n");
        printf("5. Delete Account\n6. List All Accounts\n7. Apply for Loan\n8. View Loan\n9. Repay Loan\n0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: viewAccount(); break;
            case 3: depositAmount(); break;
            case 4: withdrawAmount(); break;
            case 5: deleteAccount(); break;
            case 6: listAccounts(); break;
            case 7: applyLoan(); break;
            case 8: viewLoan(); break;
            case 9: repayLoan(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }

    } while (choice != 0);

    return 0;
}

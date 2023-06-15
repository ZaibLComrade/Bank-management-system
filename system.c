#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define StrLen 32
#define PassLength 64
#define DBLen 16
#define logLen 126
typedef enum {false, true} bool;

// Terminal functions
void termicont() {
    printf("Press any key to continue...\n");
    getchar();
    getchar();
}
void clrscr() {
    system("@cls||clear");
    /*
        Clear console
        Works on linux
    */
}

typedef struct user {
    char firstName[StrLen];
    char lastName[StrLen];
    char username[StrLen];
    char password[StrLen];
    char email[StrLen];
    char phone[StrLen];
    double balance;
    int notification;
} User;
typedef struct {
    char sender[StrLen];
    double amount;
    char reciever[StrLen];
    char state[StrLen];
    int sl;
} Transaction;

/*
    This database is implemented using a static array.
    Future version may have dynamically allocated database.
*/
User database[DBLen];
char fileExtension[] = "TLog.dat"; // Suffix of transaction log file
char centralDatabase[] = "CentralDatabase.dat"; // Transaction histroy database
char userData[] = "userData.dat";
/*
    Database notes:
    If added or removed a column of data, readUser(), readhead(),
    sortDatabase(), createAccount() functions must be modified.

    To modifiy userData file name:
    loadData(), sortDatabase(), createAccount() has has filename
    information.

    printUserInfo modification will ease debugging
*/

// Database functions
int getString(FILE *in, char str[]) {
    char ch;
    char delim = ',';
    int i = 0;
    str[i] = '\0';
    while(isspace(ch = getc(in)));
    if(ch == EOF) return 1;
    do {
        str[i++] = ch;
    } while((ch = getc(in)) != delim && ch != EOF && ch != '\n');
    str[i] = '\0';
    return 0; // Return type checks wheather end of file reached
}
User readUser(FILE *in) {
    User temp;
    int isEOF = 0;
    isEOF = getString(in, temp.firstName);
    getString(in, temp.lastName);
    getString(in, temp.username);
    getString(in, temp.email);
    getString(in, temp.password);
    getString(in, temp.phone);
    fscanf(in, "%lf", &temp.balance);
    char ch;
    if(!isEOF) ch = getc(in); // Reads comma
    fscanf(in, "%d", &temp.notification);
    if(isEOF) strcpy(temp.firstName, "END");  // End of file info
    // If the file has reached end, firstName of struct temp will
    // contain the string "END"
    return temp;
}
User readHead(FILE *in) {
    // Reads head of table
    User temp;
    int isEOF = 0;
    isEOF = getString(in, temp.firstName);
    getString(in, temp.lastName);
    getString(in, temp.username);
    getString(in, temp.email);
    getString(in, temp.password);
    getString(in, temp.phone);
    char voidString[StrLen];
    getString(in, voidString);  // Reads "Balance" as void
    getString(in, voidString); // Reads "Notificaiton" as void
    /*
        Reads the head "balance" as void string since "balance"
        itself is a string but columnt contains double type
        of data.
    */

    if(isEOF) strcpy(temp.firstName, "END");  // End of file info
    // If the file has reached end, firstName of struct temp will
    // contain the string "END"
    return temp;
}
void printUserInfo(User var) {
    // A typical debugging function
    char name[StrLen];
    strcpy(name, var.firstName);
    strcat(name, " ");
    strcat(name, var.lastName);

    // printf("First Name: %s\n", var.firstName);
    // printf("Last Name: %s\n", var.lastName);
    printf("\n");
    printf("Name: %s\n", name);
    printf("Username: %s\n", var.username);
    printf("Email: %s\n", var.email);
    printf("password: %s\n", var.password);
    printf("Phone: %s\n", var.phone);
    printf("Balance: %0.2lf$\n", var.balance);
    printf("Notification: %d\n", var.notification);
    printf("\n");
}
int loadData(User database[]) {
    // Loads database from file into stack memory
    FILE *in = fopen(userData, "r");
    if(in == NULL) {
        // File not found
        return 0;
    }
    int count = 0;
    readHead(in); // Reading Table heads as void

    User temp;
    do {
        temp = readUser(in);
        database[count++] = temp;
    } while(strcmp(temp.firstName, "END")); // END of data value
    return 1; // Successfully loaded
}
void sortDatabase(User database[]) {
    // Sorts the database using insertion sort
    int count = 0;
    int cmp;
    do {
        cmp = strcmp(database[count].firstName, "END");
        if(cmp) count++;
    } while(cmp);
    // At this point, count contains number of elements
    // present in database

    for(int i = 0; i < count; i++) {
        int k = i - 1;
        User key = database[i];
        while(k >= 0 && strcmp(key.username, database[k].username) < 0) {
            database[k + 1] = database[k];
            --k;
        }
        database[k + 1] = key;
    }

    // Rewriting the database with sorted data
    FILE *out = fopen(userData, "w");
    fprintf(out, "First Name,Last Name,Username,Email,Password,Phone,Balance,Notification");
    for(int i = 0; i < count; i++) {
        fprintf(out, "\n");
        fprintf(out, "%s,", database[i].firstName);
        fprintf(out, "%s,", database[i].lastName);
        fprintf(out, "%s,", database[i].username);
        fprintf(out, "%s,", database[i].email);
        fprintf(out, "%s,", database[i].password);
        fprintf(out, "%s,", database[i].phone);
        fprintf(out, "%0.2lf,", database[i].balance);
        fprintf(out, "%d", database[i].notification);
    }
    fclose(out);
}
int binarySearch(User database[], char key[]) {
    // Searches a sorted (by username) database by username
    int count = 0;
    int cmp;
    do {
        cmp = strcmp(database[count].firstName, "END");
        if(cmp) count++;
    } while(cmp);
    // count contains number of elements
    int lo = 0, hi = count;
    int mid;
    while(lo <= hi) {
        mid = (lo + hi) / 2;
        int cmp = strcmp(key, database[mid].username);
        if(cmp == 0) return mid;
        if(cmp < 0) hi = mid - 1;
        else lo = mid + 1;
    }
    return -1; // If key doesn't match any entry
}
Transaction readTransaction(FILE *tr, bool isHeader) {
    // Reads transaction log file
    Transaction temp;
    bool isEOF;

    isEOF = getString(tr, temp.sender);
    if(isHeader) {
        char voidString[StrLen];
        getString(tr, voidString); // Reads "Amount" header as void
    } else {
        fscanf(tr, "%lf", &temp.amount);
        char ch;
        if(!isEOF) ch = getc(tr); // Reads comma after amount
    }
    getString(tr, temp.reciever);
    getString(tr, temp.state);

    if(isEOF) strcpy(temp.sender, "END"); // End of data value
    if(!isHeader) return temp; // Returns transaction data;
}
double loadTransaction(char fileName[], int userIndex) {
    // Returns amount of recieved money

    FILE *trLog = fopen(fileName, "r");
    Transaction log[logLen];
    int slCount = 1; // Reciever serial count
    readTransaction(trLog, true);
    printf("SL. Sender   Reciever   Amount   Status\n");
    log[slCount] = readTransaction(trLog, false);
    int cmp = strcmp(log[slCount].sender, "END");
    while(cmp) {
        // Reads ans writes to terminal at the same time

        log[slCount].sl = slCount; // Serial no.
        printf("%2d.", log[slCount].sl);
        printf("%5s", log[slCount].sender);
        printf("%13s", log[slCount].reciever);
        printf("%9.2lf$", log[slCount].amount);
        printf("%10s", log[slCount].state);
        printf("\n");
        log[++slCount] = readTransaction(trLog, false);
        cmp = strcmp(log[slCount].sender, "END");
    }

    int sl; // Chosen to interact
    printf("Choose transaction (0 to exit): ");
    scanf("%d", &sl);
    if(!sl) return 0; // Exit function
    
    void writeTransaction() {
        // Writes changes to log file
        trLog = fopen(fileName, "w");
        fprintf(trLog, "Sender,amount,Reciever,State\n");
        for(int i = 1; i < slCount; i++) {
            fprintf(trLog, "%s,", log[i].sender);
            fprintf(trLog, "%0.2lf,", log[i].amount);
            fprintf(trLog, "%s,", log[i].reciever);
            fprintf(trLog, "%s", log[i].state);
            fprintf(trLog, "\n");
        }
    }

    // Operations
    int op;
    printf("1. Recieve\n");
    printf("2. Delete\n");
    printf("0. Exit\n");
    printf("Choose operation: ");
    scanf("%d", &op);
    double holdAmount = 0; // Amount to be returned
    switch(op) {
        case 1:
            if(!strcmp(log[sl].state, "Pending")
            && strcmp(log[sl].sender, database[userIndex].username)) {
                holdAmount = log[sl].amount;
            }
            if(!strcmp(log[sl].state, "Recieved")) {
                printf("Money already recieved\n");
            }
            strcpy(log[sl].state, "Recieved");
            writeTransaction();

            /*
                Future version feature: Send notification
                and recieve status to sender
            */
        break;
        case 2:
            printf("Note: *--Function Under Consideration--*");
        break;
        case 0:
            return 0;
        break;
    }
    fclose(trLog);
    return holdAmount;
}

// New Database functions


// UI functions
void changePass(int index) {
    int notMatch;
    char newPass[PassLength];
    do {
        printf("New password: ");
        scanf("%s", newPass);
        char tempPass[PassLength];
        printf("Repeat password: ");
        scanf("%s", tempPass);
        notMatch = strcmp(newPass, tempPass);
        if(notMatch) printf("Passwords don't match. Please try again\n");
    } while(notMatch);
    strcpy(database[index].password, newPass);
    printf("Password Changed Successfully\n\n");
    sortDatabase(database);
}
void deposit(int index) {
    double amount = 0;
    printf("----DEPOSIT----\n");
    printf("Enter deposit amount: ");
    scanf("%lf", &amount);
    database[index].balance += amount;

    //test
    // temp -> balance += amount;

    sortDatabase(database); // Writes to original database
    loadData(database); // Refreshes loaded database

    printf("Amount deposited successfully\n\n");
}
void withdraw(int index) {
    double amount = 0;
    printf("----WITHDRAW----\n");
    printf("Enter withdraw amount: ");
    scanf("%lf", &amount);
    if(database[index].balance >= amount) {
        database[index].balance -= amount;
        printf("Amount withdrawn successfully\n");
        sortDatabase(database);
    } else {
        printf("Not sufficient money\n");
    }
    printf("\n");
}
void sendMoney(int index) {

    User temp = database[index];
    char username[StrLen];
    int rcvIndex;  // To access reciver's data
    double amount;

    // Collecting information
    printf("Enter username of receiver: ");
    scanf("%s", username);
    rcvIndex = binarySearch(database, username);
    if(rcvIndex == -1) {
        printf("User not found\n");
        return;
    }
    printf("Enter amount to be transacted: ");
    scanf("%lf", &amount);
    if(amount > database[index].balance) {
        printf("Not sufficient money\n");
        return;
    }
    termicont();

    // Function to write transaction log
    void fprintTransaction(FILE *ptr) {
        fprintf(ptr, "%s,", temp.username);
        fprintf(ptr, "%0.2lf,", amount);
        fprintf(ptr, "%s,", username);
        fprintf(ptr, "Pending");
        fprintf(ptr, "\n");
    }
    
    FILE *fp;
    
    char fileName[StrLen];
    char headerString[126] = "Sender,Amount,Reciever,State";

    // Sender's log
    strcpy(fileName, temp.username);
    strcat(fileName, fileExtension);
    fp = fopen(fileName, "r");
    bool appendMode = true;
    char ch;
    if(fp == NULL || (ch = getc(fp)) == EOF) appendMode = false;
    if(!appendMode) {
        fp = fopen(fileName, "w");
        fprintf(fp, "%s\n", headerString);
    } else {
        fp = fopen(fileName, "a");
    }
    fprintTransaction(fp);
    fclose(fp);

    // Adding data to central database
    strcpy(fileName, centralDatabase);
    FILE *mainDB = fopen(fileName, "a");
    fprintTransaction(mainDB);
    fclose(mainDB);

    database[index].balance -= amount;
    database[rcvIndex].notification++; // Notifying user of new transaction
    sortDatabase(database); // Write Changes

    // Transacting to reciever transaction log
    strcpy(fileName, username);
    strcat(fileName, fileExtension);
    FILE *rcv = fopen(fileName, "r");
    appendMode = true;
    if(rcv == NULL || (ch = getc(rcv)) == EOF) appendMode = false;

    if(!appendMode) {
        rcv = fopen(fileName, "w");
        fprintf(rcv, "%s\n", headerString);
    } else {
        rcv = fopen(fileName, "a");
    }
    fprintTransaction(rcv);
    fclose(rcv);
    printf("Transaction Pending\n");
}
void notification(int index) {
    database[index].notification = 0;
    FILE *fp;
    char fileName[StrLen];
    strcpy(fileName, database[index].username);
    strcat(fileName, fileExtension);

    // Opening user transaction log file
    fp = fopen(fileName, "r");
    if(fp == NULL) {
        printf("Error: Transaction log not found\n");
        return;
    }
    double recievedAmount = loadTransaction(fileName, index);
    database[index].balance += recievedAmount;

    sortDatabase(database); // Write changes
}

// System functions
void createAccount() {
    printf("----SIGN UP----\n");
    // Initializing varaibles
    User temp;
    temp.balance = 0;
    temp.notification = 0;
    // Prompt user for details
    printf("First name: ");
    scanf("%s", temp.firstName);
    printf("Last name: ");
    scanf("%s", temp.lastName);
    printf("Choose username: ");
    scanf("%s", temp.username);
    printf("Email Address: ");
    scanf("%s", temp.email);

    // Promp for password while both entry doesn't match
    int notMatch;
    do {
        printf("New password: ");
        scanf("%s", temp.password);
        char tempPass[PassLength];
        printf("Repeat password: ");
        scanf("%s", tempPass);
        notMatch = strcmp(temp.password, tempPass);
        if(notMatch) printf("Passwords don't match. Please try again\n");
    } while(notMatch);
    printf("Enter phone number: ");

    scanf("%s", temp.phone);

    FILE *out;
    out = fopen(userData, "r");
    char ch;
    bool appendMode = true;
    if(out == NULL || (ch = getc(out)) == EOF) appendMode = false;
    if(!appendMode) {
        out = fopen(userData, "w");
        fprintf(out, "First Name,Last Name,Username,Email,Password,Phone,Balance,Notificaion");
    } else out = fopen(userData, "a");
    fprintf(out, "\n");
    fprintf(out, "%s,", temp.firstName);
    fprintf(out, "%s,", temp.lastName);
    fprintf(out, "%s,", temp.username);
    fprintf(out, "%s,", temp.email);
    fprintf(out, "%s,", temp.password);
    fprintf(out, "%s,", temp.phone);
    fprintf(out, "%0.2lf,", temp.balance);
    fprintf(out, "%d", temp.notification);
    fclose(out);

    loadData(database); // Loads database with changes
    sortDatabase(database); // Sorts database
    loadData(database); // Loads sorted database

    printf("Account created successfully\n");
    termicont();
    clrscr();
    // Reloads database everytime a new Account is created
}
int loginPage() {
    clrscr(); // Refresh console
    // Returns user index if logged in
    int profileIndex;

    char username[StrLen];
    // Req: Username validation
    char password[PassLength];
    // Req: Password encryption
    // Req: Password validity  

    // Prompts for username and passord
    int login() {
        printf("----LOGIN----\n");
        printf("Enter Username: ");
        scanf("%s", username);
        int pfIndex = binarySearch(database, username);
        if(pfIndex == -1) { // If account not found
            printf("Account not found. Try again.\n");
            termicont();
            loginPage(); // Go back to login page
        }
        printf("Enter Password: ");
        scanf("%s", password);
        printf("\n");
        int cmpPass = strcmp(password, database[pfIndex].password);
        if(cmpPass == 0) {
            printf("Gained Access!\n");
            return pfIndex;
        }
        else {
            printf("Access denied!\n");
            pfIndex = loginPage();
            return pfIndex;
        }
        printf("\n");
    }
    // If account is not created
    
    // Asks whether account exists
    int login_or_create() {
        int exist;
        printf("++++++++++++++++++++++++++++++\n");
        printf("====Bank Management System====\n");
        printf("++++++++++++++++++++++++++++++\n");
        printf("\n\n");
        printf("Customer login\n");
        printf("==============\n");
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("0. Exit\n");
        printf("Select: ");
        scanf("%d", &exist);
        if(!exist) exit(1);
        exist -= 1; //Converting to boolean value
        printf("\n");
        if(exist) {
            int pIndex = login();
            termicont();
            return pIndex;
        }
        else {
            createAccount();
            loginPage();
        }
    }

    profileIndex = login_or_create();
    return profileIndex;
}

int main(void) {
    int successfully_loaded = loadData(database);
    if(successfully_loaded) sortDatabase(database);
    // int count = 0;
    // while(strcmp(database[count].firstName, "END")) {
    //     printUserInfo(database[count++]);
    //     printf("\n");
    // }

    int userIndex;
    // Under Construction
    void customerDashboard(int dbIndex) {
        User loggedInUser = database[dbIndex];
        char name[StrLen];
        strcpy(name, loggedInUser.firstName);
        strcat(name, " ");
        strcat(name, loggedInUser.lastName);
        char greetings[StrLen] = "Hello, ";
        // Modifying an undeclared memory string var can cause error
        strcat(greetings, name);
        char underLine[StrLen];
        strcpy(underLine, greetings);
        memset(underLine, '=', strlen(greetings));
        // Concatead first and last name and created dynamic underline

        clrscr();
        printf("%s\n", greetings);
        printf("%s\n", underLine);
        printf("Current balance: %0.2lf$\n", loggedInUser.balance);
        printf("\n");
        int chosen;
        printf("Options:\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Send Money\n");
        printf("4. Notifications (%d)\n", loggedInUser.notification);
        printf("5. View Transaction History\n");
        printf("6. Update Account Details\n");
        printf("7. Logout\n");
        printf("8. Change Password\n");
        printf("0. Exit\n");
        printf("Choose: ");
        scanf("%d", &chosen);
        switch(chosen) {
            case 1: 
                deposit(dbIndex);    
            break;
            case 2:
                withdraw(dbIndex);
            break;
            case 3:
                sendMoney(dbIndex);
            break;
            case 4:
                notification(dbIndex);
            break;
            case 5: 
                // Under Construction
            break;
            case 6: 
                // Under Construction
            break;
            case 7:
                printf("Logging out\n");
                termicont();
                clrscr();
                userIndex = loginPage();
                customerDashboard(userIndex);
            break;
            case 8: 
                changePass(dbIndex);
            break;
            case 0:
                exit(1);
            break;
        }
        termicont();
        clrscr();
        customerDashboard(dbIndex);
    }
    userIndex = loginPage();
    customerDashboard(userIndex);
}
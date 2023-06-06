#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define StrLen 32
#define PassLength 64
#define DBLen 16
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
} User;

/*
    This database is implemented using a static array.
    Future version may have dynamically allocated database.
*/
User database[DBLen];

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
    getString(in, voidString); 
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
    printf("\n");
}
void loadData(User database[]) {
    // Loads database from file into stack memory
    FILE *in = fopen("userData.csv", "r");
    int count = 0;
    readHead(in); // Reading Table heads as void

    User temp;
    do {
        temp = readUser(in);
        database[count++] = temp;
    } while(strcmp(temp.firstName, "END")); // END of data value
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
    FILE *out = fopen("userData.csv", "w");
    fprintf(out, "First Name,Last Name,Username,Email,Password,Phone,Balance");
    for(int i = 0; i < count; i++) {
        fprintf(out, "\n");
        fprintf(out, "%s,", database[i].firstName);
        fprintf(out, "%s,", database[i].lastName);
        fprintf(out, "%s,", database[i].username);
        fprintf(out, "%s,", database[i].email);
        fprintf(out, "%s,", database[i].password);
        fprintf(out, "%s,", database[i].phone);
        fprintf(out, "%0.2lf", database[i].balance);
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

// Dashboard functions
void changePass(User temp) {
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
    printf("Password Changed Successfully\n\n");
    int index = binarySearch(database, temp.username);
    strcpy(database[index].password, temp.password);
    sortDatabase(database);
}
void deposit(User temp) {
    double amount = 0;
    printf("----DEPOSIT----\n");
    printf("Enter deposit amount: ");
    scanf("%lf", &amount);
    int index = binarySearch(database, temp.username);
    database[index].balance += amount;
    sortDatabase(database); // Writes to original database
    loadData(database); // Refreshes loaded database

    // Debug
    printUserInfo(temp);
    printUserInfo(database[index]);

    printf("Amount deposited successfully\n\n");
}
void withdraw(User temp) {
    double amount = 0;
    printf("----WITHDRAW----\n");
    printf("Enter withdraw amount: ");
    scanf("%lf", &amount);
    int index = binarySearch(database, temp.username);
    if(database[index].balance >= amount) {
        database[index].balance -= amount;
        printf("Amount withdrawn successfully\n");
        sortDatabase(database);
    } else {
        printf("Not sufficient money\n");
    }
    printf("\n");
}
void sendMoney(User temp) {
    char username[StrLen];
    double amount;
    printf("Enter email of receiver: ");
    scanf("%s", username);
    printf("Enter amount to be transacted: ");
    scanf("%lf", &amount);
    // termicont();
    
    FILE *fp;
    char fileName[StrLen] = "";
    strcpy(fileName, temp.username);
    strcat(fileName, "TLog.dat");

    // debug
    printf("Debug:\n");
    printf("FileName: %s\n", fileName);

    fp = fopen(fileName, "a");
    int appendMode = 0;
    char ch;
    if((ch = getc(fp)) == EOF) appendMode = 1;

    printf("appendMode: %d\n", appendMode);
    
    if(appendMode == 0) {
        printf("Sender,Reciver,Reciever,State\n");
    } else{
        fp = fopen(fileName, "a");
        fprintf(fp, "%s,", temp.username);
        fprintf(fp, "%0.2lf,", amount);
        fprintf(fp, "%s,", username);
        fprintf(fp, "Pending");
    }
    fclose(fp);
}

// UI functions
void createAccount() {
    // Initializing varaibles
    User temp;
    temp.balance = 0;
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

    FILE *check = fopen("userData.csv", "r");
    int appendMode = 0;
    char ch;
    /* 
        Check if file is pre-exixsted
        If file exists, open file in append mode
        If not, create file and open in write mode
        Creating new file will add table header at the beginning of file 
    */
    if((ch = getc(check)) != EOF) appendMode = 1;
    fclose(check);

    FILE *out;
    if(appendMode) out = fopen("userData.csv", "a");
    else out = fopen("userData.csv", "w");
    if(!appendMode) {
        fprintf(out, "First Name,Last Name,Username,Email,Password,Phone,Balance");
    }
    fprintf(out, "\n");
    fprintf(out, "%s,", temp.firstName);
    fprintf(out, "%s,", temp.lastName);
    fprintf(out, "%s,", temp.username);
    fprintf(out, "%s,", temp.email);
    fprintf(out, "%s,", temp.password);
    fprintf(out, "%s", temp.phone);
    fprintf(out, "%0.2lf", temp.balance);
    fclose(out);

    sortDatabase(database);
    printf("Account created successfully");
    termicont();
    clrscr();
    // Reloads database everytime a new Account is created
}
int loginPage() {
    // Returns user index if logged in
    int profileIndex;

    char username[StrLen];
    // Req: Username validation
    char password[PassLength];
    // Req: Password encryption
    // Req: Password validity  

    // Prompts for username and passord
    void login() {
        printf("Enter Username: ");
        scanf("%s", username);
        profileIndex = binarySearch(database, username);
        if(profileIndex == -1) { // If account not found
            printf("Account not found. Try again.\n");
            loginPage(); // Go back to login page
        }
        printf("Enter Password: ");
        scanf("%s", password);
        printf("\n");
        int cmpPass = strcmp(password, database[profileIndex].password);
        if(cmpPass == 0) {
            printf("Gained Access!\n");
            termicont();
        }
        else {
            printf("Access denied!\n");
            loginPage();
        }
        printf("\n");
    }
    // If account is not created
    
    // Asks whether account exists
    void login_or_create() {
        int exist;
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("0. Exit\n");
        printf("Select: ");
        scanf("%d", &exist);
        if(!exist) exit(1);
        exist -= 1; //Converting to boolean value
        printf("\n");
        if(exist) login();
        else {
            createAccount();
            loginPage();
        }
    }

    login_or_create();
    return profileIndex;
}

int main(void) {
    loadData(database);
    sortDatabase(database);
    
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
        printf("4. Notifications\n");
        printf("5. View Transaction History\n");
        printf("6. Update Account Details\n");
        printf("7. Logout\n");
        printf("8. Change Password\n");
        printf("0. Exit\n");
        printf("Choose: ");
        scanf("%d", &chosen);
        switch(chosen) {
            case 1: 
                deposit(loggedInUser);    
                /*
                    Bug: current balance output remains same
                    even after currency is deposited succesfully
                    Changes take place after program complete
                    executing. In other words, Changes are 
                    written into database when programs exits.
                */
            break;
            case 2:
                withdraw(loggedInUser);
            break;
            case 3:
                sendMoney(loggedInUser);
            break;
            case 4:
                // Under Construction
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
            break;
            case 8: 
                changePass(loggedInUser);
            break;
            case 0:
                exit(1);
            break;
        }
        termicont();
        clrscr();
        customerDashboard(dbIndex);
    }
    printf("++++++++++++++++++++++++++++++\n");
    printf("====Bank management System====\n");
    printf("++++++++++++++++++++++++++++++\n");
    printf("\n\n");
    printf("Customer login\n");
    printf("==============\n");
    userIndex = loginPage();
    customerDashboard(userIndex);
}
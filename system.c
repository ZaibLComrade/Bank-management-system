#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define StrLen 32
#define PassLength 64
#define DBLen 16

// typedef enum {customer, manager, admin} AccountType;
typedef struct user {
    char firstName[StrLen];
    char lastName[StrLen];
    char username[StrLen];
    char password[StrLen];
    char email[StrLen];
    char phone[StrLen];
} User;

typedef struct {
    User finalUser;
    int type;
    double balance;
}

// Req -> Feature that is not yet added but will be worked
// on a future version

/*
    This database is implemented using a static array.
    Future version will have dynamically allocated database.
*/
User database[DBLen];

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
    if(isEOF) strcpy(temp.firstName, "END");  // End of file info
    // If the file has reached end, firstName of struct temp will
    // contain the string "END"
    return temp;
}
void printUserInfo(User var) {
    char name[StrLen];
    strcpy(name, var.firstName);
    strcat(name, " ");
    strcat(name, var.lastName);

    // printf("First Name: %s\n", var.firstName);
    // printf("Last Name: %s\n", var.lastName);
    printf("Name: %s\n", name);
    printf("Username: %s\n", var.username);
    printf("Email: %s\n", var.email);
    printf("password: %s\n", var.password);
    printf("Phone: %s\n", var.phone);
}
void loadData(User database[]) {
    // Loads database from file into stack memory
    FILE *in = fopen("userData.csv", "r");
    int count = 0;
    readUser(in); // Reading Table heads as void

    User temp;
    do {
        temp = readUser(in);
        database[count++] = temp;
    } while(strcmp(temp.firstName, "END")); // END of data value
}
void sortDatabase(User database[]) {
    loadData(database); // Reloads Database before sorting
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
    fprintf(out, "First Name,Last Name,Username,Email,Password,Phone");
    for(int i = 0; i < count; i++) {
        fprintf(out, "\n");
        fprintf(out, "%s,", database[i].firstName);
        fprintf(out, "%s,", database[i].lastName);
        fprintf(out, "%s,", database[i].username);
        fprintf(out, "%s,", database[i].email);
        fprintf(out, "%s,", database[i].password);
        fprintf(out, "%s", database[i].phone);
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


// UI functions
int menu(void) {
    printf("====Bank Management System====\n");
    printf("Select account type\n");
    printf("1. Customer\n");
    printf("2. Manager\n");
    printf("3. Admin\n");
    printf("0. Exit\n");
    int accType; // Type of chosen account
    int invalid; // Checks whether user unput is valid
    do {
        // Prompts user for account type while input is not valid
        printf("Type: ");
        scanf("%d", &accType);
        invalid = (accType < 0 || accType > 3);
        if(invalid) printf("Not a valid type. please try again.\n");
    } while(invalid);
    printf("\n");
    if(!accType) exit(1);
    return accType;
}
void createAccount() {
    // Initializing varaibles
    User temp;

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
        fprintf(out, "First Name,Last Name,Username,Email,Password,Phone");
    }
    fprintf(out, "\n");
    fprintf(out, "%s,", temp.firstName);
    fprintf(out, "%s,", temp.lastName);
    fprintf(out, "%s,", temp.username);
    fprintf(out, "%s,", temp.email);
    fprintf(out, "%s,", temp.password);
    fprintf(out, "%s", temp.phone);
    fclose(out);

    sortDatabase(database);
    // Reloads database everytime a new Account is created
}
void loginPage() {
    char username[StrLen];
    // Req: Username validation
    char password[PassLength];
    // Req: Password encryption
    // Req: Password validity  

    // Prompts for username and passord
    void login() {
        printf("Enter Username: ");
        scanf("%s", username);
        int profileIndex = binarySearch(database, username);
        if(profileIndex == -1) { // If account not found
            printf("Account not found. Try again.\n");
            loginPage(); // Go back to login page
        }
        printf("Enter Password: ");
        scanf("%s", password);
        printf("\n");
        int gainedAccess = 0;
        int cmpPass = strcmp(password, database[profileIndex].password);
        if(cmpPass == 0) printf("Gained Access!\n");
        else {
            printf("Access denied!\n");
            loginPage();
        }
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
}

// Dashboards
void customerPrompt(void) {
    // Under Construction
    void customerDashboard() {
        printf("Hello /User/\n");
        printf("Current balance:\n");
        printf("\n");
        printf("Options:\n");
        printf("1. Change Password\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Send Money\n");
        printf("5. View Transaction History\n");
        printf("6. Update Account Details\n");
        printf("7. Logout\n");
        printf("0. Exit\n");
        printf("\n");
    }
    printf("Customer login\n");
    printf("==============\n");
    loginPage();
    customerDashboard();
}
void managerPrompt(void) {
    // Under Construction
    void managerDashboard() {
        printf("Hello /Manager/\n");
        printf("Options:\n");
        printf("\n");
        printf("1. View Customer Details\n");
        // Update customer details
        // Function to search customer name;
        printf("2. Create New User Account\n");
        printf("3. Update Account Details\n");
        printf("4. Change Password\n");
        printf("5. Logout\n");
        printf("0. Exit\n");
        printf("\n");
    }
    printf("Manager login\n");
    printf("=============\n");
    loginPage();
    managerDashboard();
}
void adminPrompt(void) {
    // Under Construction
    void adminDashboard() {
        printf("Hello /Admin/\n");
        printf("Options:\n");
        printf("\n");
        printf("1. View Branch Details\n");
        printf("2. View Customer Details\n");
        printf("3. View Manager Details\n");
        printf("4. View Account Details\n");
        printf("5. Update Account Details\n");
        printf("6. View Transaction Log\n");
        printf("7. Logout\n");
        printf("0. Exit\n");
        printf("\n");
    }
    printf("Admin login\n");
    printf("===========\n");
    loginPage();
    adminDashboard();
}


int main(void) {
    // loadData(database);

    sortDatabase(database);
    
    // int count = 0;
    // while(count < 6) {
    //     printUserInfo(database[count++]);
    //     printf("\n");
    // }


    int accType = menu();
    // Prompts login based on account type selected in menu 

    switch(accType) {
        case 1: 
            customerPrompt();
            break;
        case 2: 
            managerPrompt();
            break;
        case 3: 
            adminPrompt();
            break;
    }
    
}
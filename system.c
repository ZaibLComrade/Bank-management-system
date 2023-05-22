#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define StrLen 32
#define PassLength 64
#define DBLen 16

typedef struct user {
    char firstName[StrLen];
    char lastName[StrLen];
    char username[StrLen];
    char password[StrLen];
    char email[StrLen];
    char phone[StrLen];
} User;

// Req -> Feature that is not yet added but will be worked
// on a future version


// Creates user account at login page
// Accessible in manager and admin login page
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


    //bug: Working with files profeciency required
    FILE *out = fopen("userData.txt", "w");
    fprintf(out, "First Name, Last Name, Username, Email, Password, Phone,");
    fprintf(out, "\n");
    fprintf(out, "%s, ", temp.firstName);
    fprintf(out, "%s, ", temp.lastName);
    fprintf(out, "%s, ", temp.username);
    fprintf(out, "%s, ", temp.email);
    fprintf(out, "%s, ", temp.password);
    fprintf(out, "%s, ", temp.phone);
    fclose(out);
}

// Shows login page after choosing user type
// Not added: Checks whether account exists
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
        printf("Enter Password: ");
        scanf("%s", password);
        printf("\n");

        // A search must be performed into database
    }
    // If account is not created
    
    // Asks whether account exists
    void login_or_create() {
        int exist;
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("Select: ");
        scanf("%d", &exist);
        exist -= 1; //Converting to boolean value
        printf("\n");

        if(exist) login();
        else createAccount();
    }

    login_or_create();
}

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
    printf("\n");
}
// Runs when account type chosen is customer at menu
void customerPrompt(void) {
    printf("Customer login\n");
    printf("==============\n");
    loginPage();
    customerDashboard();
}

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
    printf("\n");
}
// Runs when account type chosen is manager at menu
void managerPrompt(void) {
    printf("Manager login\n");
    printf("=============\n");
    loginPage();
    managerDashboard();
}

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
    printf("\n");
}
// Runs when account type chosen is admin at menu
void adminPrompt(void) {
    printf("Admin login\n");
    printf("===========\n");
    loginPage();
    adminDashboard();
}

// Starting up main menu
int menu(void) {
    printf("====Bank Management System====\n");
    printf("Select account type\n");
    printf("1. Customer\n");
    printf("2. Manager\n");
    printf("3. Admin\n");
    int accType; // Type of chosen account
    int invalid; // Checks whether user unput is valid
    do {
        // Prompts user for account type while input is not valid
        printf("Type: ");
        scanf("%d", &accType);
        invalid = (accType < 1 || accType > 3);
        if(invalid) printf("Not a valid type. please try again.\n");
    } while(invalid);
    printf("\n");
    return accType;
}


//Load Database
int getString(FILE *in, char str[]) {
    char ch;
    char delim = ',';
    int i = 0;
    str[i] = '\0';
    while(isspace(ch = getc(in)));
    if(ch == EOF) return 1;
    do {
        str[i++] = ch;
    } while((ch = getc(in)) != delim && ch != EOF);
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
    if(isEOF) strcpy(temp.firstName, "END");
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
    FILE *in = fopen("userData.txt", "r");
    int count = 0;
    readUser(in); // Reading Table heads as void

    User temp;
    do {
        temp = readUser(in);
        database[count++] = temp;
    } while(strcmp(temp.firstName, "END")); // END of data value
}

int main(void) {
    FILE *in = fopen("userData.txt", "r");
    User database[DBLen];
    loadData(database);

    int i = 0;
    while(strcmp(database[i].firstName, "END")) {
        printUserInfo(database[i++]);
        printf("\n");
    }

    // loadData(in);

    // int accType = menu();
    // Prompts login based on account type selected in menu 

    // switch(accType) {
    //     case 1: 
    //         customerPrompt();
    //         break;
    //     case 2: 
    //         managerPrompt();
    //         break;
    //     case 3: 
    //         adminPrompt();
    //         break;
    // }
    
}
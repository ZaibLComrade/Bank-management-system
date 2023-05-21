#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define StrLen 32
#define PassLength 64

typedef struct node {
    char *name;
    char *username;
    char *email;
    char *password;
    char *phone;
    // struct node *next;
} Node, *NodePtr;

// Req -> Feature that is not yet added but will be worked
// on a future version


// Creates user account at login page
// Accessible in manager and admin login page
void createAccount() {
    // Initializing varaibles
    char firstName[StrLen];
    char lastName[StrLen];
    char newUsername[StrLen];
    char newPassword[PassLength];
    char email[StrLen];
    char phoneNumber[StrLen];

    // Prompt user for details
    printf("First name: ");
    scanf("%s", firstName);
    printf("Last name: ");
    scanf("%s", lastName);
    printf("Choose username: ");
    scanf("%s", &newUsername);
    printf("Email Address: ");
    scanf("%s", email);

    // Promp for password while both entry doesn't match
    int notMatch;
    do {
        printf("New password: ");
        scanf("%s", newPassword);
        char tempPass[PassLength];
        printf("Repeat password: ");
        scanf("%s", tempPass);
        notMatch = strcmp(newPassword, tempPass);
        if(notMatch) printf("Passwords don't match. Please try again\n");
    } while(notMatch);
    printf("Enter phone number: ");
    scanf("%s", phoneNumber);


    // Forming full name
    char name[StrLen];
    strcpy(name, firstName);
    strcat(name, " ");
    strcat(name, lastName);


    //bug: Working with files profeciency required
    FILE *out = fopen("userData.txt", "w");
    fprintf(out, "Name, Username, Email, Password, Phone");
    fprintf(out, "\n");
    fprintf(out, "%s, ", name);
    fprintf(out, "%s, ", newUsername);
    fprintf(out, "%s, ", email);
    fprintf(out, "%s, ", newPassword);
    fprintf(out, "%s, ", phoneNumber);
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

// void loadData(FILE *in) {
//     NodePtr makeNode() {
//         NodePtr newNode = (NodePtr) malloc(sizeof(Node));
//     }
// }

int main(void) {
    // FILE *in = fopen("userData", "r");
    // loadData(in);

    int accType = menu();
    // Prompts login based on account type selected in menu 
    
    // Test: assume one user account exists



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
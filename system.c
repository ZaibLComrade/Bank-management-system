#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define StrLen 32
#define PassLength 64

// Req -> Feature that is not yet added but will be worked
// on a future version

void loginPage(int accType) {
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
    }
    // If account is not created
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
        strcat(name, firstName);
        strcat(name, " ");
        strcat(name, lastName);

        // 
    }
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
   
    void customerPrompt(void) {
        printf("Customer login\n");
        printf("==============\n");
        login_or_create();
    }
    void managerPrompt() {
        printf("Manager login\n");
        printf("=============\n");
        login_or_create();
    }
    void adminPrompt() {
        printf("Admin loginPage\n");
        printf("===========\n");
        login_or_create();
    }

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
    }
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

int main(void) {
    int accType = menu();
    loginPage(accType);
}
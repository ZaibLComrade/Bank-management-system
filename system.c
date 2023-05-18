#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NameLength 16
#define PassLength 64

void prompt() {
    char username[NameLength];
    char password[PassLength];
    printf("Enter Username:\n");
    scanf("%s", username);
    printf("Enter Password\n");
    scanf("%s", password);
}

void customerPrompt(void) {
    printf("Customer login:\n");
    prompt();
}

void managerPrompt() {
    printf("Manager login:\n");
    prompt();
}

void adminPrompt() {
    printf("Admin login:\n");
    prompt();
}

// Starting up main menu
void menu(void) {
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

int main(void) {
    menu();
}
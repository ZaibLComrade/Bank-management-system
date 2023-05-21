#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define StrLen 32
#define PassLength 64

typedef struct user {
    char *firstName;
    char *lastName;
    char *username;
    char *password;
    char *email;
    char *phone;
} User;

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
    scanf("%s", newUsername);
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

    FILE *out = fopen("userData.txt", "w");
    //bug: Working with files profeciency required
    fprintf(out, "First Name, Last Name, Username, Email, Password, Phone,");
    fprintf(out, "\n");
    fprintf(out, "%s, ", firstName);
    fprintf(out, "%s, ", lastName);
    fprintf(out, "%s, ", newUsername);
    fprintf(out, "%s, ", email);
    fprintf(out, "%s, ", newPassword);
    fprintf(out, "%s, ", phoneNumber);
}

void printUserInfo(User var) {
    char *name;
    strcpy(name, var.firstName);
    strcat(name, " ");
    strcat(name, var.lastName);

    printf("Name: %s\n", name);
    printf("Username: %s\n", var.username);
    printf("Email: %s\n", var.email);
    printf("Password: %s\n", var.password);
    printf("Phone: %s\n", var.phone);
}

void getString(FILE *in, char str[]) {
    char ch;
    char delim = ',';
    int i = 0;
    str[i] = '\0';
    while(isspace(ch = getc(in)));
    if(ch == EOF) return;
    do {
        str[i++] = ch;
    } while((ch = getc(in)) != delim && ch != EOF);
    str[i] = '\0';
}

User readUser(FILE *in) {
    User temp;
    getString(in, temp.firstName);
    getString(in, temp.lastName);
    getString(in, temp.username);
    getString(in, temp.email);
    getString(in, temp.password);
    getString(in, temp.phone);
    return temp;   
}

int main(void) {
    FILE *in = fopen("userData.txt", "r");
    // createAccount(out);

    printf("Hello world\n");
    User u1 = readUser(in);
    printUserInfo(u1);
}
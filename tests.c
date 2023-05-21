#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define StrLen 32
#define PassLength 64

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

    //bug: Working with files profeciency required
    FILE *out = fopen("userData.txt", "w");
    fprintf(out, "First Name, Last Name, Username, Email, Password, Phone,");
    fprintf(out, "\n");
    fprintf(out, "%s, ", firstName);
    fprintf(out, "%s, ", lastName);
    fprintf(out, "%s, ", newUsername);
    fprintf(out, "%s, ", email);
    fprintf(out, "%s, ", newPassword);
    fprintf(out, "%s, ", phoneNumber);
    fclose(out);
}

typedef struct user {
    char firstName[StrLen];
    char lastName[StrLen];
    char username[StrLen];
    char password[StrLen];
    char email[StrLen];
    char phone[StrLen];
} User;

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
    char tempStr[StrLen];
    getString(in, tempStr);
    strcpy(temp.firstName, tempStr);
    getString(in, tempStr);
    strcpy(temp.lastName, tempStr);
    getString(in, tempStr);
    strcpy(temp.username, tempStr);
    getString(in, tempStr);
    strcpy(temp.email, tempStr);
    getString(in, tempStr);
    strcpy(temp.password, tempStr);
    getString(in, tempStr);
    strcpy(temp.phone, tempStr);
    return temp;   
}

void printUserInfo(User var) {
    char name[StrLen];
    strcpy(name, var.firstName);
    strcat(name, " ");
    strcat(name, var.lastName);

    printf("Name: %s\n", name);
    printf("Username: %s\n", var.username);
    printf("Email: %s\n", var.email);
    printf("Password: %s\n", var.password);
    printf("Phone: %s\n", var.phone);
}

int main(void) {
    createAccount();
    FILE *in = fopen("userData.txt", "r");
    User uVoid = readUser(in);
    User u1 = readUser(in);
    printUserInfo(u1);

    fclose(in)
}
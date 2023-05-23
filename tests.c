#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MaxDat 10
#define StrLen 32

typedef enum {false, true} bool;
typedef struct user {
    char firstName[StrLen];
    char lastName[StrLen];
    char username[StrLen];
    char password[StrLen];
    char email[StrLen];
    char phone[StrLen];
} User;
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
    printf("\n");
}
void loadData(User database[]) {
    FILE *in = fopen("userData.csv", "r");
    int count = 0;
    readUser(in); // Reading Table heads as void

    User temp;
    do {
        temp = readUser(in);
        database[count++] = temp;
    } while(strcmp(temp.firstName, "END")); // END of data value
}
void sortByUsername(User database[], int len) {
    for(int i = 0; i < len; i++) {
        int k = i - 1;
        User key = database[i];
        // printUserInfo(key);
        while(k >= 0 && strcmp(key.username, database[k].username) < 0) {
            database[k + 1] = database[k];
            --k;
        }
        database[k + 1] = key;
    }
}

int main() {
    User database[MaxDat];
    loadData(database);
    int databaseCount = 0;
    int cmp = strcmp(database[databaseCount].firstName, "END");
    while(cmp) {
        cmp = strcmp(database[databaseCount].firstName, "END");
        if(cmp) databaseCount++;
    }
    sortByUsername(database, databaseCount);
    int count = 0;
    while(strcmp(database[count].firstName, "END")) {
        printUserInfo(database[count++]);
    }
    printf("Entry: %d\n\n", databaseCount);
    return 0;
}
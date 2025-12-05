#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[20];
char currentUser[50];

/* Function Declarations */
int loginSystem();
void mainMenu();
void adminMenu();
void userMenu();
void staffMenu();
void guestMenu();

void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

/* Added Feature Functions */
int isDuplicateRoll(int roll);
void sortStudentsByName();
void backupStudents();
void resetPassword();

/* =========== MAIN =========== */
int main() {
    if (loginSystem()) {
        mainMenu();
    } else {
        printf("\nAccess Denied. Exiting...\n");
    }
    return 0;
}

/* =========== LOGIN =========== */
int loginSystem() {
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[20];

    printf("===== LOGIN =====\n");
    printf("Username: ");
    scanf("%s", username);

    printf("Password: ");
    scanf("%s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error: credentials.txt not found!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            strcpy(currentRole, fileRole);
            strcpy(currentUser, fileUser);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/* =========== MAIN MENU =========== */
void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "USER") == 0)
        userMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else
        guestMenu();
}

/* =========== ADMIN MENU =========== */
void adminMenu() {
    int choice;

    do {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Sort Students by Name\n");
        printf("7. Backup Students File\n");
        printf("8. Reset User Password\n");
        printf("9. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: sortStudentsByName(); break;
            case 7: backupStudents(); break;
            case 8: resetPassword(); break;
            case 9: return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

/* =========== USER MENU =========== */
void userMenu() {
    int choice;

    do {
        printf("\n===== USER MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: return;
            default: printf("Invalid!\n");
        }
    } while (1);
}

/* =========== STAFF MENU =========== */
void staffMenu() {
    int choice;

    do {
        printf("\n===== STAFF MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: return;
            default: printf("Invalid!\n");
        }
    } while (1);
}

/* =========== GUEST MENU =========== */
void guestMenu() {
    int choice;

    do {
        printf("\n===== GUEST MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: return;
            default: printf("Invalid!\n");
        }
    } while (1);
}

/* =========== CRUD FUNCTIONS =========== */

void addStudent() {
    struct Student s;

    printf("Enter Roll: ");
    scanf("%d", &s.roll);

    if (isDuplicateRoll(s.roll)) {
        printf("Error: Roll number already exists!\n");
        return;
    }

    FILE *fp = fopen(STUDENT_FILE, "a");

    printf("Enter Name: ");
    scanf("%s", s.name);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);

    printf("Student Added Successfully!\n");
}

void displayStudents() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");

    if (!fp) {
        printf("No student file found!\n");
        return;
    }

    printf("\n--- Student List ---\n");
    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
}

void searchStudent() {
    int r, found = 0;
    struct Student s;

    printf("Enter Roll to Search: ");
    scanf("%d", &r);

    FILE *fp = fopen(STUDENT_FILE, "r");

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == r) {
            printf("FOUND: %d %s %.2f\n", s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found)
        printf("Student Not Found!\n");
}

void updateStudent() {
    int r, found = 0;
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    printf("Enter Roll to Update: ");
    scanf("%d", &r);

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == r) {
            found = 1;
            printf("New Name: ");
            scanf("%s", s.name);
            printf("New Marks: ");
            scanf("%f", &s.marks);
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Student Updated!\n");
    else
        printf("Student Not Found!\n");
}

void deleteStudent() {
    int r, found = 0;
    struct Student s;

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    printf("Enter Roll to Delete: ");
    scanf("%d", &r);

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll != r)
            fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Student Deleted!\n");
    else
        printf("Not Found!\n");
}

/* =========================================================
                    ADDED FEATURES
   ========================================================= */

/* ---- 1. Prevent Duplicate Roll Numbers ---- */
int isDuplicateRoll(int roll) {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");

    if (!fp) return 0;

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* ---- 2. Sort Students by Name ---- */
void sortStudentsByName() {
    struct Student list[500], temp;
    int count = 0;

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No file found!\n");
        return;
    }

    while (fscanf(fp, "%d %s %f", &list[count].roll, list[count].name, &list[count].marks) == 3) {
        count++;
    }
    fclose(fp);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(list[j].name, list[j+1].name) > 0) {
                temp = list[j];
                list[j] = list[j+1];
                list[j+1] = temp;
            }
        }
    }

    fp = fopen(STUDENT_FILE, "w");
    for (int i = 0; i < count; i++)
        fprintf(fp, "%d %s %.2f\n", list[i].roll, list[i].name, list[i].marks);
    fclose(fp);

    printf("Students sorted by name!\n");
}

/* ---- 3. Backup Students File ---- */
void backupStudents() {
    FILE *src = fopen(STUDENT_FILE, "r");
    FILE *bk = fopen("backup_students.txt", "w");

    if (!src) {
        printf("No student file to backup!\n");
        return;
    }

    char ch;
    while ((ch = fgetc(src)) != EOF)
        fputc(ch, bk);

    fclose(src);
    fclose(bk);

    printf("Backup created: backup_students.txt\n");
}

/* ---- 4. Reset Password ---- */
void resetPassword() {
    char user[50], newPass[50], fileUser[50], filePass[50], fileRole[20];

    printf("Enter username to reset password: ");
    scanf("%s", user);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    FILE *temp = fopen("temp_cred.txt", "w");

    int found = 0;

    while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(fileUser, user) == 0) {
            found = 1;
            printf("Enter new password: ");
            scanf("%s", newPass);
            fprintf(temp, "%s %s %s\n", fileUser, newPass, fileRole);
        } else {
            fprintf(temp, "%s %s %s\n", fileUser, filePass, fileRole);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(CREDENTIAL_FILE);
    rename("temp_cred.txt", CREDENTIAL_FILE);

    if (found)
        printf("Password Reset Successful!\n");
    else
        printf("Username not found!\n");
}
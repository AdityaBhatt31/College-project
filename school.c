#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUBJECTS 10
#define TEACHER_FILE "teachers.dat"
#define STUDENT_FILE "students.dat"

typedef struct Teacher {
    int id;
    char name[50];
    char gender[10];
    char dob[15];
    char email[50];
    char address[100];
    char contact[15];
    char dept[30];
    char qual[30];
    char password[20];
    float salary;
    struct Teacher *next;
} Teacher;

typedef struct Student {
    int id;
    int classRoll;
    char name[50];
    char father[50];
    char mother[50];
    char contact[15];
    char className[10];
    char email[50];
    char address[100];
    char dob[15];
    char password[20];
    int totalSubjects;
    char subjects[MAX_SUBJECTS][30];
    float marks[MAX_SUBJECTS];
    float totalMarks;
    float percentage;
    float totalFees;
    float paidAmount;
    float pendingAmount;
    struct Student *next;
} Student;

/* --- Prototypes --- */
static void readLine(char *buf, int size);
Teacher* loadTeachers(void);
void saveTeachers(Teacher* head);
Student* loadStudents(void);
void saveStudents(Student* head);
void freeTeachers(Teacher* head);
void freeStudents(Student* head);

Teacher* addTeacher(Teacher* head);
Student* addStudent(Student* head);
Teacher* findTeacher(Teacher* head, int id);
Student* findStudent(Student* head, int id);

void viewTeacher(Teacher* head);
void viewStudent(Student* head);
void viewSingleStudent(Student* s);
void viewClassStudents(Student* head);

void searchTeacher(Teacher* head);
void searchStudent(Student* head);
void updateTeacher(Teacher* head);
void updateStudent(Student* head);
void deleteTeacher(Teacher** head);
void deleteStudent(Student** head);

void calculateMarks(Student* s);
void calculateFees(Student* s);

void hodMenu(void);
void teacherMenu(int tid);
void studentMenu(int sid);

/* Helper to safely read a line (removes newline)*/
static void readLine(char *buf, int size) {
    if (fgets(buf, size, stdin) != NULL) {
        buf[strcspn(buf, "\n")] = '\0';
    } else {
        buf[0] = '\0';
    }
}

/* --- File / Persistence functions --- */
Teacher* loadTeachers(void) {
    Teacher *head = NULL, *last = NULL;
    FILE *fp = fopen(TEACHER_FILE, "rb");
    if (!fp) return NULL;
    while (1) {
        Teacher *t = (Teacher*)malloc(sizeof(Teacher));
        if (!t) break;
        if (fread(t, sizeof(Teacher), 1, fp) != 1) {
            free(t);
            break;
        }
        t->next = NULL;
        if (!head) head = t;
        else last->next = t;
        last = t;
    }
    fclose(fp);
    return head;
}

void saveTeachers(Teacher* head) {
    FILE *fp = fopen(TEACHER_FILE, "wb");
    if (!fp) {
        perror("Could not open teacher file for writing");
        return;
    }
    Teacher *cur = head;
    while (cur) {
        fwrite(cur, sizeof(Teacher), 1, fp);
        cur = cur->next;
    }
    fclose(fp);
}

Student* loadStudents(void) {
    Student *head = NULL, *last = NULL;
    FILE *fp = fopen(STUDENT_FILE, "rb");
    if (!fp) return NULL;
    while (1) {
        Student *s = (Student*)malloc(sizeof(Student));
        if (!s) break;
        if (fread(s, sizeof(Student), 1, fp) != 1) {
            free(s);
            break;
        }
        s->next = NULL;
        if (!head) head = s;
        else last->next = s;
        last = s;
    }
    fclose(fp);
    return head;
}

void saveStudents(Student* head) {
    FILE *fp = fopen(STUDENT_FILE, "wb");
    if (!fp) {
        perror("Could not open student file for writing");
        return;
    }
    Student *cur = head;
    while (cur) {
        fwrite(cur, sizeof(Student), 1, fp);
        cur = cur->next;
    }
    fclose(fp);
}

/* --- Memory cleanup --- */
void freeTeachers(Teacher* head) {
    Teacher *cur = head;
    while (cur) {
        Teacher *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

void freeStudents(Student* head) {
    Student *cur = head;
    while (cur) {
        Student *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

/* --- Utilities --- */
Teacher* findTeacher(Teacher* head, int id) {
    while (head) {
        if (head->id == id) return head;
        head = head->next;
    }
    return NULL;
}

Student* findStudent(Student* head, int id) {
    while (head) {
        if (head->id == id) return head;
        head = head->next;
    }
    return NULL;
}

/* --- Calculations --- */
void calculateMarks(Student* s) {
    s->totalMarks = 0.0f;
    if (s->totalSubjects <= 0) {
        s->percentage = 0.0f;
        return;
    }
    for (int i = 0; i < s->totalSubjects && i < MAX_SUBJECTS; ++i)
        s->totalMarks += s->marks[i];
    s->percentage = (s->totalMarks / (s->totalSubjects * 100.0f)) * 100.0f;
}

void calculateFees(Student* s) {
    s->pendingAmount = s->totalFees - s->paidAmount;
    s->totalFees =  s->pendingAmount;
}

/* --- CRUD: Add --- */
Teacher* addTeacher(Teacher* head) {
    Teacher *t = (Teacher*)malloc(sizeof(Teacher));
    if (!t) { perror("malloc"); return head; }
    t->next = NULL;

    printf("Enter Teacher ID: "); if (scanf("%d", &t->id) != 1) { printf("Invalid ID.\n"); while(getchar()!='\n'); free(t); return head; }
    getchar(); // consume newline
    printf("Name: "); readLine(t->name, sizeof(t->name));
    printf("Gender: "); readLine(t->gender, sizeof(t->gender));
    printf("DOB (DD/MM/YYYY): "); readLine(t->dob, sizeof(t->dob));
    printf("Email: "); readLine(t->email, sizeof(t->email));
    printf("Address: "); readLine(t->address, sizeof(t->address));
    printf("Contact: "); readLine(t->contact, sizeof(t->contact));
    printf("Department: "); readLine(t->dept, sizeof(t->dept));
    printf("Qualification: "); readLine(t->qual, sizeof(t->qual));
    printf("Salary: "); if (scanf("%f", &t->salary) != 1) { printf("Invalid salary.\n"); while(getchar()!='\n'); free(t); return head; }
    getchar();
    printf("Password: "); readLine(t->password, sizeof(t->password));

    if (!head) head = t;
    else {
        Teacher *cur = head;
        while (cur->next) cur = cur->next;
        cur->next = t;
    }
    saveTeachers(head);
    printf("Teacher added successfully.\n");
    return head;
}

Student* addStudent(Student* head) {
    Student *s = (Student*)malloc(sizeof(Student));
    if (!s) { perror("malloc"); return head; }
    s->next = NULL;

    printf("Enter Student ID: "); if (scanf("%d", &s->id) != 1) { printf("Invalid ID.\n"); while(getchar()!='\n'); free(s); return head; }
    getchar();
    printf("Name: "); readLine(s->name, sizeof(s->name));
    printf("Father Name: "); readLine(s->father, sizeof(s->father));
    printf("Mother Name: "); readLine(s->mother, sizeof(s->mother));
    printf("Contact: "); readLine(s->contact, sizeof(s->contact));
    printf("Class: "); readLine(s->className, sizeof(s->className));
    printf("Class Roll: "); if (scanf("%d", &s->classRoll) != 1) { printf("Invalid roll.\n"); while(getchar()!='\n'); free(s); return head; }
    getchar();
    printf("Email: "); readLine(s->email, sizeof(s->email));
    printf("Address: "); readLine(s->address, sizeof(s->address));
    printf("DOB (DD/MM/YYYY): "); readLine(s->dob, sizeof(s->dob));
    printf("Password: "); readLine(s->password, sizeof(s->password));

    printf("Total Subjects (max %d): ", MAX_SUBJECTS);
    if (scanf("%d", &s->totalSubjects) != 1) { printf("Invalid number.\n"); while(getchar()!='\n'); free(s); return head; }
    getchar();
    if (s->totalSubjects > MAX_SUBJECTS) s->totalSubjects = MAX_SUBJECTS;
    if (s->totalSubjects < 0) s->totalSubjects = 0;

    for (int i = 0; i < s->totalSubjects; ++i) {
        printf("Subject %d Name: ", i+1);
        readLine(s->subjects[i], sizeof(s->subjects[i]));
        printf("Marks (out of 100): ");
        if (scanf("%f", &s->marks[i]) != 1) s->marks[i] = 0.0f;
        if (s->marks[i] > 100.0f) s->marks[i] = 100.0f;
        if (s->marks[i] < 0.0f) s->marks[i] = 0.0f;
        getchar();
    }

    printf("Total Fees: "); if (scanf("%f", &s->totalFees) != 1) s->totalFees = 0.0f;
    printf("Paid Amount: "); if (scanf("%f", &s->paidAmount) != 1) s->paidAmount = 0.0f;
    getchar();

    calculateMarks(s);
    calculateFees(s);

    if (!head) head = s;
    else {
        Student *cur = head;
        while (cur->next) cur = cur->next;
        cur->next = s;
    }
    saveStudents(head);
    printf("Student added successfully.\n");
    return head;
}

/* --- View functions --- */
void viewTeacher(Teacher* head) {
    if (!head) {
        printf("No teacher records found.\n");
        return;
    }
    printf("\n%-5s %-20s %-10s %-15s %-10s %s\n", "ID", "Name", "Gender", "Department", "Salary", "Contact");
    printf("-------------------------------------------------------------------------------\n");
    while (head) {
        printf("%-5d %-20s %-10s %-15s %-10.2f %s\n", head->id, head->name, head->gender, head->dept, head->salary, head->contact);
        head = head->next;
    }
}

void viewStudent(Student* head) {
    if (!head) {
        printf("No student records found.\n");
        return;
    }
    printf("\n%-5s %-20s %-8s %-6s %-10s %-10s %s\n", "ID", "Name", "Class", "Roll", "TotalM", "Percent", "Contact");
    printf("---------------------------------------------------------------------------------\n");
    while (head) {
        printf("%-5d %-20s %-8s %-6d %-10.2f %-10.2f %s\n",
               head->id, head->name, head->className, head->classRoll,
               head->totalMarks, head->percentage, head->contact);
        head = head->next;
    }
}

void viewSingleStudent(Student* s) {
    if (!s) return;
    printf("\n--- Student Details (ID: %d) ---\n", s->id);
    printf("Name: %s\nClass: %s (Roll: %d)\nContact: %s | Email: %s\nDOB: %s\nAddress: %s\nFather: %s | Mother: %s\n",
           s->name, s->className, s->classRoll, s->contact, s->email, s->dob, s->address, s->father, s->mother);
    printf("\n--- Academic Record ---\n");
    for (int i = 0; i < s->totalSubjects; ++i)
        printf("%-20s %.2f/100\n", s->subjects[i], s->marks[i]);
    printf("Total Marks: %.2f\nPercentage: %.2f%%\n", s->totalMarks, s->percentage);
    printf("\n--- Financial Record ---\nTotal Fees: %.2f\nPaid: %.2f\nPending: %.2f\n",
           s->totalFees, s->paidAmount, s->pendingAmount);
}

void viewClassStudents(Student* head) {
    char cls[16];
    printf("Enter Class Name to view students: ");
    readLine(cls, sizeof(cls));
    int found = 0;
    printf("\n--- Students in Class %s ---\n", cls);
    printf("%-5s %-20s %-6s %-12s %s\n", "ID", "Name", "Roll", "Total Marks", "Pending Fees");
    printf("-----------------------------------------------------------------\n");
    while (head) {
        if (strcmp(head->className, cls) == 0) {
            printf("%-5d %-20s %-6d %-12.2f %.2f\n",
                   head->id, head->name, head->classRoll, head->totalMarks, head->pendingAmount);
            found = 1;
        }
        head = head->next;
    }
    if (!found) printf("No students found in class %s.\n", cls);
}

/* --- Search, Update, Delete --- */
void searchTeacher(Teacher* head) {
    int id;
    printf("Enter Teacher ID to search: ");
    if (scanf("%d", &id) != 1) { printf("Invalid input.\n"); while(getchar()!='\n'); return; }
    getchar();
    Teacher *t = findTeacher(head, id);
    if (t) {
        printf("\nFound: ID: %d\nName: %s\nDept: %s\nSalary: %.2f\nContact: %s\n",
               t->id, t->name, t->dept, t->salary, t->contact);
    } else {
        printf("Teacher ID %d not found.\n", id);
    }
}

void searchStudent(Student* head) {
    int id;
    printf("Enter Student ID to search: ");
    if (scanf("%d", &id) != 1) { printf("Invalid input.\n"); while(getchar()!='\n'); return; }
    getchar();
    Student *s = findStudent(head, id);
    if (s) viewSingleStudent(s);
    else printf("Student ID %d not found.\n", id);
}

void updateTeacher(Teacher* head) {
    int id;
    printf("Enter Teacher ID to update: ");
    if (scanf("%d", &id) != 1) { printf("Invalid input.\n"); while(getchar()!='\n'); return; }
    getchar();
    Teacher *t = findTeacher(head, id);
    if (!t) { printf("Teacher not found.\n"); return; }
    printf("Updating Teacher: %s (ID %d)\n", t->name, t->id);
    printf("1. Change Salary\n2. Change Password\n3. Change Department\nChoose: ");
    int ch; if (scanf("%d", &ch) != 1) { printf("Invalid.\n"); while(getchar()!='\n'); return; }
    getchar();
    if (ch == 1) {
        printf("New Salary: "); if (scanf("%f", &t->salary) != 1) printf("Invalid salary.\n");
        getchar();
    } else if (ch == 2) {
        printf("New Password: "); readLine(t->password, sizeof(t->password));
    } else if (ch == 3) {
        printf("New Department: "); readLine(t->dept, sizeof(t->dept));
    } else printf("Invalid choice.\n");
    saveTeachers(head);
    printf("Teacher updated and saved.\n");
}

void updateStudent(Student* head) {
    int id;
    printf("Enter Student ID to update: ");
    if (scanf("%d", &id) != 1) { printf("Invalid input.\n"); while(getchar()!='\n'); return; }
    getchar();
    Student *s = findStudent(head, id);
    if (!s) { printf("Student not found.\n"); return; }
    printf("Updating Student: %s (ID %d)\n", s->name, s->id);
    printf("1. Update Marks\n2. Update Fees Paid\nChoose: ");
    int ch; if (scanf("%d", &ch) != 1) { printf("Invalid.\n"); while(getchar()!='\n'); return; }
    getchar();
    if (ch == 1) {
        for (int i = 0; i < s->totalSubjects; ++i) {
            printf("Current marks for %s: %.2f. New marks: ", s->subjects[i], s->marks[i]);
            if (scanf("%f", &s->marks[i]) != 1) s->marks[i] = 0.0f;
            if (s->marks[i] > 100.0f) s->marks[i] = 100.0f;
            if (s->marks[i] < 0.0f) s->marks[i] = 0.0f;
            getchar();
        }
        calculateMarks(s);
    } else if (ch == 2) {
        printf("Current Paid: %.2f. New Paid Amount: ", s->paidAmount);
        if (scanf("%f", &s->paidAmount) != 1) s->paidAmount = s->paidAmount;
        getchar();
        calculateFees(s);
    } else {
        printf("Invalid choice.\n");
    }
    saveStudents(head);
    printf("Student updated and saved.\n");
}

void deleteTeacher(Teacher** head) {
    if (!head || !*head) { printf("No teacher records.\n"); return; }
    int id;
    printf("Enter Teacher ID to delete: ");
    if (scanf("%d", &id) != 1) { printf("Invalid.\n"); while(getchar()!='\n'); return; }
    getchar();
    Teacher *cur = *head, *prev = NULL;
    while (cur && cur->id != id) { prev = cur; cur = cur->next; }
    if (!cur) { printf("Teacher not found.\n"); return; }
    if (!prev) *head = cur->next;
    else prev->next = cur->next;
    free(cur);
    saveTeachers(*head);
    printf("Teacher deleted and changes saved.\n");
}

void deleteStudent(Student** head) {
    if (!head || !*head) { printf("No student records.\n"); return; }
    int id;
    printf("Enter Student ID to delete: ");
    if (scanf("%d", &id) != 1) { printf("Invalid.\n"); while(getchar()!='\n'); return; }
    getchar();
    Student *cur = *head, *prev = NULL;
    while (cur && cur->id != id) { prev = cur; cur = cur->next; }
    if (!cur) { printf("Student not found.\n"); return; }
    if (!prev) *head = cur->next;
    else prev->next = cur->next;
    free(cur);
    saveStudents(*head);
    printf("Student deleted and changes saved.\n");
}

/* --- Menus --- */
void hodMenu(void) {
    int choice;
    Teacher *tHead = loadTeachers();
    Student *sHead = loadStudents();

    while (1) {
        printf("\n--- HOD MENU ---\n");
        printf("1. Add Teacher\n2. Add Student\n3. Update Teacher\n4. Update Student\n5. Delete Teacher\n6. Delete Student\n7. View Teachers\n8. View Students\n9. Search Teacher\n10. Search Student\n11. View Class Students\n12. Exit to Main Menu\nChoose: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();
        switch (choice) {
            case 1: tHead = addTeacher(tHead); break;
            case 2: sHead = addStudent(sHead); break;
            case 3: updateTeacher(tHead); break;
            case 4: updateStudent(sHead); break;
            case 5: deleteTeacher(&tHead); break;
            case 6: deleteStudent(&sHead); break;
            case 7: viewTeacher(tHead); break;
            case 8: viewStudent(sHead); break;
            case 9: searchTeacher(tHead); break;
            case 10: searchStudent(sHead); break;
            case 11: viewClassStudents(sHead); break;
            case 12:
                saveTeachers(tHead);
                saveStudents(sHead);
                freeTeachers(tHead);
                freeStudents(sHead);
                return;
            default: printf("Invalid choice.\n");
        }
    }
}

void teacherMenu(int tid) {
    Teacher *tHead = loadTeachers();
    Student *sHead = loadStudents();
    Teacher *t = findTeacher(tHead, tid);
    if (!t) { printf("Teacher not found (ID %d).\n", tid); freeTeachers(tHead); freeStudents(sHead); return; }

    int choice;
    while (1) {
        printf("\n--- TEACHER MENU (%s) ---\n", t->name);
        printf("1. View All Students\n2. Search Student\n3. Update Student Marks\n4. Logout\nChoose: ");
        if (scanf("%d", &choice) != 1) { printf("Invalid.\n"); while(getchar()!='\n'); continue; }
        getchar();
        if (choice == 1) viewStudent(sHead);
        else if (choice == 2) searchStudent(sHead);
        else if (choice == 3) {
            int sid;
            printf("Enter Student ID to update marks: ");
            if (scanf("%d", &sid) != 1) { printf("Invalid.\n"); while(getchar()!='\n'); continue; }
            getchar();
            Student *s = findStudent(sHead, sid);
            if (!s) { printf("Student not found.\n"); continue; }
            for (int i = 0; i < s->totalSubjects; ++i) {
                printf("Enter marks for %s: ", s->subjects[i]);
                if (scanf("%f", &s->marks[i]) != 1) s->marks[i] = 0.0f;
                if (s->marks[i] > 100.0f) s->marks[i] = 100.0f;
                if (s->marks[i] < 0.0f) s->marks[i] = 0.0f;
                getchar();
            }
            calculateMarks(s);
            saveStudents(sHead);
            printf("Marks updated and saved.\n");
        }
        else if (choice == 4) break;
        else printf("Invalid choice.\n");
    }
    freeTeachers(tHead);
    freeStudents(sHead);
}

void studentMenu(int sid) {
    Student *sHead = loadStudents();
    Student *s = findStudent(sHead, sid);
    if (!s) { printf("Student not found (ID %d).\n", sid); freeStudents(sHead); return; }

    printf("\n--- STUDENT DASHBOARD ---\n");
    viewSingleStudent(s);

    freeStudents(sHead);
}

/* --- main --- */
int main(void) {
    while (1) {
        printf("\n=== SCHOOL MANAGEMENT SYSTEM ===\n");
        printf("Login as:\n1. HOD\n2. Teacher\n3. Student\n4. Exit\nChoose: ");
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();
        if (choice == 1) {
            char user[32], pass[32];
            printf("Username: "); readLine(user, sizeof(user));
            printf("Password: "); readLine(pass, sizeof(pass));
            if (strcmp(user, "hod") == 0 && strcmp(pass, "hod123") == 0) {
                hodMenu();
            } else {
                printf("Invalid HOD credentials.\n");
            }
        } else if (choice == 2) {
            int tid;
            char pass[32];
            printf("Teacher ID: ");
            if (scanf("%d", &tid) != 1) { printf("Invalid ID.\n"); while(getchar()!='\n'); continue; }
            getchar();
            printf("Password: "); readLine(pass, sizeof(pass));
            Teacher *tHead = loadTeachers();
            Teacher *t = findTeacher(tHead, tid);
            if (t && strcmp(t->password, pass) == 0) {
                freeTeachers(tHead);
                teacherMenu(tid);
            } else {
                printf("Invalid teacher credentials.\n");
                freeTeachers(tHead);
            }
        } else if (choice == 3) {
            int sid;
            char pass[32];
            printf("Student ID: ");
            if (scanf("%d", &sid) != 1) { printf("Invalid ID.\n"); while(getchar()!='\n'); continue; }
            getchar();
            printf("Password: "); readLine(pass, sizeof(pass));
            Student *sHead = loadStudents();
            Student *s = findStudent(sHead, sid);
            if (s && strcmp(s->password, pass) == 0) {
                freeStudents(sHead);
                studentMenu(sid);
            } else {
                printf("Invalid student credentials.\n");
                freeStudents(sHead);
            }
        } else if (choice == 4) {
            printf("Exiting.\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
    return 0;
}

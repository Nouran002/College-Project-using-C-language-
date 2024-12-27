#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Subject {
    char subjectName[30];
    int subjectGrade;
};
typedef struct Subject Subject;

struct Student {
    int id;
    char name[50];
    struct Subject *subjects;
    float average;
    char grade;
    int subjectCount;
    int numSubjects;
};
typedef struct Student Student;


void addStudent(Student **students, int *size, int stuId, char stuName[], Subject subjects[], int subjectCount) {
    Student *temp = realloc(*students, (*size + 1) * sizeof(Student));
    if (temp == NULL) {
        printf(" Memory allocation failed\n");
        return;
    }
    *students = temp;

    if (strlen(stuName) == 0) {
        printf("Name cannot be empty\n");
        return;
    }
    if (stuId <= 0) {
        printf("Invalid student ID\n");
        return;
    }

    strcpy((*students)[*size].name, stuName);
    (*students)[*size].id = stuId;
    (*students)[*size].subjectCount = subjectCount;
    (*students)[*size].subjects = (Subject *)malloc(subjectCount * sizeof(Subject));

    for (int i = 0; i < subjectCount; i++) {
        if (strlen(subjects[i].subjectName) == 0) {
            printf("Subject name cannot be empty\n");
            return;
        }
        if (subjects[i].subjectGrade < 0 || subjects[i].subjectGrade > 100) {
            printf("Invalid grade for subject %s\n", subjects[i].subjectName);
            return;
        }
        strcpy((*students)[*size].subjects[i].subjectName, subjects[i].subjectName);
        (*students)[*size].subjects[i].subjectGrade = subjects[i].subjectGrade;
    }

    (*size)++;
    printf("Student Added successfully\n");
}


int findStudentIndex(Student *students, int size, int id) {
    for (int i = 0; i < size; i++) {
        if (students[i].id == id) {
            return i;
        }
    }
    return -1;
}

void calculateStudentAverage(Student *student) {
    if (student->subjectCount <= 0) {
        student->average = 0;
        student->grade = 'F';
        return;
    }

    int totalStuGrade = 0;
    for (int i = 0; i < student->subjectCount; i++) {
        if (student->subjects[i].subjectGrade < 0 || student->subjects[i].subjectGrade > 100) {
            printf("Invalid grade %d for subject %s\n", student->subjects[i].subjectGrade, student->subjects[i].subjectName);
            student->average = 0;
            student->grade = 'F';
            return;
        }
        totalStuGrade += student->subjects[i].subjectGrade;
    }

    student->average = (float)totalStuGrade / student->subjectCount;

    if (student->average >= 90) {
        student->grade = 'A';
    } else if (student->average >= 80) {
        student->grade = 'B';
    } else if (student->average >= 70) {
        student->grade = 'C';
    } else if (student->average >= 60) {
        student->grade = 'D';
    } else {
        student->grade = 'F';
    }
}

void displaySpecificStudent(Student *students, int size, int searchStuId) {
    int index = -1;
    for (int i = 0; i < size; i++) {
        if (students[i].id == searchStuId) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Student with ID %d not found\n", searchStuId);
        return;
    }

    calculateStudentAverage(&students[index]);
    printf("\nStudent: %s\n", students[index].name);
    printf("ID: %d\n", students[index].id);
    printf("Average: %.2f\n", students[index].average);
    printf("Grade: %c\n", students[index].grade);

    printf("Subjects:\n");
    for (int i = 0; i < students[index].subjectCount; i++) {
        printf("  - %s: %d\n", students[index].subjects[i].subjectName, students[index].subjects[i].subjectGrade);
    }
}


void displayAllStudents(Student *students, int size) {
    if (size == 0) {
        printf("No students to display\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        calculateStudentAverage(&students[i]);
        printf("\nStudent: %s\n", students[i].name);
        printf("ID: %d\n", students[i].id);
        printf("Average: %.2f\n", students[i].average);
        printf("Grade: %c\n", students[i].grade);

        printf("Subjects:\n");
        for (int j = 0; j < students[i].subjectCount; j++) {
            printf("  - %s: %d\n", students[i].subjects[j].subjectName, students[i].subjects[j].subjectGrade);
        }
    }
}


void updateStudent(Student *students, int *studentCount, int id, const char *newName, Subject *newSubjects, int numSubjects) {
    int index = findStudentIndex(students, *studentCount, id);
    if (index == -1) {
        printf("Student with ID %d not found\n", id);
        return;
    }
    strcpy(students[index].name, newName);

    if (students[index].subjects == NULL) {
        students[index].subjects = (Subject *)malloc(numSubjects * sizeof(Subject));
    } else {
        students[index].subjects = (Subject *)realloc(students[index].subjects, numSubjects * sizeof(Subject));
    }

    if (students[index].subjects == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    students[index].numSubjects = numSubjects;
    for (int i = 0; i < numSubjects; i++) {
        strcpy(students[index].subjects[i].subjectName, newSubjects[i].subjectName);
        students[index].subjects[i].subjectGrade = newSubjects[i].subjectGrade;
    }

    calculateStudentAverage(&students[index]);

    printf("Student data updated successfully\n");
}



void classifyStudents(Student *students, int size) {
    printf("\nPassing Students with Grade D or higher:\n");
    for (int i = 0; i < size; i++) {
        if (students[i].grade < 'A' || (students[i].grade > 'D' && students[i].grade != 'F')) {
            printf("Invalid grade '%c' for student %s (ID: %d). Please recheck the calculations\n",
                   students[i].grade, students[i].name, students[i].id);
            continue;
        }

        if (students[i].grade == 'A' || students[i].grade == 'B' ||
            students[i].grade == 'C' || students[i].grade == 'D') {
            printf("Student: %s, ID: %d, Grade: %c\n", students[i].name, students[i].id, students[i].grade);
        }
    }

    printf("\nFailing Students (Grade F):\n");
    for (int i = 0; i < size; i++) {
        if (students[i].grade == 'F') {
            printf("Student: %s, ID: %d, Grade: %c\n", students[i].name, students[i].id, students[i].grade);
        }
    }
}


void displayAverages(Student *students, int size) {
    if (size == 0) {
        printf("No students available\n");
        return;
    }

    printf("Averages of all students:\n");
    for (int i = 0; i < size; i++) {
        printf("Student: %s, ID: %d, Average: %.2f\n", students[i].name, students[i].id, students[i].average);
    }
}


void highestAvg(Student *students, int size) {
    if (size == 0) {
        printf("No students available\n");
        return;
    }

    int maxIndex = 0;

    for (int i = 1; i < size; i++) {
        if (students[i].average > students[maxIndex].average) {
            maxIndex = i;
        }
    }

    printf("Student with Max Average: %s\n", students[maxIndex].name);
    printf("ID: %d\n", students[maxIndex].id);
    printf("Max Average: %.2f\n", students[maxIndex].average);
}


void deleteStudent(Student **students, int *size, int DelStuId) {
    int index = findStudentIndex(*students, *size, DelStuId);

    if (index == -1) {
        printf("Student with ID %d not found\n", DelStuId);
        return;
    }

    free((*students)[index].subjects);

    for (int i = index; i < *size - 1; i++) {
        (*students)[i] = (*students)[i + 1];
    }

    (*size)--;

    Student *temp = realloc(*students, (*size) * sizeof(Student));
    if (temp == NULL && *size > 0) {
        printf("Memory reallocation failed\n");
        return;
    }
    *students = temp;

    printf("Student with ID %d has been deleted\n", DelStuId);
}


int main() {
    int studentCount = 0;
    Student *students = NULL;
    int choice;

    while (1) {
        printf("\nSelect an option:\n");
        printf("1. Add Student\n");
        printf("2. Display Student\n");
        printf("3. Display All Students\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Find Student with Highest Average\n");
        printf("7. Display Averages of All Students\n");
        printf("8. Classify Students (Passing/Failing)\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        int id, numSubjects;
        char name[50];

        if (choice == 9) {
            break;
        } else if (choice == 1) {
            int numStudents;
            printf("Enter the number of students to add: ");
            scanf("%d", &numStudents);
            getchar();

            for (int i = 0; i < numStudents; i++) {
                printf("\nEnter details for student %d:\n", i + 1);

                printf("Enter ID: ");
                scanf("%d", &id);
                getchar();

                printf("Enter name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';

                printf("Enter the number of subjects: ");
                scanf("%d", &numSubjects);
                getchar();

                Subject *subjects = (Subject *)malloc(numSubjects * sizeof(Subject));
                for (int j = 0; j < numSubjects; j++) {
                    printf("Enter subject %d name: ", j + 1);
                    fgets(subjects[j].subjectName, sizeof(subjects[j].subjectName), stdin);
                    subjects[j].subjectName[strcspn(subjects[j].subjectName, "\n")] = '\0';

                    printf("Enter subject %d grade: ", j + 1);
                    scanf("%d", &subjects[j].subjectGrade);
                    getchar();
                }

                addStudent(&students, &studentCount, id, name, subjects, numSubjects);
                free(subjects);
            }
        } else if (choice == 2) {
            printf("Enter Student ID to Display: ");
            scanf("%d", &id);
            displaySpecificStudent(students, studentCount, id);
        } else if (choice == 3) {
            displayAllStudents(students, studentCount);
        } else if (choice == 4) {
            printf("Enter Student ID to Update: ");
    scanf("%d", &id);
    getchar();

    int index = findStudentIndex(students, studentCount, id);
    if (index == -1) {
        printf("Student with ID %d not found\n", id);
        continue;
    }

    printf("Enter New Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter New Number of Subjects: ");
    scanf("%d", &numSubjects);
    getchar();

    Subject *subjects = (Subject *)malloc(numSubjects * sizeof(Subject));
    if (subjects == NULL) {
        printf("Memory allocation failed.\n");
        continue;
    }

    for (int j = 0; j < numSubjects; j++) {
        printf("Enter subject %d name: ", j + 1);
        fgets(subjects[j].subjectName, sizeof(subjects[j].subjectName), stdin);
        subjects[j].subjectName[strcspn(subjects[j].subjectName, "\n")] = '\0';

        printf("Enter subject %d grade: ", j + 1);
        scanf("%d", &subjects[j].subjectGrade);
        getchar();
    }
    updateStudent(students, &studentCount, id, name, subjects, numSubjects);

    free(subjects);
        } else if (choice == 5) {
            printf("Enter Student ID to Delete: ");
            scanf("%d", &id);
            deleteStudent(&students, &studentCount, id);
        } else if (choice == 6) {
            highestAvg(students, studentCount);
        } else if (choice == 7) {
            displayAverages(students, studentCount);
        } else if (choice == 8) {
            classifyStudents(students, studentCount);
        } else {
            printf("Invalid choice. Please try again\n");
        }
    }

    for (int i = 0; i < studentCount; i++) {
        free(students[i].subjects);
    }
    free(students);

    return 0;
}

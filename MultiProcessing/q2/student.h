#ifndef MULTIPROCESSING_STUDENT_H
#define MULTIPROCESSING_STUDENT_H

#include "company.h"
#include "functions.h"

#define STUDENT_STATE_GATE -3
#define STUDENT_STATE_COLLEGE -2
#define STUDENT_STATE_HOME -1

pthread_mutex_t* students_mutex;

typedef struct Student {
    int id;
    int state;
    int tries;
    pthread_mutex_t* mutex;
} Student;

int n_students;
int n_students_arrived, n_students_vaccinated, n_students_queued;
Student *all_students;

void student_init(int n);
void student_make(Student *student);
void* student_process(void *student);
void student_test(Company *company, Student* student);

#endif //MULTIPROCESSING_STUDENT_H

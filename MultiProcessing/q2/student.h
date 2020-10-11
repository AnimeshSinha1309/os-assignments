#ifndef MULTIPROCESSING_STUDENT_H
#define MULTIPROCESSING_STUDENT_H

#include "company.h"
#include "functions.h"

#define STUDENT_STATE_GATE 0
#define STUDENT_STATE_COLLEGE -2
#define STUDENT_STATE_HOME -1

typedef struct Student {
    int id;
    int state;
    pthread_mutex_t* mutex;
} Student;

int n_students;
int n_students_arrived, n_students_vaccinated;
Student *all_students;

void student_init(int n);
void student_make(Student *student);
void* student_process(void *student);
bool student_test(Company *company);

#endif //MULTIPROCESSING_STUDENT_H

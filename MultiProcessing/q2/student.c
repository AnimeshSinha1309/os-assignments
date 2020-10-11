#include <pthread.h>
#include "student.h"
#include "functions.h"
#include "zone.h"

void student_init(int n) {
    n_students = n;
    n_students_arrived = 0;
    n_students_vaccinated = 0;
    all_students = (Student*) calloc(n_students, sizeof(Student));
    for (int i = 0; i < n_students; i++) {
        all_students[i].id = i;
        student_make(all_students + i);
    }
}

void student_make(Student *student) {
    student->mutex = (pthread_mutex_t *)share_memory(sizeof(pthread_mutex_t));
    pthread_mutex_init(student->mutex, NULL);
    student->state = STUDENT_STATE_GATE;
}

void* student_process(void* input) {
    Student *student = (Student *) input;
    if (student->state == STUDENT_STATE_GATE) {
        title_print(CLASS_STUDENT, student->id, "left their house");
        int time_delay = randint(L_ARRIVAL_TIME, R_ARRIVAL_TIME);
        char* time_string = calloc(50, sizeof(char));
        sprintf(time_string, "arrived at gate after %d seconds", time_delay);
        delay(time_delay);
        title_print(CLASS_STUDENT, student->id, time_string);
        n_students_arrived++;
        while (!assign_slot(student));
    } else if (student->state > 0) {
        student->state = (randint(0, 2) == 0)
                ? STUDENT_STATE_COLLEGE
                : STUDENT_STATE_HOME;
    } else if (student->state == STUDENT_STATE_COLLEGE || student->state == STUDENT_STATE_HOME) {
    }
    return NULL;
}

bool student_test(Company *company) {
    bool result = (random() <= (int)(RAND_MAX * company->success));
    if (result == FALSE) {

    }
}

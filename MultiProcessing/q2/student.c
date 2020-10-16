#include <pthread.h>
#include "student.h"
#include "functions.h"
#include "zone.h"

void student_init(int n) {
    n_students = n;
    n_students_arrived = 0;
    n_students_vaccinated = 0;
    n_students_queued = 0;
    all_students = (Student*) calloc(n_students, sizeof(Student));
    for (int i = 0; i < n_students; i++) {
        all_students[i].id = i;
        student_make(all_students + i);
    }
    students_mutex = (pthread_mutex_t *)share_memory(sizeof(pthread_mutex_t));
    pthread_mutex_init(students_mutex, NULL);
}

void student_make(Student *student) {
    student->mutex = (pthread_mutex_t *)share_memory(sizeof(pthread_mutex_t));
    pthread_mutex_init(student->mutex, NULL);
    student->state = STUDENT_STATE_GATE;
    student->tries = 0;
}

void* student_process(void* input) {
    // Start from the house
    Student *student = (Student *) input;
    if (student->state != STUDENT_STATE_GATE) perror("At the process start he is not at the gate");
    title_print(CLASS_STUDENT, student->id, "left their house");
    // Arrive at the gate with some delay
    int time_delay = randint(L_ARRIVAL_TIME, R_ARRIVAL_TIME);
    char* time_string = calloc(50, sizeof(char));
    sprintf(time_string, "arrived at gate after %d seconds", time_delay);
    delay(time_delay);
    title_print(CLASS_STUDENT, student->id, time_string);
    n_students_arrived++;
    n_students_queued++;
    // Search for a slot
    while (TRUE) {
        if (student->state == STUDENT_STATE_HOME || student->state == STUDENT_STATE_COLLEGE)
            break;
        if (student->state == STUDENT_STATE_GATE)
            assign_slot(student);
    }
    return NULL;
}

void student_test(Company *company, Student* student) {
    pthread_mutex_lock(student->mutex);
    bool result = ((double)random() / (double)RAND_MAX) <= company->success;
    if (result == FALSE) {
        student->tries++;
        char* message = (char*) calloc(50, sizeof(char));
        sprintf(message, "failed vaccination attempt #%d", student->tries);
        title_print(CLASS_STUDENT, student->id, message);
        if (student->tries == 3) {
            title_print(CLASS_STUDENT, student->id, "got sent home");
            n_students_vaccinated++;
            student->state = STUDENT_STATE_HOME;
        } else {
            student->state = STUDENT_STATE_GATE;
            n_students_queued++;
        }
    } else {
        student->state = STUDENT_STATE_COLLEGE;
        n_students_vaccinated++;
        title_print(CLASS_STUDENT, student->id, "got into the college");
    }
    pthread_mutex_unlock(student->mutex);
}

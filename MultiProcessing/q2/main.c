#include <stdio.h>

#include "company.h"
#include "student.h"
#include "zone.h"
#include "functions.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
int main() {
    // Take the inputs and initialize classes
    int n_company, n_zone, n_student;
    printf("Enter robot count, student count, and table count: ");
    fflush(stdout);
    scanf("%d %d %d", &n_company, &n_zone, &n_student);
    company_init(n_company);
    zone_init(n_zone);
    student_init(n_student);
    // Branch off and start all processes
    pthread_t *student_threads = (pthread_t *) share_memory(sizeof(pthread_t) * n_student);
    for (int i = 0; i < n_student; i++)
        pthread_create(&student_threads[i], NULL, student_process, all_students + i);
    pthread_t *company_threads = (pthread_t *) share_memory(sizeof(pthread_t) * n_student);
    for (int i = 0; i < n_company; i++)
        pthread_create(&student_threads[i], NULL, company_process, all_companies + i);
    pthread_t *zone_threads = (pthread_t *) share_memory(sizeof(pthread_t) * n_student);
    for (int i = 0; i < n_company; i++)
        pthread_create(&zone_threads[i], NULL, zone_process, all_zones + i);
    // Wrap the starter process up
    for (int i = 0; i < n_student; i++) {
        pthread_join(student_threads[i], NULL);
    }
    for (int i = 0; i < n_company; i++) {
        pthread_join(company_threads[i], NULL);
    }
    for (int i = 0; i < n_company; i++) {
        pthread_join(zone_threads[i], NULL);
    }
    return 0;
}
#pragma clang diagnostic pop
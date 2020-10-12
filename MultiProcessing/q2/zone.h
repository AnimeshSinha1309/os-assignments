#ifndef MULTIPROCESSING_ZONE_H
#define MULTIPROCESSING_ZONE_H

#include <pthread.h>
#include "functions.h"
#include "company.h"
#include "student.h"

// Zones will be passive players to the actively polling companies and students
typedef struct Zone {
    int id;
    int num_slots, used_slots;
    Company* company_of_purchase;
    pthread_mutex_t *mutex;
    Student** waiting_list;
} Zone;

int n_zones;
Zone* all_zones;

void zone_init(int n); // Class Initializer
void zone_make(Zone* zone); // Object Constructor
bool assign_batch(Company* company); // Gets a zone to send a batch to
bool assign_slot(Student* student); // Gets a zone with a free slot
void* zone_process(void *input);

#endif //MULTIPROCESSING_ZONE_H

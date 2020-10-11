#include <stddef.h>
#include <pthread.h>
#include "zone.h"


void zone_init(int n) {
    n_zones = n;
    all_zones = (Zone*) calloc(n_zones, sizeof(Zone));
    for (int i = 0; i < n_zones; i++) {
        all_zones[i].id = i;
        zone_make(all_zones + i);
    }
}

void zone_make(Zone* zone) {
    zone->mutex = (pthread_mutex_t*) calloc(1, sizeof(pthread_mutex_t));
    pthread_mutex_init(zone->mutex, NULL);
}

bool assign_batch(Company* company) {
    for (int i = 0; i < n_zones; i++) {
        pthread_mutex_lock(all_zones[i].mutex);
        if (all_zones[i].num_slots == all_zones[i].used_slots) {
            // Get the number of drugs and print it
            char* message = calloc(50, sizeof(char));
            all_zones[i].num_slots = min_3(8,
                    n_students_arrived - n_students_vaccinated,
                    company->doses_in_batch[company->num_batches - 1]);
            all_zones[i].used_slots = 0;
            if (all_zones[i].num_slots == 0) continue;
            sprintf(message, "company %d supplied %d", company->id + 1, all_zones[i].num_slots);
            // Update the details for the company
            all_zones[i].company_of_purchase = company;
            company->num_batches--;
            title_print(CLASS_ZONE, i, message);
            pthread_mutex_unlock(all_zones[i].mutex);
            return TRUE;
        }
        pthread_mutex_unlock(all_zones[i].mutex);
    }
    return FALSE;
}

bool assign_slot(Student* student) {
    for (int i = 0; i < n_zones; i++) {
        pthread_mutex_lock(all_zones[i].mutex);
        if (all_zones[i].num_slots > all_zones[i].used_slots) {
            all_zones[i].used_slots++;
            student->state = i;
            char* message = calloc(50, sizeof(char));
            sprintf(message, "accepted student %d ", student->id + 1);
            title_print(CLASS_ZONE, all_zones[i].id, message);
            pthread_mutex_unlock(all_zones[i].mutex);
            return TRUE;
        } else {
            all_zones[i].company_of_purchase = NULL;
        }
        pthread_mutex_unlock(all_zones[i].mutex);
    }
    return FALSE;
}

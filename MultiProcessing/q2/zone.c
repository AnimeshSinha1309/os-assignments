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
        delay(2);
        if (all_zones[i].vaccines == 0 && all_zones[i].num_slots == 0) {
            // Get the number of drugs and print it
            char* message = calloc(50, sizeof(char));
            all_zones[i].vaccines = company->doses_in_batch[company->num_batches - 1];
            sprintf(message, "company %d supplied %d doses", company->id + 1, all_zones[i].vaccines);
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
            // Update the zone
            char* message = calloc(50, sizeof(char));
            sprintf(message, "accepted student %d ", student->id + 1);
            title_print(CLASS_ZONE, all_zones[i].id, message);
            // Update the student
            student->state = i;
            all_zones[i].waiting_list[all_zones[i].used_slots] = student;
            all_zones[i].used_slots++;
            // Unlock and leave
            pthread_mutex_unlock(all_zones[i].mutex);
            return TRUE;
        }
        pthread_mutex_unlock(all_zones[i].mutex);
    }
    return FALSE;
}

void* zone_process(void *input) {
    Zone *zone = (Zone *) input;
    while (n_students_vaccinated < n_students) {
        pthread_mutex_lock(zone->mutex);
        if (zone->num_slots != 0 && zone->num_slots == zone->used_slots) {
            // If a set of slots is full, start vaccination and dump these slots
            title_print(CLASS_ZONE, zone->id, "is starting vaccination");
            for (int j = 0; j < zone->num_slots; j++) {
                student_test(zone->company_of_purchase, zone->waiting_list[j]);
            }
            zone->num_slots = 0;
            zone->used_slots = 0;
        } else if (zone->num_slots == 0 && zone->vaccines != 0) {
            // If all slots got dumped, and there are vaccines left, start off again
            zone->num_slots = min_3(8, n_students_arrived - n_students_vaccinated, zone->vaccines);
            zone->vaccines -= zone->num_slots;
            zone->waiting_list = (Student**) calloc(zone->num_slots, sizeof(Student*));
            zone->used_slots = 0;

            char* message = calloc(50, sizeof(char));
            sprintf(message, "made %d slots", zone->num_slots);
            title_print(CLASS_ZONE, zone->id, message);
        }
        pthread_mutex_unlock(zone->mutex);
    }
    return NULL;
}

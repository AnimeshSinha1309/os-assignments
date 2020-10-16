#include "company.h"
#include "functions.h"
#include "zone.h"
#include <stdlib.h>

void company_init(int n) {
    n_companies = n;
    all_companies = (Company*) calloc(n_companies, sizeof(Company));
    for (int i = 0; i < n_companies; i++) {
        all_companies[i].id = i;
        company_make(all_companies + i);
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
void company_make(Company *company) {
    scanf("%lf", &company->success);
}
#pragma clang diagnostic pop

void* company_process(void* input) {
    Company* company = (Company *) input;
    title_print(CLASS_COMPANY, company->id, "read about CoViD in the newspaper");
    while (n_students_vaccinated < n_students) {
        if (company->num_batches == 0) {
            // Poll to check if the drugs have been consumed
            bool can_produce = TRUE;
            for (int i = 0; i < n_zones; i++) {
                if (all_zones[i].company_of_purchase != NULL && all_zones[i].company_of_purchase->id == company->id
                        && all_zones[i].vaccines != 0)
                    can_produce = FALSE;
            }
            if (!can_produce) continue;
            // Wait for production time
            int batches_produced = randint(L_BATCHES_PRODUCED, R_BATCHES_PRODUCED);
            int time_delay = randint(L_PRODUCTION_TIME, R_PRODUCTION_TIME);
            char* message = calloc(50, sizeof(char));
            sprintf(message, "%d batches made in %d seconds", batches_produced, time_delay);
            delay(time_delay);
            title_print(CLASS_COMPANY, company->id, message);
            // Start dispatching
            company->num_batches = batches_produced;
            company->doses_in_batch = (int*) calloc(company->num_batches, sizeof(int));
            for (int i = 0; i < company->num_batches; i++)
                company->doses_in_batch[i] = randint(L_DOSES_IN_BATCH, R_DOSES_IN_BATCH);
        } else {
            // Give it to the zones
            assign_batch(company);
        }
    }
    return NULL;
}
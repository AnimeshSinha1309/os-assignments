#ifndef MULTIPROCESSING_COMPANY_H
#define MULTIPROCESSING_COMPANY_H

#define MANUFACTURING_PHASE 0

typedef struct Company {
    int id;
    int num_batches;
    double success;
    int* doses_in_batch;
} Company;

int n_companies;
Company *all_companies;

void company_init(int n); // Class initializer for company
void company_make(Company *company); // Constructor for company
void* company_process(void* input); // Proceeds with processing batch production and dispatch

#endif //MULTIPROCESSING_COMPANY_H

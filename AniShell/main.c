#include "processor/prompt.h"
#include "processor/parser.h"
#include "utils/terminate.h"

int main() {
    process_name = "anishell";
    prompt_init();
    setup_exit();
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        prompt_print();
        await_input();
    }
#pragma clang diagnostic pop
}

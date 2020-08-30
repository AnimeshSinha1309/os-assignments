#include "processor/prompt.h"
#include "processor/parser.h"

int main() {
    prompt_init();
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        prompt_print();
        await_input();
    }
#pragma clang diagnostic pop
}

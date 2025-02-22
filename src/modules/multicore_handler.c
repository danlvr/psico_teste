#include "modules/multicore_handler.h"
#include "common.h"
#include "pico/multicore.h"

// Core 1
void core1_entry() {
    while (1) {
        uintptr_t (*func)() = (uintptr_t(*)()) multicore_fifo_pop_blocking();
        (*func)();
    }
}

// Configura o core 1
void multicore_init() {
    multicore_launch_core1(core1_entry);
}; 

// Envia uma função para o core 1
void multicore_send_to_core_1(void(*function)(uintptr_t)) {
    multicore_fifo_push_blocking((uintptr_t) function);
};

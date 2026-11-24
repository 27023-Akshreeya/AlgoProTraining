#include <stdint.h>

#define STATUS_REG_ADDR 0x40001000

void wait_for_ready(void) {
    volatile uint32_t *status_reg = (volatile uint32_t *)STATUS_REG_ADDR;
    while ((*status_reg & 0x01) == 0) {
        // Wait for bit 0 to become 1
    }
}

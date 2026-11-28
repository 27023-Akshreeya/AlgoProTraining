#include <stdint.h>
#include <stdio.h>

volatile uint32_t GPIO_REG = 0x00000000;
volatile uint32_t TIMER_REG = 0x00000000;
volatile uint32_t INTERRUPT_REG = 0x00000000;

#define BIT_MASK(BIT) (1U << (BIT))
#define SET_BIT(REG, BIT_MASK) ((REG) |= (BIT_MASK))
#define CLEAR_BIT(REG, BIT_MASK) ((REG) &= ~(BIT_MASK))
#define TOGGLE_BIT(REG, BIT_MASK) ((REG) ^= (BIT_MASK))
#define READ_BIT(REG, MASK) (!!(REG) & (MASK))

int main(){
    printf("Initial GPIO_REG: 0x%08X\n", GPIO_REG);
    printf("Initial TIMER_REG: 0x%08X\n", TIMER_REG);
    printf("Initial INTERRUPT_REG: 0x%08X\n", INTERRUPT_REG);

    SET_BIT(GPIO_REG, BIT_MASK(3));
    SET_BIT(TIMER_REG, BIT_MASK(5));
    SET_BIT(INTERRUPT_REG, BIT_MASK(7));

    printf("After setting bits:\n");
    printf("GPIO_REG: 0x%08X\n", GPIO_REG);
    printf("TIMER_REG: 0x%08X\n", TIMER_REG);
    printf("INTERRUPT_REG: 0x%08X\n", INTERRUPT_REG);

    CLEAR_BIT(GPIO_REG, BIT_MASK(3));
    TOGGLE_BIT(TIMER_REG, BIT_MASK(6));
    TOGGLE_BIT(INTERRUPT_REG, BIT_MASK(7));

    printf("After clearing and toggling bits:\n");
    printf("GPIO_REG: 0x%08X\n", GPIO_REG);
    printf("TIMER_REG: 0x%08X\n", TIMER_REG);
    printf("INTERRUPT_REG: 0x%08X\n", READ_BIT(INTERRUPT_REG, BIT_MASK(7)));

}
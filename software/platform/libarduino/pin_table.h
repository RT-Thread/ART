#ifndef PIN_TABLE_H_INCLUDED
#define PIN_TABLE_H_INCLUDED

#include <stm32f4xx.h>

struct pin_index
{
    int index;
    uint32_t rcc;
    GPIO_TypeDef *gpio;
    uint32_t pin;
};

const struct pin_index * get_pin(uint8_t pin);
#endif // PIN_TABLE_H_INCLUDED

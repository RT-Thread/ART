#include <rtthread.h>
#include "pin_table.h"

static const struct pin_index pins[] =
{
    { 0, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_7},
    { 1, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_6},
    { 2, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_8},
    { 3, RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_11},
    { 4, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_14},
    { 5, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_13},
    { 6, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_11},
    { 7, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_9},

    { 8, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_12},
    { 9, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_13},
    {10, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_14},
    {11, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_15},
    {12, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_6},
    {13, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_5},

    {14, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_8},
    {15, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_9},
    {16, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_5},
    {17, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_6},
    {18, RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_6},
    {19, RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_7},
    {20, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_9},
    {21, RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_8},

    {22, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_12},
    {23, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_2},
    {24, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_1},
    {25, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_0},
    {26, RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_9},
    {27, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_13},
    {28, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_15},
    {29, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_12},
    {30, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_10},
    {31, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_8},
    {32, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_7},
    {33, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_4},
    {34, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_3},
    {35, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_2},
    {36, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_1},
    {37, RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_0},
    {38, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_11},
    {39, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_10},
    {40, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_7},
    {41, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_3},
    {42, RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_4},
    {43, RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_8},
    {44, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_15},
    {45, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_14},
    {46, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_11},
    {47, RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_5},
    {48, RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_10},
    {49, RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_15},
    {50, RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_4},
    {51, RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_7},
    {52, RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_3},
    {53, RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_4},
};

#define ITEM_NUM(items)	sizeof(items)/sizeof(items[0])
const struct pin_index * get_pin(uint8_t pin)
{
    const struct pin_index* index;

    if(pin < ITEM_NUM(pins))
    {
        index = &pins[pin];
    }
    else
    {
        index = RT_NULL;
    }

    return index;
};

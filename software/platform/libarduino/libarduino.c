#include <rtm.h>
#include <stm32f4xx.h>
#include <finsh.h>

#include "libarduino.h"

struct pin_index
{
	int index;
	uint32_t rcc;
	GPIO_TypeDef *gpio;
	uint32_t pin;
};

#define ITEM_NUM(items)	sizeof(items)/sizeof(items[0])

struct pin_index pins[] =
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

void pinMode(uint8_t pin, uint8_t mode)
{
	const struct pin_index* index;
	if (pin < ITEM_NUM(pins))
	{
	    GPIO_InitTypeDef  GPIO_InitStructure;
		index = &pins[pin];

	    /* GPIO Periph clock enable */
	    RCC_AHB1PeriphClockCmd(index->rcc, ENABLE);

	    /* Configure PE5 */
	    GPIO_InitStructure.GPIO_Pin = index->pin;
	    if (mode == OUTPUT)
	    {
	    	/* output setting */
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	    }
	    else
	    {
	    	/* input setting */
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	    }
	    GPIO_Init(index->gpio, &GPIO_InitStructure);
	}
}
FINSH_FUNCTION_EXPORT(pinMode, set pin mode);

void digitalWrite(uint8_t pin, uint8_t value)
{
	const struct pin_index* index;
	if (pin < ITEM_NUM(pins))
	{
		index = &pins[pin];
		if (value == HIGH)
			GPIO_SetBits(index->gpio, index->pin);
		else
			GPIO_ResetBits(index->gpio, index->pin);
	}
}
FINSH_FUNCTION_EXPORT(digitalWrite, write value to digital pin);

int digitalRead(uint8_t pin)
{
	int value;
	const struct pin_index *index;

	value = LOW;
	if (pin < ITEM_NUM(pins))
	{
		index = &pins[pin];
		if (GPIO_ReadInputDataBit(index->gpio, index->pin) == Bit_RESET)
			value = LOW;
		else
			value = HIGH;
	}

	return value;
}
FINSH_FUNCTION_EXPORT(digitalRead, read value from digital pin);

volatile static voidFuncPtr intFunc[EXTERNAL_NUM_INTERRUPTS];

static void defaultIsrHandler(void)
{
	rt_kprintf("Unknow ISR\n");
}

void initInterrupt(void)
{
	rt_uint32_t index;

	for (index = 0; index < EXTERNAL_NUM_INTERRUPTS; index ++)
	{
		intFunc[index] = defaultIsrHandler;
	}
}

void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode) {
	if (interruptNum < EXTERNAL_NUM_INTERRUPTS)
	{
		intFunc[interruptNum] = userFunc;
	}
}

void detachInterrupt(uint8_t interruptNum) {
	if (interruptNum < EXTERNAL_NUM_INTERRUPTS)
	{
		intFunc[interruptNum] = defaultIsrHandler;
	}
}

RTM_EXPORT(pinMode);
RTM_EXPORT(digitalWrite);
RTM_EXPORT(digitalRead);

RTM_EXPORT(attachInterrupt);
RTM_EXPORT(detachInterrupt);

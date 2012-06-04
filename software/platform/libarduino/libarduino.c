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

const static struct pin_index pins[] =
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

RTM_EXPORT(pinMode);
RTM_EXPORT(digitalWrite);
RTM_EXPORT(digitalRead);

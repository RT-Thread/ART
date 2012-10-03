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

typedef struct
{
	TIM_TypeDef *tim;
	uint32_t tim_rcc;
	uint16_t tim_channel;
	uint8_t pin_source;
	uint8_t gpio_af;
} pin_to_timer_index_t;

#define NOT_A_TIM RT_NULL
pin_to_timer_index_t pin_to_timer_index[] =
{
	{NOT_A_TIM},
	{NOT_A_TIM},
	{TIM3, RCC_APB1Periph_TIM3, TIM_Channel_3, GPIO_PinSource8, GPIO_AF_TIM3},
	{TIM2, RCC_APB1Periph_TIM2, TIM_Channel_4, GPIO_PinSource11, GPIO_AF_TIM2},
	{TIM1, RCC_APB2Periph_TIM1, TIM_Channel_4, GPIO_PinSource14, GPIO_AF_TIM1},

	{TIM1, RCC_APB2Periph_TIM1, TIM_Channel_3, GPIO_PinSource13, GPIO_AF_TIM1},
	{TIM1, RCC_APB2Periph_TIM1, TIM_Channel_2, GPIO_PinSource11, GPIO_AF_TIM1},
	{TIM1, RCC_APB2Periph_TIM1, TIM_Channel_1, GPIO_PinSource9, GPIO_AF_TIM1},
	{TIM4, RCC_APB1Periph_TIM4, TIM_Channel_1, GPIO_PinSource12, GPIO_AF_TIM4},
	{TIM4, RCC_APB1Periph_TIM4, TIM_Channel_2, GPIO_PinSource13, GPIO_AF_TIM4},

	{TIM4, RCC_APB1Periph_TIM4, TIM_Channel_3, GPIO_PinSource14, GPIO_AF_TIM4},
	{TIM4, RCC_APB1Periph_TIM4, TIM_Channel_4, GPIO_PinSource15, GPIO_AF_TIM4},
	{TIM9, RCC_APB2Periph_TIM8, TIM_Channel_2, GPIO_PinSource6, GPIO_AF_TIM9},
	{TIM9, RCC_APB2Periph_TIM9, TIM_Channel_1, GPIO_PinSource5, GPIO_AF_TIM9},
};

rt_inline pin_to_timer_index_t *pin_to_timer(uint8_t pin)
{
	if (pin >= ITEM_NUM(pin_to_timer_index))
		return RT_NULL;
	else
		return pin_to_timer_index + pin;
}

typedef struct
{
	uint32_t rcc;
	GPIO_TypeDef *gpio;
	uint32_t pin;
	ADC_TypeDef *adc;
	uint32_t adc_rcc;
	uint8_t adc_channel;
} pin_to_analog_index_t;

pin_to_analog_index_t pin_to_analog_index[] =
{
	{RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_0, ADC1, RCC_APB2Periph_ADC1, ADC_Channel_10},
	{RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_1, ADC1, RCC_APB2Periph_ADC1, ADC_Channel_11},
	{RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_0, ADC1, RCC_APB2Periph_ADC1, ADC_Channel_0},
	{RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_1, ADC1, RCC_APB2Periph_ADC1, ADC_Channel_1},
	{RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_2, ADC1, RCC_APB2Periph_ADC1, ADC_Channel_2},

	{RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_3, ADC1, RCC_APB2Periph_ADC1, ADC_Channel_3},
	{RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_5, ADC1, RCC_APB2Periph_ADC1, ADC_Channel_5},
	{RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_6, ADC1, RCC_APB2Periph_ADC1, ADC_Channel_6},
	{RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_4, ADC1, RCC_APB2Periph_ADC1, ADC_Channel_14},
	{RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_5, ADC1, RCC_APB2Periph_ADC1, ADC_Channel_15},

	{RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_0, ADC1, RCC_APB2Periph_ADC1, ADC_Channel_8},
	{RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_1, ADC1, RCC_APB2Periph_ADC1, ADC_Channel_9},
	{RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_3, ADC1, RCC_APB2Periph_ADC1, ADC_Channel_13},
	{RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_2, ADC1, RCC_APB2Periph_ADC1, ADC_Channel_12}
};

rt_inline pin_to_analog_index_t *pin_to_analog(uint8_t pin)
{
	if (pin >= ITEM_NUM(pin_to_analog_index))
		return RT_NULL;
	else
		return pin_to_analog_index + pin;
}

void pinMode(uint8_t pin, uint8_t mode)
{
	const struct pin_index* index;
	if (pin < ITEM_NUM(pins))
	{
	    GPIO_InitTypeDef  GPIO_InitStructure;
		index = &pins[pin];

	    /* GPIO Periph clock enable */
	    RCC_AHB1PeriphClockCmd(index->rcc, ENABLE);

	    /* Configure GPIO_InitStructure */
	    GPIO_InitStructure.GPIO_Pin = index->pin;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	    if (mode == OUTPUT)
	    {
	    	/* output setting */
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
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

void pwmConfig(uint8_t pin, uint8_t duty_cycle, unsigned int frequency, unsigned int clock)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	uint16_t PrescalerValue;
	GPIO_InitTypeDef GPIO_InitStructure;
	pin_to_timer_index_t *timer_index;
	struct pin_index *pin_index_p;
	uint32_t period = clock / frequency - 1;
	RT_ASSERT(period <= UINT16_MAX);

 	if(pin >= ITEM_NUM(pins))
		return;
	pin_index_p = pins + pin;

	timer_index = pin_to_timer(pin);
	if(!timer_index || !timer_index->tim)
		return;

	/* TIM clock enable */
	RCC_APB2PeriphClockCmd(timer_index->tim_rcc, ENABLE);

	/* GPIO clock enable */
	RCC_AHB1PeriphClockCmd(pin_index_p->rcc, ENABLE);

	/* GPIO configuration */
	GPIO_InitStructure.GPIO_Pin = pin_index_p->pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(pin_index_p->gpio, &GPIO_InitStructure);
	/* Connect TIM3 pins to AF */
	GPIO_PinAFConfig(pin_index_p->gpio, timer_index->pin_source, timer_index->gpio_af);

	/* Compute the prescaler value */
	RT_ASSERT(SystemCoreClock / 2 / clock - 1 <= UINT16_MAX);
	PrescalerValue = (uint16_t) (SystemCoreClock / 2 / clock - 1);

	/* Time base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = period;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(timer_index->tim, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = period * duty_cycle / UINT8_MAX;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	switch (timer_index->tim_channel)
	{
		case TIM_Channel_1:
			TIM_OC1Init(timer_index->tim, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(timer_index->tim, TIM_OCPreload_Enable);
			break;
		case TIM_Channel_2:
			TIM_OC1Init(timer_index->tim, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(timer_index->tim, TIM_OCPreload_Enable);
			break;
		case TIM_Channel_3:
			TIM_OC1Init(timer_index->tim, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(timer_index->tim, TIM_OCPreload_Enable);
			break;
		case TIM_Channel_4:
			TIM_OC1Init(timer_index->tim, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(timer_index->tim, TIM_OCPreload_Enable);
			break;
		default:
			RT_ASSERT(0);
			return;
	}

	TIM_ARRPreloadConfig(timer_index->tim, ENABLE);
	TIM_Cmd(timer_index->tim, ENABLE);
}

#define PWM_COUNTER_CLOCK 	1000000	// choose a value which will result in proper prescalar value
#define PWM_FREQUENCY		64000	// choose a value which will result in proper period
void analogWrite(uint8_t pin, uint8_t value)
{
	// We need to make sure the PWM output is enabled for those pins
	// that support it, as we turn it off when digitally reading or
	// writing with them.  Also, make sure the pin is in output mode
	// for consistenty with Wiring, which doesn't require a pinMode
	// call for the analog output pins.

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	if (value == 0)
	{
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	}
	else if (value == 255)
	{
		pinMode(pin, OUTPUT);
		digitalWrite(pin, HIGH);
	}
	else
	{
		pwmConfig(pin, value, PWM_FREQUENCY, PWM_COUNTER_CLOCK);
	}
}
FINSH_FUNCTION_EXPORT(analogWrite, write analog value to digital pin using pwm);

int analogRead(uint8_t pin)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef GPIO_InitStruture;
	pin_to_analog_index_t *pin_index_p;

	pin_index_p = pin_to_analog(pin);

	RCC_AHB1PeriphClockCmd(pin_index_p->rcc, ENABLE);

	GPIO_InitStruture.GPIO_Pin = pin_index_p->pin;
	GPIO_InitStruture.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruture.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(pin_index_p->gpio, &GPIO_InitStruture);

	RCC_APB2PeriphClockCmd(pin_index_p->adc_rcc, ENABLE);

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(pin_index_p->adc, &ADC_InitStructure);

	ADC_RegularChannelConfig(pin_index_p->adc, pin_index_p->adc_channel, 1, ADC_SampleTime_3Cycles);
	ADC_Cmd(pin_index_p->adc, ENABLE);

	ADC_SoftwareStartConv(pin_index_p->adc);
	while(ADC_GetFlagStatus(pin_index_p->adc, ADC_FLAG_EOC) == RESET)
		;
	return ADC_GetConversionValue(pin_index_p->adc);
}
FINSH_FUNCTION_EXPORT(analogRead, Reads the value from the specified analog pin);

void noTone(uint8_t pin)
{
	pin_to_timer_index_t *index_p = pin_to_timer(pin);
	if (index_p)
	{
		TIM_Cmd(index_p->tim, DISABLE);
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	}
}
FINSH_FUNCTION_EXPORT(noTone, Stops the generation of a square wave triggered by tone());


/* normally the tone frequency should be 31 to 4978, refer to piches.h */
void tone(uint8_t pin, uint16_t frequency, unsigned long duration)
{
	RT_ASSERT(frequency * 2 < UINT16_MAX);
	pwmConfig(pin, UINT8_MAX / 2, frequency, UINT16_MAX);
	rt_thread_delay(rt_tick_from_millisecond(duration));
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}
FINSH_FUNCTION_EXPORT(tone, generates a square wave of specified freqency (and 50% duty cycle) on a pin);

/**************************** interrupt ****************************/
/*
So interrupt
0 -- PWM2 or digital pin 2
1 -- PWM3 or digital pin 3
2 -- SCL or 21
3 -- SDA or 20
4 -- RXD1 or 19
5 -- TXD1 or 18
6 -- not routed out
7 -- not routed out
*/

/* PC8: TIM3_CH34 */
static void int0_init(int mode)
{
    rt_kprintf("no implement!\r\n");
}

/* PB11: INT11 */
static void int1_init(int mode)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    uint32_t EXTI_Line = EXTI_Line11;

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);

    /* Enable the EXTI0 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure EXTI  */
    EXTI_InitStructure.EXTI_Line = EXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

    if(mode == FALLING)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    }
    else if(mode == RISING)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    }
    else if(mode == CHANGE)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    }

    EXTI_InitStructure.EXTI_LineCmd = ENABLE;

    EXTI_ClearITPendingBit(EXTI_Line);
    EXTI_Init(&EXTI_InitStructure);
}

/* PA8: INT8 */
static void int2_init(int mode)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    uint32_t EXTI_Line = EXTI_Line8;

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource8);

    /* Enable the EXTI0 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure EXTI  */
    EXTI_InitStructure.EXTI_Line = EXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

    if(mode == FALLING)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    }
    else if(mode == RISING)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    }
    else if(mode == CHANGE)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    }

    EXTI_InitStructure.EXTI_LineCmd = ENABLE;

    EXTI_ClearITPendingBit(EXTI_Line);
    EXTI_Init(&EXTI_InitStructure);
}

/* PC9: INT9 */
static void int3_init(int mode)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    uint32_t EXTI_Line = EXTI_Line9;

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource9);

    /* Enable the EXTI0 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure EXTI  */
    EXTI_InitStructure.EXTI_Line = EXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

    if(mode == FALLING)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    }
    else if(mode == RISING)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    }
    else if(mode == CHANGE)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    }

    EXTI_InitStructure.EXTI_LineCmd = ENABLE;

    EXTI_ClearITPendingBit(EXTI_Line);
    EXTI_Init(&EXTI_InitStructure);
}

/* PB7: INT7 */
static void int4_init(int mode)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    uint32_t EXTI_Line = EXTI_Line7;

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource7);

    /* Enable the EXTI0 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure EXTI  */
    EXTI_InitStructure.EXTI_Line = EXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

    if(mode == FALLING)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    }
    else if(mode == RISING)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    }
    else if(mode == CHANGE)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    }

    EXTI_InitStructure.EXTI_LineCmd = ENABLE;

    EXTI_ClearITPendingBit(EXTI_Line);
    EXTI_Init(&EXTI_InitStructure);
}

/* PB6: EXTI_Line6 */
static void int5_init(int mode)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    uint32_t EXTI_Line = EXTI_Line6;

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource6);

    /* Enable the EXTI0 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure EXTI  */
    EXTI_InitStructure.EXTI_Line = EXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

    if(mode == FALLING)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    }
    else if(mode == RISING)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    }
    else if(mode == CHANGE)
    {
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    }

    EXTI_InitStructure.EXTI_LineCmd = ENABLE;

    EXTI_ClearITPendingBit(EXTI_Line);
    EXTI_Init(&EXTI_InitStructure);
}

static void int6_init(int mode)
{
    rt_kprintf("no implement!\r\n");
}
static void int7_init(int mode)
{
    rt_kprintf("no implement!\r\n");
}

typedef void (*int_init_func_ptr)(int mode);
static const int_init_func_ptr init_func[EXTERNAL_NUM_INTERRUPTS] =
{
    int0_init, /* TIM3_CH34 */
    int1_init, /* EXTI_Line11 */
    int2_init, /* EXTI_Line8 */
    int3_init, /* EXTI_Line9 */
    int4_init, /* EXTI_Line7 */
    int5_init, /* EXTI_Line6 */
    int6_init,
    int7_init,
};

volatile static voidFuncPtr intFunc[EXTERNAL_NUM_INTERRUPTS];

void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line5);
        rt_kprintf("EXTI_Line5\n");
    }

    if(EXTI_GetITStatus(EXTI_Line6) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line6);
        if(intFunc[5] != RT_NULL)
        {
            intFunc[5]();
        }
    }

    if(EXTI_GetITStatus(EXTI_Line7) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line7);
        if(intFunc[4] != RT_NULL)
        {
            intFunc[4]();
        }
    }

    if(EXTI_GetITStatus(EXTI_Line8) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line8);
        if(intFunc[2] != RT_NULL)
        {
            intFunc[2]();
        }
    }

    if(EXTI_GetITStatus(EXTI_Line9) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line9);
        if(intFunc[3] != RT_NULL)
        {
            intFunc[3]();
        }
    }
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line10) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line10);
        rt_kprintf("EXTI_Line10\n");
    }

    if(EXTI_GetITStatus(EXTI_Line11) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line11);
        if(intFunc[1] != RT_NULL)
        {
            intFunc[1]();
        }
    }

    if(EXTI_GetITStatus(EXTI_Line12) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line12);
        rt_kprintf("EXTI_Line12\n");
    }

    if(EXTI_GetITStatus(EXTI_Line13) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line13);
        rt_kprintf("EXTI_Line13\n");
    }

    if(EXTI_GetITStatus(EXTI_Line14) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line14);
        rt_kprintf("EXTI_Line14\n");
    }

    if(EXTI_GetITStatus(EXTI_Line15) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line15);
        rt_kprintf("EXTI_Line15\n");
    }
}

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

void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode)
{
    if (interruptNum < EXTERNAL_NUM_INTERRUPTS)
    {
        intFunc[interruptNum] = userFunc;
        init_func[interruptNum](mode);
    }
}

void detachInterrupt(uint8_t interruptNum)
{
    if (interruptNum < EXTERNAL_NUM_INTERRUPTS)
    {
        intFunc[interruptNum] = defaultIsrHandler;
    }
}
/**************************** interrupt ****************************/

RTM_EXPORT(pinMode);
RTM_EXPORT(digitalWrite);
RTM_EXPORT(digitalRead);
RTM_EXPORT(analogWrite);
RTM_EXPORT(analogRead);
RTM_EXPORT(tone);

RTM_EXPORT(attachInterrupt);
RTM_EXPORT(detachInterrupt);

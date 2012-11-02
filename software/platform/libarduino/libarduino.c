#include <rtm.h>
#include <stm32f4xx.h>
#include <finsh.h>

#include "libarduino.h"
#include "pin_table.h"
#include "tim.h"

#define ITEM_NUM(items) sizeof(items)/sizeof(items[0])

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
    if(pin >= ITEM_NUM(pin_to_analog_index))
        return RT_NULL;
    else
        return pin_to_analog_index + pin;
}

void pinMode(uint8_t pin, uint8_t mode)
{
    const struct pin_index *index;
    GPIO_InitTypeDef  GPIO_InitStructure;

    index = get_pin(pin);
    if(index == RT_NULL)
    {
        return;
    }

    /* GPIO Periph clock enable */
    RCC_AHB1PeriphClockCmd(index->rcc, ENABLE);

    /* Configure GPIO_InitStructure */
    GPIO_InitStructure.GPIO_Pin = index->pin;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    if(mode == OUTPUT)
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
FINSH_FUNCTION_EXPORT(pinMode, set pin mode);

void digitalWrite(uint8_t pin, uint8_t value)
{
    const struct pin_index *index;

    index = get_pin(pin);
    if(index == RT_NULL)
    {
        return;
    }

    if(value == LOW)
    {
        GPIO_ResetBits(index->gpio, index->pin);
    }
    else
    {
        GPIO_SetBits(index->gpio, index->pin);
    }
}
FINSH_FUNCTION_EXPORT(digitalWrite, write value to digital pin);

int digitalRead(uint8_t pin)
{
    int value;
    const struct pin_index *index;

    value = LOW;

    index = get_pin(pin);
    if(index == RT_NULL)
    {
        return value;
    }

    if(GPIO_ReadInputDataBit(index->gpio, index->pin) == Bit_RESET)
    {
        value = LOW;
    }
    else
    {
        value = HIGH;
    }

    return value;
}
FINSH_FUNCTION_EXPORT(digitalRead, read value from digital pin);

#define PWM_COUNTER_CLOCK       1000000 // choose a value which will result in proper prescalar value
#define PWM_FREQUENCY_DEFAULT   1000    // choose a value which will result in proper period
void analogWrite(uint8_t pin, uint8_t value)
{
    // We need to make sure the PWM output is enabled for those pins
    // that support it, as we turn it off when digitally reading or
    // writing with them.  Also, make sure the pin is in output mode
    // for consistenty with Wiring, which doesn't require a pinMode
    // call for the analog output pins.

    if(value == 0)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    else if(value == 255)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }
    else
    {
        pwmConfig(pin, value);
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
    while (ADC_GetFlagStatus(pin_index_p->adc, ADC_FLAG_EOC) == RESET)
        ;
    return ADC_GetConversionValue(pin_index_p->adc);
}
FINSH_FUNCTION_EXPORT(analogRead, Reads the value from the specified analog pin);

void noTone(uint8_t pin)
{
    const pin_to_timer_index_t *index_p = pin_to_timer(pin);
    if(index_p)
    {
        TIM_Cmd(index_p->tim, DISABLE);
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
}
FINSH_FUNCTION_EXPORT(noTone, Stops the generation of a square wave triggered by tone());

static void pwm_shutdown(void *pin)
{
    uint32_t p;

    p = (rt_uint32_t) pin;
    pinMode((uint8_t) p, OUTPUT);
    digitalWrite((uint8_t) p, LOW);
}
/* normally the tone frequency should be 31 to 4978, refer to piches.h */
void tone(uint8_t pin, uint16_t frequency, unsigned long duration)
{
    static struct rt_timer timer2, timer3;
    rt_timer_t timer;
    
    RT_ASSERT(frequency * 2 < UINT16_MAX);
    if(pin < 2 || pin > 3)
        return;
    if (pin == 2)
    {
        timer = &timer2;
    }
    else if (pin == 3)
    {
        timer = &timer3;    
    }
    rt_timer_stop(timer);
    rt_timer_init(timer, "pwmkill", 
                  pwm_shutdown, (void*) pin,
                  rt_tick_from_millisecond(duration),
                  RT_TIMER_FLAG_ONE_SHOT);
    TIM_config(pin, frequency);
    pwmConfig(pin, UINT8_MAX / 2);
    rt_timer_start(timer);
}
FINSH_FUNCTION_EXPORT(tone, generates a square wave of specified freqency ( and 50 % duty cycle) on a pin);

void exec(const char* module)
{
    rt_module_open(module);
}
RTM_EXPORT(exec);

RTM_EXPORT(pinMode);
RTM_EXPORT(digitalWrite);
RTM_EXPORT(digitalRead);
RTM_EXPORT(analogWrite);
RTM_EXPORT(analogRead);
RTM_EXPORT(noTone);
RTM_EXPORT(tone);

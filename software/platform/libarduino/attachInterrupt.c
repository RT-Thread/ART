#include <rtm.h>
#include <stm32f4xx.h>
#include "libarduino.h"

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
RTM_EXPORT(attachInterrupt);

void detachInterrupt(uint8_t interruptNum)
{
    if (interruptNum < EXTERNAL_NUM_INTERRUPTS)
    {
        intFunc[interruptNum] = defaultIsrHandler;
    }
}
RTM_EXPORT(detachInterrupt);

/**************************** interrupt ****************************/

#include <rtm.h>
#include <stm32f4xx.h>

#include <finsh.h>

void led_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* GPIO Periph clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    /* Configure PE5 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}
FINSH_FUNCTION_EXPORT(led_init, initial led setting);

void led(int no, int level)
{
	if (level)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_5);
	}
}
FINSH_FUNCTION_EXPORT(led, led on or off);

RTM_EXPORT(led);
RTM_EXPORT(led_init);

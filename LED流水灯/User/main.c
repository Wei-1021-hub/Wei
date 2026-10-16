#include "stm32f10x.h"                  // Device header

volatile uint32_t g_systick_ms = 0;

#define LED_GPIO_PORT       GPIOC
#define LED_COUNT           9U
#define LED_ALL_PINS        (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | \
                             GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | \
                             GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8)
#define LED_STEP_MS         100U
#define LED_PIN_MASK(n)     ((uint16_t)(1U << (n)))

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint32_t previous_step_ms;
	uint8_t active_led = 0U;

	/* Configure PC0-PC8 as push-pull outputs. */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_ALL_PINS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

	/* The LEDs are active-low, so keep all of them off initially. */
	GPIO_SetBits(LED_GPIO_PORT, LED_ALL_PINS);

	/* Generate the 1 ms time base used to create the 10 Hz sequence. */
	if (SysTick_Config(SystemCoreClock / 1000U) != 0U)
	{
		while (1)
		{
		}
	}

	previous_step_ms = g_systick_ms;
	GPIO_ResetBits(LED_GPIO_PORT, LED_PIN_MASK(active_led));

	while(1)
	{
		if ((uint32_t)(g_systick_ms - previous_step_ms) >= LED_STEP_MS)
		{
			previous_step_ms += LED_STEP_MS;

			active_led++;
			if (active_led >= LED_COUNT)
			{
				active_led = 0U;
			}

			GPIO_SetBits(LED_GPIO_PORT, LED_ALL_PINS);
			GPIO_ResetBits(LED_GPIO_PORT, LED_PIN_MASK(active_led));
		}
	}
}

#include <stdbool.h>
#include <stm8s.h>
#include "main.h"
#include "milis.h"
#include "daughterboard.h"

volatile uint8_t ledpointer = 0;

void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    init_milis();

    GPIO_Init(LED2_PORT, LED2_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LED1_PORT, LED1_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LED4_PORT, LED4_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    GPIO_Init(S1_PORT, S1_PIN, GPIO_MODE_IN_PU_NO_IT);

    TIM3_TimeBaseInit(TIM3_PRESCALER_128, 50000 - 1);
    TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);
    TIM3_Cmd(ENABLE);

    enableInterrupts();
}

void leds_off(void)
{
    GPIO_WriteLow(LED1_PORT, LED1_PIN);
    GPIO_WriteLow(LED2_PORT, LED2_PIN);
    GPIO_WriteLow(LED4_PORT, LED4_PIN);
}

int main(void)
{
    uint32_t time = 0;
    bool S1_press = false;

    init();

    while (1) {

        if (milis() - time > 33) {
            time = milis();

            bool now = PUSH(S1);

            if (now && !S1_press) {

                ledpointer++;
                if (ledpointer > 2)
                    ledpointer = 0;

                leds_off();
            }

            S1_press = now;
        }
    }
}
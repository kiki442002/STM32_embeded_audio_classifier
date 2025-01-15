#include "touchscreen.h"

void touchscreen_Handle(void)
{
    TS_StateTypeDef TS_State;
    BSP_TS_GetState(&TS_State);
    if (TS_State.touchDetected)
    {
        if (TS_State.touchX[0] < 100 && TS_State.touchY[0] < 100)
        {
            BSP_LED_Toggle(LED1);
        }
    }
}
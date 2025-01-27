#include "touchscreen.h"
#include "screen.h"
#include "../../Drivers/BSP/STM32F769I-Discovery/stm32f769i_discovery_audio.h"
#include "filtrage.h"

uint32_t last_tick = 0;
void touchscreen_Handle(void)
{
    TS_StateTypeDef TS_State;
    BSP_TS_GetState(&TS_State);
    if (TS_State.touchDetected && TS_State.touchY[0] > START_Y && TS_State.touchY[0] < START_Y + BUTTON_WIDTH_HEIGHT && HAL_GetTick() - last_tick > 500)
    {
        // Play/Pause Button
        if (TS_State.touchX[0] > START_X && TS_State.touchX[0] < START_X + BUTTON_WIDTH_HEIGHT)
        {
            hApp.play_activation = !hApp.play_activation;
            if (hApp.play_activation)
            {
                Draw_Pause_Button();
                if (!hApp.IA_activation)

                    Draw_AI_Button(LCD_COLOR_WHITE);
                else
                    Draw_AI_Button(LCD_COLOR_GREEN);
                //  Draw_SD_Button(LCD_COLOR_WHITE);
                if (!hApp.output_activation)
                    Draw_Output_Button(LCD_COLOR_WHITE);
                else
                    Draw_Output_Button(LCD_COLOR_GREEN);
                Feature_Export_Reset();
                BSP_AUDIO_IN_Resume();
                BSP_AUDIO_OUT_Resume();
                BSP_AUDIO_OUT_SetMute(!hApp.output_activation);
            }
            else
            {
                Draw_Play_Button();
                // Draw_SD_Button(LCD_COLOR_GRAY);
                Draw_Output_Button(LCD_COLOR_GRAY);
                Draw_AI_Button(LCD_COLOR_GRAY);
                BSP_AUDIO_OUT_Pause();
                BSP_AUDIO_IN_Pause();
            }
        }

        // Audio Out Button
        if (TS_State.touchX[0] > START_X + SPACE_BETWEEN_BUTTONS && TS_State.touchX[0] < START_X + SPACE_BETWEEN_BUTTONS + BUTTON_WIDTH_HEIGHT && hApp.play_activation)
        {

            hApp.output_activation = !hApp.output_activation;

            if (hApp.output_activation)
                Draw_Output_Button(LCD_COLOR_GREEN);
            else
                Draw_Output_Button(LCD_COLOR_WHITE);
            BSP_AUDIO_OUT_SetMute(!hApp.output_activation);
        }
        if (TS_State.touchX[0] > START_X + SPACE_BETWEEN_BUTTONS * 3 && TS_State.touchX[0] < START_X + SPACE_BETWEEN_BUTTONS * 3 + BUTTON_WIDTH_HEIGHT && hApp.play_activation)
        {

            if (!hApp.IA_activation)
            {
                Draw_AI_Button(LCD_COLOR_GREEN);
                Feature_Export_Reset();
                BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 35, "Prediction:", CENTER_MODE);
            }

            else
            {
                Draw_AI_Button(LCD_COLOR_WHITE);
                Clear_Log();
            }

            hApp.IA_activation = !hApp.IA_activation;
        }
        last_tick = HAL_GetTick();
    }
}
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
                if (!hApp.output_activation)
                    Draw_Output_Button(LCD_COLOR_WHITE);
                else
                    Draw_Output_Button(LCD_COLOR_GREEN);

                if (hApp.SD_state == SD_APP_PRESENT)
                {
                    Draw_SD_Button(LCD_COLOR_WHITE);
                }
                else
                {
                    Draw_SD_Button(LCD_COLOR_GRAY);
                }
                Feature_Export_Reset();
                BSP_AUDIO_IN_Resume();
                BSP_AUDIO_OUT_Resume();
                BSP_AUDIO_OUT_SetMute(!hApp.output_activation);
            }
            else
            {
                Draw_Play_Button();
                Draw_SD_Button(LCD_COLOR_GRAY);
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
                BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 35, (uint8_t *)("Prediction:"), CENTER_MODE);
            }

            else
            {
                Draw_AI_Button(LCD_COLOR_WHITE);
                Clear_Log();
            }

            hApp.IA_activation = !hApp.IA_activation;
        }
        if (TS_State.touchX[0] > START_X + SPACE_BETWEEN_BUTTONS * 2 && TS_State.touchX[0] < START_X + SPACE_BETWEEN_BUTTONS * 2 + BUTTON_WIDTH_HEIGHT && hApp.play_activation && hApp.SD_state == SD_APP_PRESENT)
        {
            hApp.record_activation = !hApp.record_activation;
            if (hApp.record_activation)
            {
                Draw_SD_Button(LCD_COLOR_GREEN);
            }
            else
            {
                Draw_SD_Button(LCD_COLOR_WHITE);
            }
        }
        last_tick = HAL_GetTick();
    }
    if (TS_State.touchY[0] > BSP_LCD_GetYSize() - 55 && TS_State.touchY[0] < BSP_LCD_GetYSize() - 5)
    {
        if (TS_State.touchX[0] > 20 && TS_State.touchX[0] < BSP_LCD_GetXSize() / 2 - 20)
        {
            hApp.luminosity = (uint8_t)((TS_State.touchX[0]) * 100 / (BSP_LCD_GetXSize() / 2 - 20));
            if (hApp.luminosity > LUMINOSITY_MAX)
                hApp.luminosity = LUMINOSITY_MAX;
            else if (hApp.luminosity < LUMINOSITY_MIN)
                hApp.luminosity = LUMINOSITY_MIN;
            Draw_luminosity(hApp.luminosity);
            BSP_LCD_SetBrightness(hApp.luminosity);
        }
        if (TS_State.touchX[0] > BSP_LCD_GetXSize() / 2 + 20 && TS_State.touchX[0] < BSP_LCD_GetXSize() - 20)
        {
            hApp.volume = (uint8_t)((TS_State.touchX[0] - BSP_LCD_GetXSize() / 2) * 100 / (BSP_LCD_GetXSize() / 2 - 40));
            if (hApp.volume > VOLUME_MAX)
                hApp.volume = VOLUME_MAX;
            else if (hApp.volume < VOLUME_MIN)
                hApp.volume = VOLUME_MIN;
            Draw_volume(hApp.volume);
            BSP_AUDIO_OUT_SetVolume(hApp.volume);
        }
    }
}

#include "main.h"
#include "../../Drivers/BSP/STM32F769I-Discovery/stm32f769i_discovery_lcd.h"

#define BUTTON_WIDTH_HEIGHT 140
#define START_X 40
#define START_Y 40
#define SPACE_BETWEEN_BUTTONS 190

void Draw_Play_Button(void);
void print_Menu_Interface(void);
void Draw_Pause_Button(void);
void Draw_SD_Button(uint32_t color);
void Draw_AI_Button(uint32_t color);
void Clear_Log(void);
void Draw_Output_Button(uint32_t color);
void Draw_volume(uint8_t volume);
void Draw_luminosity(uint8_t luminosity);
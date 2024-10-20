#include "main.h"
#include "../../Drivers/BSP/STM32F769I-Discovery/stm32f769i_discovery_lcd.h"

#define BUTTON_WIDTH_HEIGHT 140
#define START_X 40
#define START_Y 40
#define SPACE_BETWEEN_BUTTONS 190

#define IA_ACTIVATE (uint8_t)1
#define IA_DESACTIVATE (uint8_t)0
#define RECORD_ACTIVATE (uint8_t)1
#define RECORD_DESACTIVATE (uint8_t)0
#define PLAY_ACTIVATE (uint8_t)1
#define PLAY_DESACTIVATE (uint8_t)0
#define OUTPUT_ACTIVATE (uint8_t)1
#define OUTPUT_DESACTIVATE (uint8_t)0
#define LUMINOSITY_MAX (uint8_t)100
#define LUMINOSITY_MIN (uint8_t)10
#define VOLUME_MAX (uint8_t)100
#define VOLUME_MIN (uint8_t)0


void Draw_Play_Button(void);
void print_Menu_Interface(void);
void Draw_Pause_Button(void);
void Draw_Output_Button(void);
void Draw_SD_Button(void);
void Draw_AI_Button(void);
void Clear_Log(void);
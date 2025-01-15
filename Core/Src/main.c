

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "rtc.h"
#include "app_x-cube-ai.h"
#include "fatfs.h"
#include "rtc.h"
#include "usart.h"
#include "ai_datatypes_defines.h"
#include "ai_platform.h"
#include "audio_classifier.h"
#include "audio_classifier_data.h"

#include "../../Drivers/BSP/STM32F769I-Discovery/stm32f769i_discovery.h"
#include "../../Drivers/BSP/STM32F769I-Discovery/stm32f769i_discovery_audio.h"
#include "../../Drivers/BSP/STM32F769I-Discovery/stm32f769i_discovery_lcd.h"
#include "../../Drivers/BSP/STM32F769I-Discovery/stm32f769i_discovery_ts.h"
#include "filtrage.h"
#include "screen.h"
#include "touchscreen.h"
/* END Includes -------------------------------------------------------------------*/

/* Structure ----------------------------------------------------------------------*/
typedef struct App_HandleTypeDef
{
  uint8_t IA_activation;
  uint8_t record_activation;
  uint8_t play_activation;
  uint8_t output_activation;
  uint8_t luminosity;
  uint8_t volume;
} App_HandleTypeDef;
/* END Structure -----------------------------------------------------------------*/

/* Variables ---------------------------------------------------------------------*/
uint16_t RecordBuffer[STEREO_RECORD_BUFFER_SIZE];
float32_t MelData[30 * 32];

int32_t Scratch[SCRATCH_BUFF_SIZE];
uint8_t audio_rec_buffer_state;
App_HandleTypeDef hApp;
/* END Variables -----------------------------------------------------------------*/

/* Private function prototypes ---------------------------------------------------*/
void SystemClock_Config(void);
/* END Private function prototypes -----------------------------------------------*/

int main(void)
{

  uint8_t lcd_status = LCD_OK;

  /* Enable Cache ------------------------------------------------------------*/
  SCB_EnableICache();
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_CRC_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  MX_X_CUBE_AI_Init();
  Feature_Export_Init();
  BSP_LED_Init(LED1);

  // LCD Initialisation
  lcd_status = BSP_LCD_Init();
  while (lcd_status != LCD_OK)
    ;
  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Hello, world!", CENTER_MODE);

  // TouchScreen Initialisation
  if (BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) != TS_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Error: TS Init", CENTER_MODE);
    Error_Handler();
  }
  if (BSP_TS_ITConfig() != TS_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Error: TS IT Config", CENTER_MODE);
    Error_Handler();
  }

  // Audio In Initialisation
  if (BSP_AUDIO_IN_Init(BSP_AUDIO_FREQUENCY_16K, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR) != AUDIO_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Error: AUDIO IN initialization failed", CENTER_MODE);
    Error_Handler();
  }
  if (BSP_AUDIO_IN_AllocScratch(Scratch, SCRATCH_BUFF_SIZE) != AUDIO_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Error: AUDIO IN Scratch", CENTER_MODE);
    Error_Handler();
  }

  // Audio Out Initialisation
  if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE,
                         100,
                         BSP_AUDIO_FREQUENCY_16K) != AUDIO_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Error: AUDIO OUT INIT", CENTER_MODE);
    Error_Handler();
  }
  BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);

  /* END MCU Configuration---------------------------------------------------*/

  printf("\rHello, world!\n\r");
  HAL_Delay(1000);
  print_Menu_Interface();

  if (BSP_AUDIO_IN_Record((uint16_t *)&RecordBuffer[0], STEREO_RECORD_BUFFER_SIZE) != AUDIO_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Error: AUDIO IN RECORD", CENTER_MODE);
    Error_Handler();
  }

  audio_rec_buffer_state = BUFFER_OFFSET_NONE;
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Enregistrement Audio", CENTER_MODE);

  HAL_Delay(2);

  /* Play the recorded buffer */
  BSP_AUDIO_OUT_Play((uint16_t *)&RecordBuffer[0], STEREO_RECORD_BUFFER_SIZE * 2);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 40, (uint8_t *)"Retour Active", CENTER_MODE);

  // OpenWavFile();

  uint8_t res = FEATURE_EXPORT_PROGRESS;

  // Infinite Loop
  while (1)
  {
    if (audio_rec_buffer_state != BUFFER_OFFSET_NONE)
    {
      /* Copy half of the record buffer to the playback buffer */
      if (audio_rec_buffer_state == BUFFER_OFFSET_HALF)
      {
        uint32_t start = HAL_GetTick();
        res = Feature_Export(MelData, (int16_t *)RecordBuffer, BUFFER_OFFSET_HALF);
        printf("Time: %ld\r\n", HAL_GetTick() - start);
      }
      else /* if(audio_rec_buffer_state == BUFFER_OFFSET_FULL)*/
      {
        uint32_t start = HAL_GetTick();
        res = Feature_Export(MelData, (int16_t *)RecordBuffer, BUFFER_OFFSET_FULL);
        printf("Time: %ld\r\n", HAL_GetTick() - start);
      }
    }
    if (res == FEATURE_EXPORT_OK)
    {
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 70, (uint8_t *)"Feature Export OK", CENTER_MODE);
      // BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW);
      WriteBufferFile_F32(MelData, 30 * 32, "mel_data.txt");
      while (1)
        ;
    }

    // Wait next data
    audio_rec_buffer_state = BUFFER_OFFSET_NONE;
  }
}

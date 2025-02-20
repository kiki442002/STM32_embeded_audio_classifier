

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

/* Variables ---------------------------------------------------------------------*/
volatile uint16_t RecordBuffer[STEREO_RECORD_BUFFER_SIZE] = {0};
volatile float32_t MelData[30 * 32] = {0};
volatile uint16_t sd_wav_cache[STEREO_RECORD_BUFFER_SIZE];
float32_t ia_input[32 * 30];
float32_t ia_output[4];
const char *labels[4] = {"Pluie", "Pas", "Vent", "Voiture"};

int32_t Scratch[SCRATCH_BUFF_SIZE];
uint8_t audio_rec_buffer_state;
App_HandleTypeDef hApp;
volatile uint8_t feature_export_status = FEATURE_EXPORT_PROGRESS;

/* END Variables -----------------------------------------------------------------*/

/* Private function prototypes ---------------------------------------------------*/
void SystemClock_Config(void);
/* END Private function prototypes -----------------------------------------------*/

int main(void)
{

  uint8_t lcd_status = LCD_OK;
  hApp.luminosity = 50;
  hApp.volume = 50;
  hApp.IA_activation = IA_DESACTIVATE;
  hApp.record_activation = RECORD_DESACTIVATE;
  hApp.play_activation = PLAY_DESACTIVATE;
  hApp.output_activation = OUTPUT_DESACTIVATE;
  hApp.SD_state = SD_PRESENT;

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
  MX_X_CUBE_AI_Init(ia_input, ia_output);
  Feature_Export_Init();
  BSP_LED_Init(LED1);

  // LCD Initialisation
  lcd_status = BSP_LCD_Init();
  while (lcd_status != LCD_OK)
    ;
  BSP_LCD_SetBrightness(hApp.luminosity);
  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  // BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Hello, world!", CENTER_MODE);

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
  BSP_AUDIO_OUT_SetVolume(hApp.volume);

  /* END MCU Configuration---------------------------------------------------*/

  printf("\rHello, world!\n\r");
  // HAL_Delay(1000);

  // Show Interface
  print_Menu_Interface();
  Draw_Output_Button(LCD_COLOR_GRAY);
  Draw_SD_Button(LCD_COLOR_GRAY);
  Draw_AI_Button(LCD_COLOR_GRAY);
  Draw_volume(hApp.volume);
  Draw_luminosity(hApp.luminosity);
  // Usefull variables
  audio_rec_buffer_state = BUFFER_OFFSET_NONE;

  // Start Recording
  if (BSP_AUDIO_IN_Record((uint16_t *)&RecordBuffer[0], STEREO_RECORD_BUFFER_SIZE) != AUDIO_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Error: AUDIO IN RECORD", CENTER_MODE);
    Error_Handler();
  }

  BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
  // Wait first data
  while (audio_rec_buffer_state == BUFFER_OFFSET_NONE)
    ;

  /* Play the recorded buffer */
  if (BSP_AUDIO_OUT_Play((uint16_t *)&RecordBuffer[0], STEREO_RECORD_BUFFER_SIZE * 2) != AUDIO_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Error: AUDIO OUT PLAY", CENTER_MODE);
    Error_Handler();
  }
  BSP_AUDIO_IN_Pause();
  BSP_AUDIO_OUT_Pause();

  //  Infinite Loop
  while (1)
  {

    if (hApp.record_state == RECORD_ACTIVATE && audio_rec_buffer_state == BUFFER_OFFSET_FULL && hApp.record_activation != RECORD_ACTIVATE_END)
    {

      WriteWAVFile((uint8_t *)sd_wav_cache, STEREO_RECORD_BUFFER_SIZE * 2, CONTINUE_WAV_FILE);
      audio_rec_buffer_state = BUFFER_OFFSET_NONE;
    }
    else if (hApp.record_state == RECORD_ACTIVATE && audio_rec_buffer_state != BUFFER_OFFSET_NONE && hApp.record_activation == RECORD_ACTIVATE_END)
    {

      WriteWAVFile((uint8_t *)sd_wav_cache, STEREO_RECORD_BUFFER_SIZE * 2, END_WAV_FILE);
      audio_rec_buffer_state = BUFFER_OFFSET_NONE;
      hApp.record_state = RECORD_DESACTIVATE;
      hApp.record_activation = RECORD_DESACTIVATE;
      printf("End recording\n\r");
    }
    else if (hApp.record_activation == RECORD_ACTIVATE && hApp.record_state == RECORD_DESACTIVATE)
    {
      BSP_SD_Init();
      FRESULT res = OpenWavFile(STEREO_WAV);
      hApp.record_state = RECORD_ACTIVATE;
      printf("Recording %d\n\r", res);
    }

    if (feature_export_status == FEATURE_EXPORT_OK && hApp.IA_activation == IA_ACTIVATE)
    {
      transpose_matrix((float32_t *)MelData, ia_input, 32, 30);
      MX_X_CUBE_AI_Process();
      char tmp_msg[30];
      max_output max_val = max(ia_output, 4);
      sprintf(tmp_msg, "      %s : %d.%02d %%       ", labels[max_val.arg], (int)(max_val.max * 100), ((int)(max_val.max * 10000) % 100));
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 65, (uint8_t *)tmp_msg, CENTER_MODE);

      feature_export_status = FEATURE_EXPORT_PROGRESS;
    }
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == TS_INT_PIN)
  {
    touchscreen_Handle();
  }
  else if (GPIO_Pin == SD_DETECT_PIN)
  {
    if (BSP_SD_IsDetected() == SD_PRESENT)
    {
      if (hApp.play_activation)
        Draw_SD_Button(LCD_COLOR_WHITE);
      hApp.SD_state = SD_APP_PRESENT;
    }
    else
    {
      if (hApp.play_activation)
        Draw_SD_Button(LCD_COLOR_GRAY);
      hApp.SD_state = SD_APP_NOT_PRESENT;
    }
  }
}

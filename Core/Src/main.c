/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dsihost.h"
#include "fatfs.h"
#include "ltdc.h"
#include "quadspi.h"
#include "rtc.h"
#include "sai.h"
#include "sdmmc.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../Drivers/BSP/STM32F769I-Discovery/stm32f769i_discovery.h"
#include "../../Drivers/BSP/STM32F769I-Discovery/stm32f769i_discovery_audio.h"
#include "../../Drivers/BSP/STM32F769I-Discovery/stm32f769i_discovery_lcd.h"
#include "filtrage.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF = 1,
  BUFFER_OFFSET_FULL = 2,
} BUFFER_StateTypeDef;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define SCRATCH_BUFF_SIZE 512
#define RECORD_BUFFER_SIZE 4096

uint16_t RecordBuffer[RECORD_BUFFER_SIZE];
uint16_t PlaybackBuffer[RECORD_BUFFER_SIZE / 2];
float32_t FFTBuffer[RECORD_BUFFER_SIZE / 2];

int32_t Scratch[SCRATCH_BUFF_SIZE];
uint32_t audio_rec_buffer_state;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */
  uint8_t lcd_status = LCD_OK;
  uint32_t audio_loop_back_init = RESET;
  /* USER CODE END 1 */

  /* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  // PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  // MX_CRC_Init();
  // MX_FMC_Init();
  // MX_QUADSPI_Init();
  // MX_RTC_Init();
  // MX_SDMMC2_SD_Init();
  MX_USART1_UART_Init();
  // MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  // ! ||--------------------------------------------------------------------------------||
  // ! ||                            Configuration de l'écran                            ||
  // ! ||--------------------------------------------------------------------------------||
  lcd_status = BSP_LCD_Init();
  while (lcd_status != LCD_OK)
    ;
  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Hello, world!", CENTER_MODE);

  // ! ||--------------------------------------------------------------------------------||
  // ! ||                                Configuration LED                               ||
  // ! ||--------------------------------------------------------------------------------||
  BSP_LED_Init(LED1);

  // ! ||--------------------------------------------------------------------------------||
  // ! ||                               Configuration Audio                              ||
  // ! ||--------------------------------------------------------------------------------||
  if (BSP_AUDIO_IN_Init(BSP_AUDIO_FREQUENCY_16K, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR) != AUDIO_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Error: AUDIO IN initialization failed", CENTER_MODE);
    Error_Handler();
  }

  printf("\rHello, world!\n\r");
  HAL_Delay(1000);
  if (BSP_AUDIO_IN_AllocScratch(Scratch, SCRATCH_BUFF_SIZE) != AUDIO_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Error: AUDIO IN Scratch", CENTER_MODE);
    Error_Handler();
  }
  if (BSP_AUDIO_IN_Record((uint16_t *)&RecordBuffer[0], RECORD_BUFFER_SIZE) != AUDIO_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Error: AUDIO IN RECORD", CENTER_MODE);
    Error_Handler();
  }
  audio_rec_buffer_state = BUFFER_OFFSET_NONE;
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 - 20, (uint8_t *)"Enregistrement Audio", CENTER_MODE);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* 1st or 2nd half of the record buffer ready for being copied
    to the playbakc buffer */
    if (audio_rec_buffer_state != BUFFER_OFFSET_NONE)
    {

      /* Copy half of the record buffer to the playback buffer */
      if (audio_rec_buffer_state == BUFFER_OFFSET_HALF)
      {
        Hamming_window(&PlaybackBuffer[0], &RecordBuffer[0], RECORD_BUFFER_SIZE / 4, STEREO);
        if (audio_loop_back_init == RESET)
        {
          /* Initialize the audio device*/
          if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE,
                                 100,
                                 BSP_AUDIO_FREQUENCY_16K) != AUDIO_OK)
          {
            BSP_LCD_SetTextColor(LCD_COLOR_RED);
            BSP_LCD_Clear(LCD_COLOR_WHITE);
            BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Error: AUDIO OUT INIT", CENTER_MODE);
            Error_Handler();
          }

          // BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
          BSP_AUDIO_OUT_SetAudioFrameSlot_MONO();

          /* Play the recorded buffer */
          BSP_AUDIO_OUT_Play((uint16_t *)&PlaybackBuffer[0], RECORD_BUFFER_SIZE);

          /* Audio device is initialized only once */
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 20, (uint8_t *)"Retour Active", CENTER_MODE);
          audio_loop_back_init = SET;
        }
      }
      else /* if(audio_rec_buffer_state == BUFFER_OFFSET_FULL)*/
      {
        Hamming_window(&PlaybackBuffer[RECORD_BUFFER_SIZE / 4], &RecordBuffer[RECORD_BUFFER_SIZE / 2], RECORD_BUFFER_SIZE / 4, STEREO);
        //  Affichage du buffer deans les log
        // for (int i = 0; i < RECORD_BUFFER_SIZE / 2; i++)
        // {
        //   PlaybackBuffer[i] = (float)PlaybackBuffer[i];
        //   printf("%d\n\r", PlaybackBuffer[i]);
        // }
      }

      /* Wait for next data */
      audio_rec_buffer_state = BUFFER_OFFSET_NONE;
    }
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
   */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
   */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1);
}

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
   */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC | RCC_PERIPHCLK_SAI1 | RCC_PERIPHCLK_SAI2 | RCC_PERIPHCLK_SDMMC2 | RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 3;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV4;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLLSAI;
  PeriphClkInitStruct.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLSAI;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLLSAIP;
  PeriphClkInitStruct.Sdmmc2ClockSelection = RCC_SDMMC2CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  audio_rec_buffer_state = BUFFER_OFFSET_FULL;
}
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  audio_rec_buffer_state = BUFFER_OFFSET_HALF;
}
void BSP_AUDIO_IN_Error_CallBack(void)
{
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2, (uint8_t *)"Error: AUDIO IN", CENTER_MODE);
  Error_Handler();
}

int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

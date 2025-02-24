/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file   fatfs.h
 * @brief  Header for fatfs applications
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __fatfs_H
#define __fatfs_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h" /* defines SD_Driver as external */

  /* USER CODE BEGIN Includes */

  /* USER CODE END Includes */
#define CONTINUE_WAV_FILE 0
#define END_WAV_FILE 1

  extern uint8_t retSD;  /* Return value for SD */
  extern char SDPath[4]; /* SD logical drive path */
  extern FATFS SDFatFS;  /* File system object for SD logical drive */
  extern FIL SDFile;     /* File object for SD */

  void MX_FATFS_Init(void);

  /* USER CODE BEGIN Prototypes */
  void WriteToSDCard(void);
  FRESULT WriteWAVFile(const uint8_t *audioData, uint32_t dataSize, uint8_t finalize);
  FRESULT GenerateUniqueFilename(char *filename);
  FRESULT OpenWavFile();
  FRESULT WriteBufferFile_F32(float *pIn, uint32_t size, char *filename);
  FRESULT WriteBufferFile_INT16(int16_t *pIn, uint32_t size, char *filename);
/* USER CODE END Prototypes */
#ifdef __cplusplus
}
#endif
#endif /*__fatfs_H */

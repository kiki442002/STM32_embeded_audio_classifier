/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file   fatfs.c
 * @brief  Code for fatfs applications
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
#include "fatfs.h"
#include "main.h"

uint8_t retSD;  /* Return value for SD */
char SDPath[4]; /* SD logical drive path */
FATFS SDFatFS;  /* File system object for SD logical drive */
FIL SDFile;     /* File object for SD */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  if (BSP_SD_Init() == MSD_ERROR)
  {
    printf("\rError initializing SD card\n\r");
  }
  printf("\rSD card initialized\n\r");

  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
  printf("\rFATFS Link Driver: %d\n\r", retSD);
  /* USER CODE BEGIN Init */
  /* additional user code for init */
  WriteToSDCard();
  /* USER CODE END Init */
}

/**
 * @brief  Gets Time from RTC
 * @param  None
 * @retval Time in DWORD
 */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
void WriteToSDCard(void)
{
  FRESULT res;                          // Variable pour stocker les résultats des fonctions FATFS
  UINT bytesWritten;                    // Variable pour stocker le nombre de bytes écrits
  const char *data = "Hello, SD Card!"; // Données à écrire
  printf("Writing to file\n\r");
  // Initialiser le système de fichiers
  retSD = f_mount(&SDFatFS, SDPath, 1);
  if (retSD != FR_OK)
  {
    // Erreur lors du montage du système de fichiers

    // Affiche le code d'erreur
    printf("Error mounting file system %d %d\n\r", retSD, SDFatFS.fsize);
    return;
  }

  // Ouvrir un fichier pour écrire (créer un nouveau fichier ou écraser un fichier existant)
  res = f_open(&SDFile, "test.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if (res != FR_OK)
  {
    // Erreur lors de l'ouverture du fichier
    f_mount(0, SDPath, 1); // Démonter le système de fichiers
    printf("Error opening file %d\n\r", res);
    return;
  }

  // Écrire des données dans le fichier
  res = f_write(&SDFile, data, strlen(data), &bytesWritten);
  if (res != FR_OK || bytesWritten != strlen(data))
  {
    // Erreur lors de l'écriture des données
    f_close(&SDFile);      // Fermer le fichier
    f_mount(0, SDPath, 1); // Démonter le système de fichiers
    printf("Error writing to file %d\n\r", res);
    return;
  }

  // Fermer le fichier
  f_close(&SDFile);

  // Démonter le système de fichiers
  f_mount(0, SDPath, 1);
  printf("Data written to file\n");
}
/* USER CODE END Application */

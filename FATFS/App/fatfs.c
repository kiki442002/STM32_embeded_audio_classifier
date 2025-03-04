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

#define MAX_FILENAME_LENGTH 14

#pragma pack(push, 1)
typedef struct
{
  char riff[4];           // "RIFF"
  uint32_t chunkSize;     // Taille du fichier - 8
  char wave[4];           // "WAVE"
  char fmt[4];            // "fmt "
  uint32_t subchunk1Size; // 16 for PCM
  uint16_t audioFormat;   // PCM = 1
  uint16_t numChannels;   // Nombre de canaux
  uint32_t sampleRate;    // Taux d'échantillonnage
  uint32_t byteRate;      // sampleRate * numChannels * bitsPerSample/8
  uint16_t blockAlign;    // numChannels * bitsPerSample/8
  uint16_t bitsPerSample; // Bits par échantillon
  char data[4];           // "data"
  uint32_t dataSize;      // Taille des données audio
} WAVHeader;
#pragma pack(pop)

uint8_t retSD;  /* Return value for SD */
char SDPath[4]; /* SD logical drive path */
FATFS SDFatFS;  /* File system object for SD logical drive */
FIL SDFile;     /* File object for SD */

// Variables pour le fichier WAV
FIL file;
uint32_t totalDataSize = 0;
char filename[] = "samp_001.wav";

WAVHeader headerStereo = {
    .riff = "RIFF",
    .chunkSize = 0, // Sera calculé plus tard
    .wave = "WAVE",
    .fmt = "fmt ",
    .subchunk1Size = 16,
    .audioFormat = 1,    // PCM
    .numChannels = 2,    // Sera défini plus tard
    .sampleRate = 16000, // Sera défini plus tard
    .byteRate = 64000,   // Sera calculé plus tard
    .blockAlign = 4,     // Sera calculé plus tard
    .bitsPerSample = 16, // Sera défini plus tard
    .data = "data",
    .dataSize = 0 // Sera défini plus tard
};

WAVHeader headerMono = {
    .riff = "RIFF",
    .chunkSize = 0, // Sera calculé plus tard
    .wave = "WAVE",
    .fmt = "fmt ",
    .subchunk1Size = 16,
    .audioFormat = 1,    // PCM
    .numChannels = 1,    // Sera défini plus tard
    .sampleRate = 16000, // Sera défini plus tard
    .byteRate = 32000,   // Sera calculé plus tard
    .blockAlign = 2,     // Sera calculé plus tard
    .bitsPerSample = 16, // Sera défini plus tard
    .data = "data",
    .dataSize = 0 // Sera défini plus tard
};

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  BSP_SD_ITConfig();
  if (BSP_SD_Init() == MSD_ERROR_SD_NOT_PRESENT)
  {
    hApp.SD_state = SD_NOT_PRESENT;
  }

  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
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
void WriteToSDCard()
{
  FRESULT res;                          // Variable pour stocker les résultats des fonctions FATFS
  UINT bytesWritten;                    // Variable pour stocker le nombre de bytes écrits
  const char *data = "Hello, SD Card!"; // Données à écrire
  printf("Writing to file\n\r");
  // Initialiser le système de fichiers
  retSD = f_mount(&SDFatFS, SDPath, 1);
  if (retSD != FR_OK)
  {
    // Affiche le code d'erreur
    printf("Error mounting file system %d %ld\n\r", retSD, SDFatFS.fsize);
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

FRESULT WriteBufferFile_F32(float *pIn, uint32_t size, char *filename)
{
  FRESULT res = FR_OK; // Variable pour stocker les résultats des fonctions FATFS

  retSD = f_mount(&SDFatFS, SDPath, 1);
  if (retSD != FR_OK)
  {
    return retSD;
  }

  res = f_open(&SDFile, filename, FA_CREATE_ALWAYS | FA_WRITE);
  if (res != FR_OK)
  {
    // Erreur lors de l'ouverture du fichier
    f_mount(0, SDPath, 1); // Démonter le système de fichiers
    return res;
  }

  // Écrire les données en format texte avec une valeur par ligne
  for (uint32_t i = 0; i < size; i++)
  {
    // Convertir la valeur flottante en chaîne de caractères
    if (pIn[i] < 0)
    {
      res = f_printf(&SDFile, "-%d.%06d\n", (int)abs(pIn[i]), abs((int)((pIn[i] - (int)pIn[i]) * 1000000)));
    }
    else
    {
      res = f_printf(&SDFile, "%d.%06d\n", (int)pIn[i], abs((int)((pIn[i] - (int)pIn[i]) * 1000000)));
    }
    if (res < 0 || res == 255)
    {
      printf("Erreur lors de l'écriture des données: %d\n", res);
      f_close(&SDFile);      // Fermer le fichier
      f_mount(0, SDPath, 1); // Démonter le système de fichiers
      return FR_DENIED;
    }
  }
  // Fermer le fichier
  f_close(&SDFile);

  // Démonter le système de fichiers
  f_mount(0, SDPath, 1);
  printf("Data written to file\r\n");
  return FR_OK;
}

FRESULT WriteBufferFile_INT16(int16_t *pIn, uint32_t size, char *filename)
{
  FRESULT res = FR_OK; // Variable pour stocker les résultats des fonctions FATFS

  retSD = f_mount(&SDFatFS, SDPath, 1);
  if (retSD != FR_OK)
  {
    return retSD;
  }

  res = f_open(&SDFile, filename, FA_CREATE_ALWAYS | FA_WRITE);
  if (res != FR_OK)
  {
    // Erreur lors de l'ouverture du fichier
    f_mount(0, SDPath, 1); // Démonter le système de fichiers
    return res;
  }

  // Écrire les données en format texte avec une valeur par ligne
  for (uint32_t i = 0; i < size; i++)
  {
    // Convertir la valeur flottante en chaîne de caractères
    res = f_printf(&SDFile, "%d\r\n", pIn[i]);
    if (res < 0 || res == 255)
    {
      printf("Erreur lors de l'écriture des données: %d\n", res);
      f_close(&SDFile);      // Fermer le fichier
      f_mount(0, SDPath, 1); // Démonter le système de fichiers
      return FR_DENIED;
    }
  }
  // Fermer le fichier
  f_close(&SDFile);

  // Démonter le système de fichiers
  f_mount(0, SDPath, 1);
  printf("Data written to file\r\n");
  return FR_OK;
}

FRESULT OpenWavFile(uint8_t type)
{
  FRESULT res = FR_OK;
  UINT bytesWritten;
  // GenerateUniqueFilename(filename);
  //  Initialiser le système de fichiers
  res = f_mount(&SDFatFS, SDPath, 1);
  if (res != FR_OK)
  {
    return res;
  }

  res = GenerateUniqueFilename(filename);
  if (res != FR_OK)
  {
    f_mount(0, SDPath, 1); // Démonter le système de fichiers
    return res;
  }

  // Ouvrir un fichier pour écrire
  res = f_open(&file, filename, FA_CREATE_ALWAYS | FA_WRITE);
  if (res != FR_OK)
  {
    f_mount(0, SDPath, 1); // Démonter le système de fichiers
    return res;
  }

  // Écrire un en-tête vide pour réserver de l'espace
  if (type == MONO_WAV)
    res = f_write(&file, &headerMono, sizeof(WAVHeader), &bytesWritten);
  else
    res = f_write(&file, &headerStereo, sizeof(WAVHeader), &bytesWritten);
  if (res != FR_OK || bytesWritten != sizeof(WAVHeader))
  {
    f_close(&file);
    f_mount(0, SDPath, 1); // Démonter le système de fichiers
    return res;
  }
  return res;
}
FRESULT WriteWAVFile(const uint8_t *audioData, uint32_t dataSize, uint8_t finalize)
{

  FRESULT res = FR_OK;
  UINT bytesWritten;

  // Écrire les données audio
  res = f_write(&file, audioData, dataSize, &bytesWritten);
  if (res != FR_OK || bytesWritten != dataSize)
  {
    f_close(&file);
    f_mount(0, SDPath, 1); // Démonter le système de fichiers
    return res;
  }

  totalDataSize += dataSize;

  if (finalize)
  {
    //  Calculer les nouvelles valeurs de chunkSize et dataSize
    uint32_t chunkSize = 36 + totalDataSize;
    uint32_t dataSize = totalDataSize;

    // Déplacer le pointeur de fichier à la position de chunkSize
    f_lseek(&file, offsetof(WAVHeader, chunkSize));
    res = f_write(&file, &chunkSize, sizeof(chunkSize), &bytesWritten);
    if (res != FR_OK || bytesWritten != sizeof(chunkSize))
    {
      return res;
    }

    // Déplacer le pointeur de fichier à la position de dataSize
    f_lseek(&file, offsetof(WAVHeader, dataSize));
    res = f_write(&file, &dataSize, sizeof(dataSize), &bytesWritten);
    if (res != FR_OK || bytesWritten != sizeof(dataSize))
    {
      return res;
    }
    // Fermer le fichier
    f_close(&file);
    f_mount(0, SDPath, 1); // Démonter le système de fichiers
    totalDataSize = 0;
  }
  return res;
}

FRESULT GenerateUniqueFilename(char *filename)
{
  FRESULT res = FR_OK;
  DIR dir;
  FILINFO fno;
  int maxNumber = 0;

  // Ouvrir le répertoire racine
  res = f_opendir(&dir, "/");
  if (res != FR_OK)
  {
    return res;
  }

  // Parcourir les fichiers dans le répertoire
  while (1)
  {

    res = f_readdir(&dir, &fno);
    if (fno.fname[0] == 'S' && fno.fname[1] == 'A')
    {
      maxNumber++;
    }
    if (res != FR_OK)
    {
      f_closedir(&dir);
      return res;
    }
    if (fno.fname[0] == 0)
      break;
  }

  // Fermer le répertoire
  f_closedir(&dir);

  // Générer le prochain nom de fichier unique
  snprintf(filename, MAX_FILENAME_LENGTH, "%s%03d%s", "samp_", maxNumber, ".wav");
  return res;
}
/* USER CODE END Application */

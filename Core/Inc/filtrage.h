#include "main.h"
#include "../../Drivers/CMSIS/DSP/Include/arm_math.h"

#define STEREO_TO_MONO_OK ((uint8_t)1)
#define STEREO_TO_MONO_ERROR ((uint8_t)0)
#define HAMMING_FILTER_OK ((uint8_t)1)
#define HAMMING_FILTER_ERROR ((uint8_t)0)
#define HANNING_FILTER_OK ((uint8_t)1)
#define HANNING_FILTER_ERROR ((uint8_t)0)
#define MONO ((uint8_t)0)
#define STEREO ((uint8_t)1)
#define FILTER_CALCULATION_ERROR ((uint8_t)0)
#define FILTER_CALCULATION_OK ((uint8_t)1)
#define FILTRAGE_SIZE 1024
#define BUFFER_FULL ((uint8_t)2)
#define BUFFER_HALF ((uint8_t)1)
#define BUFFER_HALF_FIRST ((uint8_t)1)
#define BUFFER_HALF_NONE ((uint8_t)0)

#define FEATURE_EXPORT_PROGRESS ((uint8_t)1)
#define FEATURE_EXPORT_OK ((uint8_t)2)
#define FEATURE_EXPORT_ERROR ((uint8_t)0)

#define AUDIO_RECORD ((uint8_t)1)
#define AUDIO_NO_RECORD ((uint8_t)0)

#define PSD_init(size) FFT_init(size)

uint8_t StereoToMono(int16_t *pOut, int16_t *pIn, uint32_t size);
uint8_t Hamming_window(float32_t *pOut, int16_t *pIn, uint32_t size, uint8_t signal_input_type);
uint8_t Hanning_window(float32_t *pOut, int16_t *pIn, uint32_t size, uint8_t signal_input_type);
uint8_t FFT_Calculation(float32_t *pOut, float32_t *pIn);
uint8_t FFT_init(uint32_t size);
uint8_t DSE_Calculation(float32_t *pOut, float32_t *pIn);
void print_mel_filters(float32_t *pOut, int n_mels);
uint8_t MEL_Calculation(float32_t *pOut, float32_t *pIn);
uint8_t ZScore_Calculation(float32_t *pIn, uint32_t size);
uint8_t Feature_Export(float32_t *pOut, int16_t *pIn, uint8_t bufferState, uint8_t audio_record);
uint8_t Feature_Export_Init();
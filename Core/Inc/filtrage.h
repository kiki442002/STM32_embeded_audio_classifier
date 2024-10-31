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
#define FFT_CALCULATION_ERROR ((uint8_t)0)
#define FFT_CALCULATION_OK ((uint8_t)1)

uint8_t StereoToMono(int16_t *pOut, int16_t *pIn, uint32_t size);
uint8_t Hamming_window(float32_t *pOut, int16_t *pIn, uint32_t size, uint8_t signal_input_type);
uint8_t FFT_Calculation(float32_t *pOut, float32_t *pIn, uint32_t size);
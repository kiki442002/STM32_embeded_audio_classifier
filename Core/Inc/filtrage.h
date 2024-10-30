#include "main.h"

#define STEREO_TO_MONO_OK ((uint8_t)1)
#define STEREO_TO_MONO_ERROR ((uint8_t)0)
#define HAMMING_FILTER_OK ((uint8_t)1)
#define HAMMING_FILTER_ERROR ((uint8_t)0)
#define MONO ((uint8_t)0)
#define STEREO ((uint8_t)1)

uint8_t StereoToMono(int16_t *pOut, int16_t *pIn, uint32_t size);
uint8_t Hamming_window(int16_t *pOut, int16_t *pIn, uint32_t size, uint8_t signal_input_type);
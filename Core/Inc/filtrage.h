#include "main.h"

#define STEREO_TO_MONO_OK ((uint8_t)0)
#define STEREO_TO_MONO_ERROR ((uint8_t)1)

uint8_t StereoToMono(int16_t *pIn, int16_t *pOut, uint32_t size);
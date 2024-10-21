
#include "filtrage.h"
#include "../../Drivers/CMSIS/DSP/Include/arm_math.h"
#include "fir_coef.h"

arm_fir_instance_f32 hammingFilter;
uint32_t blockSize;
float *firHammingState = NULL;

uint8_t StereoToMono(int16_t *pOut, int16_t *pIn, uint32_t size)
{
    uint32_t i = 0;

    if (pIn == NULL || pOut == NULL || size == 0)
    {
        return STEREO_TO_MONO_ERROR;
    }

    for (i = 0; i < size; i += 2)
    {
        pOut[i / 2] = (pIn[i] + pIn[i + 1]) / 2;
    }

    return STEREO_TO_MONO_OK;
}

uint8_t Hamming_filter_Init(uint32_t size)
{
    if (size == 0)
    {
        return HAMMING_FILTER_ERROR;
    }
    blockSize = size;
    // Allouer de la mémoire pour le tableau
    firHammingState = (float *)malloc((2 * size - 1) * sizeof(float));
    if (firHammingState == NULL)
    {
        return HAMMING_FILTER_ERROR; // Erreur d'allocation de mémoire
    }
    arm_fir_init_f32(&hammingFilter, size, hammingWindow, &firHammingState[0], size);
    return HAMMING_FILTER_OK;
}

uint8_t Hamming_filter_DeInit(void)
{
    if (firHammingState != NULL)
    {
        free(firHammingState);
        firHammingState = NULL;
    }
    return HAMMING_FILTER_OK;
}

uint8_t Hamming_filter(int16_t *pOut, int16_t *pIn, uint8_t signal_input)
{
    uint32_t i = 0;
    float pIn_float[blockSize];
    float pOut_float[blockSize];

    if (pIn == NULL || pOut == NULL)
    {
        return HAMMING_FILTER_ERROR;
    }

    if (signal_input == STEREO)
    {
        StereoToMono(pIn, pIn, blockSize);
    }

    // convert all data into float
    for (i = 0; i < blockSize; i++)
    {
        pIn_float[i] = (float)pIn[i];
    }
    // apply the filter
    arm_fir_f32(&hammingFilter, pIn_float, pOut_float, blockSize);

    // convert all data into int16_t
    for (i = 0; i < blockSize; i++)
    {
        pOut[i] = (int16_t)pOut_float[i];
    }
}
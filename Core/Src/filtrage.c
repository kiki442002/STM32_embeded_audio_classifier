
#include "filtrage.h"
#include "../../Drivers/CMSIS/DSP/Include/arm_math.h"
#include "fir_coef.h"

arm_fir_instance_f32 hammingFilter;
uint32_t blockSize;
float *firHammingState = NULL;
int16_t *tmpSteroBff;
uint8_t signal_input;

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

uint8_t Hamming_filter_Init(uint32_t size, uint8_t signal_input_type)
{
    if (size == 0)
    {
        return HAMMING_FILTER_ERROR;
    }
    blockSize = size;
    signal_input = signal_input_type;
    // Allouer de la mémoire pour le tableau
    firHammingState = (float *)malloc((2 * size - 1) * sizeof(float));
    if (firHammingState == NULL)
    {
        return HAMMING_FILTER_ERROR; // Erreur d'allocation de mémoire
    }
    arm_fir_init_f32(&hammingFilter, size, hammingWindow, &firHammingState[0], size);
    if (signal_input == STEREO)
    {
        tmpSteroBff = (int16_t *)malloc(size * sizeof(int16_t));
        if (tmpSteroBff == NULL)
        {
            return HAMMING_FILTER_ERROR;
        }
    }
    return HAMMING_FILTER_OK;
}

uint8_t Hamming_filter_DeInit(void)
{
    if (firHammingState != NULL)
    {
        free(firHammingState);
        firHammingState = NULL;
    }
    if (tmpSteroBff != NULL)
    {
        free(tmpSteroBff);
        tmpSteroBff = NULL;
    }
    return HAMMING_FILTER_OK;
}

uint8_t Hamming_filter(int16_t *pOut, int16_t *pIn)
{

    if (pIn == NULL || pOut == NULL)
    {
        return HAMMING_FILTER_ERROR;
    }
    int16_t *pIn_tmp = pIn;

    if (signal_input == STEREO)
    {
        StereoToMono(tmpSteroBff, pIn, blockSize * 2);
        pIn_tmp = tmpSteroBff;
    }
    for (uint32_t i = 0; i < blockSize; i++)
    {
        pOut[i] = (int16_t)(hammingWindow[i] * (float)pIn_tmp[i]);
    }
    return HAMMING_FILTER_OK;
}
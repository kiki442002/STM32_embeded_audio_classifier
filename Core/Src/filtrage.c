
#include "filtrage.h"
#include "fir_coef.h"

uint8_t StereoToMono(int16_t *pOut, int16_t *pIn, uint32_t size)
{
    uint32_t i = 0;

    if (pIn == NULL || pOut == NULL || size == 0)
    {
        return STEREO_TO_MONO_ERROR;
    }

    for (i = 0; i < size; i += 2)
    {
        pOut[i / 2] = pIn[i];
    }

    return STEREO_TO_MONO_OK;
}

uint8_t Hamming_window(float32_t *pOut, int16_t *pIn, uint32_t size, uint8_t signal_input_type)
{

    if (pIn == NULL || pOut == NULL)
    {
        return HAMMING_FILTER_ERROR;
    }
    int16_t *pIn_tmp = pIn;

    if (signal_input_type == STEREO)
    {
        int16_t tmpSteroBff[size];
        StereoToMono(tmpSteroBff, pIn, size * 2);
        pIn_tmp = tmpSteroBff;
    }
    for (uint32_t i = 0; i < size; i++)
    {
        pOut[i] = (hammingWindow[i] * (float)pIn_tmp[i]);
    }
    return HAMMING_FILTER_OK;
}

uint8_t FFT_Calculation(float32_t *pOut, float32_t *pIn, uint32_t size)
{
    if (pIn == NULL || pOut == NULL || size < 16 || size > 4096 || size % 2 != 0)
    {
        return FFT_CALCULATION_ERROR;
    }

    arm_rfft_fast_instance_f32 S;
    arm_rfft_fast_init_f32(&S, size);
    arm_rfft_fast_f32(&S, pIn, pOut, 0);

    return FFT_CALCULATION_OK;
}
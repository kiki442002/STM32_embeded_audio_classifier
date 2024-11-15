
#include "filtrage.h"
#include "hammingWindow.h"
#include "hanningWindow.h"

arm_rfft_fast_instance_f32 FFT_struct;

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
uint8_t Hanning_window(float32_t *pOut, int16_t *pIn, uint32_t size, uint8_t signal_input_type)
{

    if (pIn == NULL || pOut == NULL)
    {
        return HANNING_FILTER_ERROR;
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
        pOut[i] = (hanningWindow[i] * (float)pIn_tmp[i]);
    }
    return HANNING_FILTER_OK;
}

uint8_t FFT_init(uint32_t size)
{
    if (arm_rfft_fast_init_f32(&FFT_struct, size) != ARM_MATH_SUCCESS)
    {
        return FFT_CALCULATION_ERROR;
    }
    return FFT_CALCULATION_OK;
}

uint8_t FFT_Calculation(float32_t *pOut, float32_t *pIn)
{
    if (pIn == NULL || pOut == NULL)
    {
        return FFT_CALCULATION_ERROR;
    }

    arm_rfft_fast_f32(&FFT_struct, pIn, pOut, 0);

    return FFT_CALCULATION_OK;
}

uint8_t PSD_Calculation(float32_t *pOut, float32_t *pIn, uint32_t size)
{
    // Calculer la FFT
    arm_rfft_fast_f32(&FFT_struct, pIn, pOut, 0);

    // Calculer le module au carré des coefficients de la FFT
    float32_t magSquared[size / 2];
    arm_cmplx_mag_squared_f32(pOut + 2, magSquared, size / 2 - 1);

    // Traiter les deux premières valeurs spéciales
    magSquared[0] = pOut[0] * pOut[0];            // Coefficient DC
    magSquared[size / 2 - 1] = pOut[1] * pOut[1]; // Coefficient de la fréquence de Nyquist

    // Normaliser les valeurs pour obtenir la densité spectrale de puissance
    for (uint32_t i = 0; i < size / 2; i++)
    {
        pOut[i] = magSquared[i] / size;
    }
}
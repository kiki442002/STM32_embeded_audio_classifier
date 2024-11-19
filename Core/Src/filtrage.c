
#include "filtrage.h"
#include "hammingWindow.h"
#include "hanningWindow.h"
#include "mel_filters.h"

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
        return FILTER_CALCULATION_ERROR;
    }
    return FILTER_CALCULATION_OK;
}

uint8_t FFT_Calculation(float32_t *pOut, float32_t *pIn)
{
    if (pIn == NULL || pOut == NULL)
    {
        return FILTER_CALCULATION_ERROR;
    }

    arm_rfft_fast_f32(&FFT_struct, pIn, pOut, 0);

    return FILTER_CALCULATION_OK;
}

uint8_t DSE_Calculation(float32_t *pOut, float32_t *pIn)
{
    if (pIn == NULL || pOut == NULL)
    {
        return FILTER_CALCULATION_ERROR;
    }
    uint32_t size = FFT_struct.fftLenRFFT / 2;

    // Calculer le module au carré des coefficients de la FFT
    arm_cmplx_mag_squared_f32(pIn + 2, pOut + 1, size - 1);

    // Traiter les deux premières valeurs spéciales
    pOut[0] = pIn[0] * pIn[0];    // Coefficient DC
    pOut[size] = pIn[1] * pIn[1]; // Coefficient de la fréquence de Nyquist

    return FILTER_CALCULATION_OK;
}

uint8_t MEL_Calculation(float32_t *pOut, float32_t *pIn)
{
    if (pIn == NULL || pOut == NULL)
    {
        return FILTER_CALCULATION_ERROR;
    }

    int start_data = 0;
    for (int i = 0; i < N_MELS; i++)
    {
        float sum = 0.0;
        int end_data = mel_filters_num_non_zero[i] + start_data;
        int start_segment = mel_filters_zeros_before[i];
        for (int j = start_data, x = start_segment; j <= end_data; j++, x++)
        {
            sum += pIn[x] * mel_filters_non_zero_data[j];
        }
        start_data = end_data;

        pOut[i] = logf(sum + 1e-10);
    }
    return FILTER_CALCULATION_OK;
}

void print_mel_filters(float32_t *pOut, int n_mels)
{

    int start_data = 0;

    for (int j = 0; j < 512; j++)
    {
        pOut[j] = 1.0;
    }

    for (int i = 0; i < n_mels; i++)
    {
        start_data += mel_filters_num_non_zero[i];
    }
    printf("start_data: %d\n\r", start_data);

    int end_data = mel_filters_num_non_zero[n_mels] + start_data;
    int start_segment = mel_filters_zeros_before[n_mels];

    printf("end_data: %d\n\r", end_data);

    for (int j = 0; j < start_segment; j++)
    {
        pOut[j] = 0;
    }

    for (int j = start_data, x = start_segment; j < end_data; j++, x++)
    {
        pOut[x] = mel_filters_non_zero_data[j];
    }

    for (int j = start_segment + mel_filters_num_non_zero[n_mels]; j < 512; j++)
    {
        pOut[j] = 0;
    }
}
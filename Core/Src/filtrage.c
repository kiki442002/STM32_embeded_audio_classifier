
#include "filtrage.h"
#include "hammingWindow.h"
#include "hanningWindow.h"
#include "mel_filters.h"

arm_rfft_fast_instance_f32 FFT_struct;
int32_t cReadIndex = FILTRAGE_SIZE;
int16_t distIndex = 0;
int16_t rawPCMdata[FILTRAGE_SIZE];
float32_t tmp_buf_1[FILTRAGE_SIZE];
float32_t tmp_buf_2[FILTRAGE_SIZE];

uint8_t buffer_run = BUFFER_HALF_FIRST;
uint8_t mel_indice = 0;

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
        for (int j = start_data, x = start_segment; j < end_data; j++, x++)
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

uint8_t ZScore_Calculation(float32_t *pIn, uint32_t size)
{
    if (pIn == NULL)
    {
        return FILTER_CALCULATION_ERROR;
    }

    float32_t mean;
    float32_t std;

    arm_mean_f32(pIn, size, &mean);
    arm_std_f32(pIn, size, &std);

    for (int i = 0; i < size; i++)
    {
        pIn[i] = (pIn[i] - mean) / std;
    }

    return FILTER_CALCULATION_OK;
}

uint8_t Feature_Export(float32_t *pOut, int16_t *pIn, uint8_t bufferState)
{
    if (bufferState == BUFFER_HALF)
    {
        if (buffer_run != BUFFER_HALF_FIRST)
        {
            cReadIndex -= FILTRAGE_SIZE;
            arm_circularRead_q15(pIn, STEREO_RECORD_BUFFER_SIZE, &cReadIndex, 2, rawPCMdata, &distIndex, FILTRAGE_SIZE, 1, FILTRAGE_SIZE);

            Hanning_window(tmp_buf_1, rawPCMdata, FILTRAGE_SIZE, MONO);
            FFT_Calculation(tmp_buf_2, tmp_buf_1);
            DSE_Calculation(tmp_buf_1, tmp_buf_2);
            MEL_Calculation(&pOut[mel_indice * N_MELS], tmp_buf_1);
            ZScore_Calculation(&pOut[mel_indice * N_MELS], N_MELS);
            buffer_run = BUFFER_HALF_NONE;
            mel_indice++;
        }
        if (mel_indice == 32)
        {
            mel_indice = 0;
            return FEATURE_EXPORT_OK;
        }
        cReadIndex -= FILTRAGE_SIZE;
        arm_circularRead_q15(pIn, STEREO_RECORD_BUFFER_SIZE, &cReadIndex, 2, rawPCMdata, &distIndex, FILTRAGE_SIZE, 1, FILTRAGE_SIZE);
        Hanning_window(tmp_buf_1, rawPCMdata, FILTRAGE_SIZE, MONO);
        FFT_Calculation(tmp_buf_2, tmp_buf_1);
        DSE_Calculation(tmp_buf_1, tmp_buf_2);
        MEL_Calculation(&pOut[mel_indice * N_MELS], tmp_buf_1);
        ZScore_Calculation(&pOut[mel_indice * N_MELS], N_MELS);
        buffer_run = BUFFER_HALF_NONE;
        mel_indice++;
    }
    else if (bufferState == BUFFER_FULL)
    {
        cReadIndex -= FILTRAGE_SIZE;
        arm_circularRead_q15(pIn, STEREO_RECORD_BUFFER_SIZE, &cReadIndex, 2, rawPCMdata, &distIndex, FILTRAGE_SIZE, 1, FILTRAGE_SIZE);

        Hanning_window(tmp_buf_1, rawPCMdata, FILTRAGE_SIZE, MONO);
        FFT_Calculation(tmp_buf_2, tmp_buf_1);
        DSE_Calculation(tmp_buf_1, tmp_buf_2);
        MEL_Calculation(&pOut[mel_indice * N_MELS], tmp_buf_1);
        ZScore_Calculation(&pOut[mel_indice * N_MELS], N_MELS);
        mel_indice++;
        if (mel_indice == 32)
        {
            mel_indice = 0;
            return FEATURE_EXPORT_OK;
        }
        cReadIndex -= FILTRAGE_SIZE;
        arm_circularRead_q15(pIn, STEREO_RECORD_BUFFER_SIZE, &cReadIndex, 2, rawPCMdata, &distIndex, FILTRAGE_SIZE, 1, FILTRAGE_SIZE);
        Hanning_window(tmp_buf_1, rawPCMdata, FILTRAGE_SIZE, MONO);
        FFT_Calculation(tmp_buf_2, tmp_buf_1);
        DSE_Calculation(tmp_buf_1, tmp_buf_2);
        MEL_Calculation(&pOut[mel_indice * N_MELS], tmp_buf_1);
        ZScore_Calculation(&pOut[mel_indice * N_MELS], N_MELS);
        mel_indice++;
    }

    if (mel_indice == 32)
    {
        mel_indice = 0;
        return FEATURE_EXPORT_OK;
    }

    return FEATURE_EXPORT_PROGRESS;
}

uint8_t
Feature_Export_Init()
{
    return FFT_init(FILTRAGE_SIZE);
}

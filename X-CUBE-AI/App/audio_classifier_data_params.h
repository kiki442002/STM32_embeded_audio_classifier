/**
  ******************************************************************************
  * @file    audio_classifier_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-01-13T16:52:02+0100
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef AUDIO_CLASSIFIER_DATA_PARAMS_H
#define AUDIO_CLASSIFIER_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_AUDIO_CLASSIFIER_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_audio_classifier_data_weights_params[1]))
*/

#define AI_AUDIO_CLASSIFIER_DATA_CONFIG               (NULL)


#define AI_AUDIO_CLASSIFIER_DATA_ACTIVATIONS_SIZES \
  { 19344, }
#define AI_AUDIO_CLASSIFIER_DATA_ACTIVATIONS_SIZE     (19344)
#define AI_AUDIO_CLASSIFIER_DATA_ACTIVATIONS_COUNT    (1)
#define AI_AUDIO_CLASSIFIER_DATA_ACTIVATION_1_SIZE    (19344)



#define AI_AUDIO_CLASSIFIER_DATA_WEIGHTS_SIZES \
  { 152496, }
#define AI_AUDIO_CLASSIFIER_DATA_WEIGHTS_SIZE         (152496)
#define AI_AUDIO_CLASSIFIER_DATA_WEIGHTS_COUNT        (1)
#define AI_AUDIO_CLASSIFIER_DATA_WEIGHT_1_SIZE        (152496)



#define AI_AUDIO_CLASSIFIER_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_audio_classifier_activations_table[1])

extern ai_handle g_audio_classifier_activations_table[1 + 2];



#define AI_AUDIO_CLASSIFIER_DATA_WEIGHTS_TABLE_GET() \
  (&g_audio_classifier_weights_table[1])

extern ai_handle g_audio_classifier_weights_table[1 + 2];


#endif    /* AUDIO_CLASSIFIER_DATA_PARAMS_H */

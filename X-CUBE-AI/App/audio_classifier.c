/**
  ******************************************************************************
  * @file    audio_classifier.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-01-13T19:09:32+0100
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "audio_classifier.h"
#include "audio_classifier_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_audio_classifier
 
#undef AI_AUDIO_CLASSIFIER_MODEL_SIGNATURE
#define AI_AUDIO_CLASSIFIER_MODEL_SIGNATURE     "0xa1b612fd02abb0dd6389601db2ec0f4e"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2025-01-13T19:09:32+0100"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_AUDIO_CLASSIFIER_N_BATCHES
#define AI_AUDIO_CLASSIFIER_N_BATCHES         (1)

static ai_ptr g_audio_classifier_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_audio_classifier_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  input_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 960, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  _conv1_conv1_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3840, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  _conv1_conv1_1_Relu_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3840, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  _conv1_conv1_2_MaxPool_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 960, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  _conv2_conv2_0_Conv_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3840, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  _conv2_conv2_1_Relu_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3840, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  _conv2_conv2_2_MaxPool_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 896, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  _flatten_Flatten_output_0_to_chlast_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 896, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  _fc1_fc1_0_Gemm_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 40, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  _fc1_fc1_1_Relu_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 40, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  _fc2_fc2_0_Gemm_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  _fc2_fc2_1_Relu_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  _fc3_fc3_0_Gemm_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  _fc3_fc3_1_Relu_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  _fc4_Gemm_output_0_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  output_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 4, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  _conv1_conv1_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 36, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  _conv1_conv1_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  _conv2_conv2_0_Conv_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 576, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  _conv2_conv2_0_Conv_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 16, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  _fc1_fc1_0_Gemm_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 35840, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  _fc1_fc1_0_Gemm_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 40, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  _fc2_fc2_0_Gemm_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1280, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  _fc2_fc2_0_Gemm_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)

/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  _fc3_fc3_0_Gemm_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 256, AI_STATIC)

/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  _fc3_fc3_0_Gemm_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  _fc4_Gemm_output_0_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)

/* Array#27 */
AI_ARRAY_OBJ_DECLARE(
  _fc4_Gemm_output_0_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4, AI_STATIC)

/* Array#28 */
AI_ARRAY_OBJ_DECLARE(
  _conv1_conv1_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 9, AI_STATIC)

/* Array#29 */
AI_ARRAY_OBJ_DECLARE(
  _conv2_conv2_0_Conv_output_0_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 36, AI_STATIC)

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  input_output, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 30, 32), AI_STRIDE_INIT(4, 4, 4, 4, 120),
  1, &input_output_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  _conv1_conv1_0_Conv_output_0_output, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 30, 32), AI_STRIDE_INIT(4, 4, 4, 16, 480),
  1, &_conv1_conv1_0_Conv_output_0_output_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  _conv1_conv1_1_Relu_output_0_output, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 30, 32), AI_STRIDE_INIT(4, 4, 4, 16, 480),
  1, &_conv1_conv1_1_Relu_output_0_output_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  _conv1_conv1_2_MaxPool_output_0_output, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 15, 16), AI_STRIDE_INIT(4, 4, 4, 16, 240),
  1, &_conv1_conv1_2_MaxPool_output_0_output_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  _conv2_conv2_0_Conv_output_0_output, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 15, 16), AI_STRIDE_INIT(4, 4, 4, 64, 960),
  1, &_conv2_conv2_0_Conv_output_0_output_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  _conv2_conv2_1_Relu_output_0_output, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 15, 16), AI_STRIDE_INIT(4, 4, 4, 64, 960),
  1, &_conv2_conv2_1_Relu_output_0_output_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  _conv2_conv2_2_MaxPool_output_0_output, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 7, 8), AI_STRIDE_INIT(4, 4, 4, 64, 448),
  1, &_conv2_conv2_2_MaxPool_output_0_output_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  _flatten_Flatten_output_0_to_chlast_output, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 7, 8, 16), AI_STRIDE_INIT(4, 4, 4, 28, 224),
  1, &_flatten_Flatten_output_0_to_chlast_output_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  _flatten_Flatten_output_0_to_chlast_output0, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 896, 1, 1), AI_STRIDE_INIT(4, 4, 4, 3584, 3584),
  1, &_flatten_Flatten_output_0_to_chlast_output_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  _fc1_fc1_0_Gemm_output_0_output, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 1, 1), AI_STRIDE_INIT(4, 4, 4, 160, 160),
  1, &_fc1_fc1_0_Gemm_output_0_output_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  _fc1_fc1_1_Relu_output_0_output, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 1, 1), AI_STRIDE_INIT(4, 4, 4, 160, 160),
  1, &_fc1_fc1_1_Relu_output_0_output_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  _fc2_fc2_0_Gemm_output_0_output, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &_fc2_fc2_0_Gemm_output_0_output_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  _fc2_fc2_1_Relu_output_0_output, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &_fc2_fc2_1_Relu_output_0_output_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  _fc3_fc3_0_Gemm_output_0_output, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &_fc3_fc3_0_Gemm_output_0_output_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  _fc3_fc3_1_Relu_output_0_output, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &_fc3_fc3_1_Relu_output_0_output_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  _fc4_Gemm_output_0_output, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &_fc4_Gemm_output_0_output_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  output_output, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &output_output_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  _conv1_conv1_0_Conv_output_0_weights, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 4), AI_STRIDE_INIT(4, 4, 4, 16, 48),
  1, &_conv1_conv1_0_Conv_output_0_weights_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  _conv1_conv1_0_Conv_output_0_bias, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &_conv1_conv1_0_Conv_output_0_bias_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  _conv2_conv2_0_Conv_output_0_weights, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 4, 3, 3, 16), AI_STRIDE_INIT(4, 4, 16, 256, 768),
  1, &_conv2_conv2_0_Conv_output_0_weights_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  _conv2_conv2_0_Conv_output_0_bias, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &_conv2_conv2_0_Conv_output_0_bias_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  _fc1_fc1_0_Gemm_output_0_weights, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 896, 40, 1, 1), AI_STRIDE_INIT(4, 4, 3584, 143360, 143360),
  1, &_fc1_fc1_0_Gemm_output_0_weights_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  _fc1_fc1_0_Gemm_output_0_bias, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 1, 40, 1, 1), AI_STRIDE_INIT(4, 4, 4, 160, 160),
  1, &_fc1_fc1_0_Gemm_output_0_bias_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  _fc2_fc2_0_Gemm_output_0_weights, AI_STATIC,
  23, 0x0,
  AI_SHAPE_INIT(4, 40, 32, 1, 1), AI_STRIDE_INIT(4, 4, 160, 5120, 5120),
  1, &_fc2_fc2_0_Gemm_output_0_weights_array, NULL)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  _fc2_fc2_0_Gemm_output_0_bias, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &_fc2_fc2_0_Gemm_output_0_bias_array, NULL)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  _fc3_fc3_0_Gemm_output_0_weights, AI_STATIC,
  25, 0x0,
  AI_SHAPE_INIT(4, 32, 8, 1, 1), AI_STRIDE_INIT(4, 4, 128, 1024, 1024),
  1, &_fc3_fc3_0_Gemm_output_0_weights_array, NULL)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  _fc3_fc3_0_Gemm_output_0_bias, AI_STATIC,
  26, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &_fc3_fc3_0_Gemm_output_0_bias_array, NULL)

/* Tensor #27 */
AI_TENSOR_OBJ_DECLARE(
  _fc4_Gemm_output_0_weights, AI_STATIC,
  27, 0x0,
  AI_SHAPE_INIT(4, 8, 4, 1, 1), AI_STRIDE_INIT(4, 4, 32, 128, 128),
  1, &_fc4_Gemm_output_0_weights_array, NULL)

/* Tensor #28 */
AI_TENSOR_OBJ_DECLARE(
  _fc4_Gemm_output_0_bias, AI_STATIC,
  28, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &_fc4_Gemm_output_0_bias_array, NULL)

/* Tensor #29 */
AI_TENSOR_OBJ_DECLARE(
  _conv1_conv1_0_Conv_output_0_scratch0, AI_STATIC,
  29, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 3, 3), AI_STRIDE_INIT(4, 4, 4, 4, 12),
  1, &_conv1_conv1_0_Conv_output_0_scratch0_array, NULL)

/* Tensor #30 */
AI_TENSOR_OBJ_DECLARE(
  _conv2_conv2_0_Conv_output_0_scratch0, AI_STATIC,
  30, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 3, 3), AI_STRIDE_INIT(4, 4, 4, 16, 48),
  1, &_conv2_conv2_0_Conv_output_0_scratch0_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  output_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc4_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &output_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  output_layer, 15,
  NL_TYPE, 0x0, NULL,
  nl, forward_sm,
  &output_chain,
  NULL, &output_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _fc4_Gemm_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc3_fc3_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc4_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_fc4_Gemm_output_0_weights, &_fc4_Gemm_output_0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _fc4_Gemm_output_0_layer, 14,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &_fc4_Gemm_output_0_chain,
  NULL, &output_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _fc3_fc3_1_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc3_fc3_0_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc3_fc3_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _fc3_fc3_1_Relu_output_0_layer, 13,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &_fc3_fc3_1_Relu_output_0_chain,
  NULL, &_fc4_Gemm_output_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _fc3_fc3_0_Gemm_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc2_fc2_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc3_fc3_0_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_fc3_fc3_0_Gemm_output_0_weights, &_fc3_fc3_0_Gemm_output_0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _fc3_fc3_0_Gemm_output_0_layer, 12,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &_fc3_fc3_0_Gemm_output_0_chain,
  NULL, &_fc3_fc3_1_Relu_output_0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _fc2_fc2_1_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc2_fc2_0_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc2_fc2_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _fc2_fc2_1_Relu_output_0_layer, 11,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &_fc2_fc2_1_Relu_output_0_chain,
  NULL, &_fc3_fc3_0_Gemm_output_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _fc2_fc2_0_Gemm_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc1_fc1_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc2_fc2_0_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_fc2_fc2_0_Gemm_output_0_weights, &_fc2_fc2_0_Gemm_output_0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _fc2_fc2_0_Gemm_output_0_layer, 10,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &_fc2_fc2_0_Gemm_output_0_chain,
  NULL, &_fc2_fc2_1_Relu_output_0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _fc1_fc1_1_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc1_fc1_0_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc1_fc1_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _fc1_fc1_1_Relu_output_0_layer, 9,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &_fc1_fc1_1_Relu_output_0_chain,
  NULL, &_fc2_fc2_0_Gemm_output_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _fc1_fc1_0_Gemm_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_flatten_Flatten_output_0_to_chlast_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_fc1_fc1_0_Gemm_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_fc1_fc1_0_Gemm_output_0_weights, &_fc1_fc1_0_Gemm_output_0_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _fc1_fc1_0_Gemm_output_0_layer, 8,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &_fc1_fc1_0_Gemm_output_0_chain,
  NULL, &_fc1_fc1_1_Relu_output_0_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _flatten_Flatten_output_0_to_chlast_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv2_conv2_2_MaxPool_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_flatten_Flatten_output_0_to_chlast_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _flatten_Flatten_output_0_to_chlast_layer, 7,
  TRANSPOSE_TYPE, 0x0, NULL,
  transpose, forward_transpose,
  &_flatten_Flatten_output_0_to_chlast_chain,
  NULL, &_fc1_fc1_0_Gemm_output_0_layer, AI_STATIC, 
  .out_mapping = AI_SHAPE_INIT(6, AI_SHAPE_IN_CHANNEL, AI_SHAPE_WIDTH, AI_SHAPE_HEIGHT, AI_SHAPE_CHANNEL, AI_SHAPE_DEPTH, AI_SHAPE_EXTENSION), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _conv2_conv2_2_MaxPool_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv2_conv2_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv2_conv2_2_MaxPool_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _conv2_conv2_2_MaxPool_output_0_layer, 6,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp,
  &_conv2_conv2_2_MaxPool_output_0_chain,
  NULL, &_flatten_Flatten_output_0_to_chlast_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _conv2_conv2_1_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv2_conv2_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv2_conv2_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _conv2_conv2_1_Relu_output_0_layer, 5,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &_conv2_conv2_1_Relu_output_0_chain,
  NULL, &_conv2_conv2_2_MaxPool_output_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _conv2_conv2_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv1_conv1_2_MaxPool_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv2_conv2_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_conv2_conv2_0_Conv_output_0_weights, &_conv2_conv2_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_conv2_conv2_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _conv2_conv2_0_Conv_output_0_layer, 4,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_conv2_conv2_0_Conv_output_0_chain,
  NULL, &_conv2_conv2_1_Relu_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _conv1_conv1_2_MaxPool_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv1_conv1_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv1_conv1_2_MaxPool_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _conv1_conv1_2_MaxPool_output_0_layer, 3,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp,
  &_conv1_conv1_2_MaxPool_output_0_chain,
  NULL, &_conv2_conv2_0_Conv_output_0_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _conv1_conv1_1_Relu_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv1_conv1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv1_conv1_1_Relu_output_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  _conv1_conv1_1_Relu_output_0_layer, 2,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &_conv1_conv1_1_Relu_output_0_chain,
  NULL, &_conv1_conv1_2_MaxPool_output_0_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  _conv1_conv1_0_Conv_output_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &_conv1_conv1_0_Conv_output_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &_conv1_conv1_0_Conv_output_0_weights, &_conv1_conv1_0_Conv_output_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &_conv1_conv1_0_Conv_output_0_scratch0, NULL)
)

AI_LAYER_OBJ_DECLARE(
  _conv1_conv1_0_Conv_output_0_layer, 1,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &_conv1_conv1_0_Conv_output_0_chain,
  NULL, &_conv1_conv1_1_Relu_output_0_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 152496, 1, 1),
    152496, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 19344, 1, 1),
    19344, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_AUDIO_CLASSIFIER_IN_NUM, &input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_AUDIO_CLASSIFIER_OUT_NUM, &output_output),
  &_conv1_conv1_0_Conv_output_0_layer, 0x90005191, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 152496, 1, 1),
      152496, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 19344, 1, 1),
      19344, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_AUDIO_CLASSIFIER_IN_NUM, &input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_AUDIO_CLASSIFIER_OUT_NUM, &output_output),
  &_conv1_conv1_0_Conv_output_0_layer, 0x90005191, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool audio_classifier_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_audio_classifier_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    input_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 0);
    input_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 0);
    _conv1_conv1_0_Conv_output_0_scratch0_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 3840);
    _conv1_conv1_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 3840);
    _conv1_conv1_0_Conv_output_0_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 3876);
    _conv1_conv1_0_Conv_output_0_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 3876);
    _conv1_conv1_1_Relu_output_0_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 3876);
    _conv1_conv1_1_Relu_output_0_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 3876);
    _conv1_conv1_2_MaxPool_output_0_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 0);
    _conv1_conv1_2_MaxPool_output_0_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 0);
    _conv2_conv2_0_Conv_output_0_scratch0_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 3840);
    _conv2_conv2_0_Conv_output_0_scratch0_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 3840);
    _conv2_conv2_0_Conv_output_0_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 3984);
    _conv2_conv2_0_Conv_output_0_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 3984);
    _conv2_conv2_1_Relu_output_0_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 3984);
    _conv2_conv2_1_Relu_output_0_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 3984);
    _conv2_conv2_2_MaxPool_output_0_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 0);
    _conv2_conv2_2_MaxPool_output_0_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 0);
    _flatten_Flatten_output_0_to_chlast_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 3584);
    _flatten_Flatten_output_0_to_chlast_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 3584);
    _fc1_fc1_0_Gemm_output_0_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 0);
    _fc1_fc1_0_Gemm_output_0_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 0);
    _fc1_fc1_1_Relu_output_0_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 160);
    _fc1_fc1_1_Relu_output_0_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 160);
    _fc2_fc2_0_Gemm_output_0_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 0);
    _fc2_fc2_0_Gemm_output_0_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 0);
    _fc2_fc2_1_Relu_output_0_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 128);
    _fc2_fc2_1_Relu_output_0_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 128);
    _fc3_fc3_0_Gemm_output_0_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 0);
    _fc3_fc3_0_Gemm_output_0_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 0);
    _fc3_fc3_1_Relu_output_0_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 32);
    _fc3_fc3_1_Relu_output_0_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 32);
    _fc4_Gemm_output_0_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 0);
    _fc4_Gemm_output_0_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 0);
    output_output_array.data = AI_PTR(g_audio_classifier_activations_map[0] + 16);
    output_output_array.data_start = AI_PTR(g_audio_classifier_activations_map[0] + 16);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool audio_classifier_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_audio_classifier_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    _conv1_conv1_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _conv1_conv1_0_Conv_output_0_weights_array.data = AI_PTR(g_audio_classifier_weights_map[0] + 0);
    _conv1_conv1_0_Conv_output_0_weights_array.data_start = AI_PTR(g_audio_classifier_weights_map[0] + 0);
    _conv1_conv1_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _conv1_conv1_0_Conv_output_0_bias_array.data = AI_PTR(g_audio_classifier_weights_map[0] + 144);
    _conv1_conv1_0_Conv_output_0_bias_array.data_start = AI_PTR(g_audio_classifier_weights_map[0] + 144);
    _conv2_conv2_0_Conv_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _conv2_conv2_0_Conv_output_0_weights_array.data = AI_PTR(g_audio_classifier_weights_map[0] + 160);
    _conv2_conv2_0_Conv_output_0_weights_array.data_start = AI_PTR(g_audio_classifier_weights_map[0] + 160);
    _conv2_conv2_0_Conv_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _conv2_conv2_0_Conv_output_0_bias_array.data = AI_PTR(g_audio_classifier_weights_map[0] + 2464);
    _conv2_conv2_0_Conv_output_0_bias_array.data_start = AI_PTR(g_audio_classifier_weights_map[0] + 2464);
    _fc1_fc1_0_Gemm_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _fc1_fc1_0_Gemm_output_0_weights_array.data = AI_PTR(g_audio_classifier_weights_map[0] + 2528);
    _fc1_fc1_0_Gemm_output_0_weights_array.data_start = AI_PTR(g_audio_classifier_weights_map[0] + 2528);
    _fc1_fc1_0_Gemm_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _fc1_fc1_0_Gemm_output_0_bias_array.data = AI_PTR(g_audio_classifier_weights_map[0] + 145888);
    _fc1_fc1_0_Gemm_output_0_bias_array.data_start = AI_PTR(g_audio_classifier_weights_map[0] + 145888);
    _fc2_fc2_0_Gemm_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _fc2_fc2_0_Gemm_output_0_weights_array.data = AI_PTR(g_audio_classifier_weights_map[0] + 146048);
    _fc2_fc2_0_Gemm_output_0_weights_array.data_start = AI_PTR(g_audio_classifier_weights_map[0] + 146048);
    _fc2_fc2_0_Gemm_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _fc2_fc2_0_Gemm_output_0_bias_array.data = AI_PTR(g_audio_classifier_weights_map[0] + 151168);
    _fc2_fc2_0_Gemm_output_0_bias_array.data_start = AI_PTR(g_audio_classifier_weights_map[0] + 151168);
    _fc3_fc3_0_Gemm_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _fc3_fc3_0_Gemm_output_0_weights_array.data = AI_PTR(g_audio_classifier_weights_map[0] + 151296);
    _fc3_fc3_0_Gemm_output_0_weights_array.data_start = AI_PTR(g_audio_classifier_weights_map[0] + 151296);
    _fc3_fc3_0_Gemm_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _fc3_fc3_0_Gemm_output_0_bias_array.data = AI_PTR(g_audio_classifier_weights_map[0] + 152320);
    _fc3_fc3_0_Gemm_output_0_bias_array.data_start = AI_PTR(g_audio_classifier_weights_map[0] + 152320);
    _fc4_Gemm_output_0_weights_array.format |= AI_FMT_FLAG_CONST;
    _fc4_Gemm_output_0_weights_array.data = AI_PTR(g_audio_classifier_weights_map[0] + 152352);
    _fc4_Gemm_output_0_weights_array.data_start = AI_PTR(g_audio_classifier_weights_map[0] + 152352);
    _fc4_Gemm_output_0_bias_array.format |= AI_FMT_FLAG_CONST;
    _fc4_Gemm_output_0_bias_array.data = AI_PTR(g_audio_classifier_weights_map[0] + 152480);
    _fc4_Gemm_output_0_bias_array.data_start = AI_PTR(g_audio_classifier_weights_map[0] + 152480);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_audio_classifier_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_AUDIO_CLASSIFIER_MODEL_NAME,
      .model_signature   = AI_AUDIO_CLASSIFIER_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 226004,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x90005191,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_audio_classifier_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_AUDIO_CLASSIFIER_MODEL_NAME,
      .model_signature   = AI_AUDIO_CLASSIFIER_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 226004,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x90005191,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_audio_classifier_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_audio_classifier_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_audio_classifier_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_audio_classifier_create(network, AI_AUDIO_CLASSIFIER_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_audio_classifier_data_params_get(&params) != true) {
    err = ai_audio_classifier_get_error(*network);
    return err;
  }
#if defined(AI_AUDIO_CLASSIFIER_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_AUDIO_CLASSIFIER_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_audio_classifier_init(*network, &params) != true) {
    err = ai_audio_classifier_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_audio_classifier_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_audio_classifier_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_audio_classifier_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_audio_classifier_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= audio_classifier_configure_weights(net_ctx, params);
  ok &= audio_classifier_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_audio_classifier_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_audio_classifier_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_AUDIO_CLASSIFIER_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME


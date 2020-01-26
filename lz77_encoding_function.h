//
// Created by Mitchell Mancuso on 2019-03-06.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libpnm.h"
#include <math.h>

#ifndef CS4481B_A3_LZ77_ENCODING_FUNCTION_H
#define CS4481B_A3_LZ77_ENCODING_FUNCTION_H

void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size, float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr);

#endif //CS4481B_A3_LZ77_ENCODING_FUNCTION_H

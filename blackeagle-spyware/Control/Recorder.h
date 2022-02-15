#pragma once
#include <Windows.h>
#include <mmeapi.h>
#include <MMSystem.h>
#include <iostream>
#include <stdio.h>
#include "../DataManagement/files.h"

typedef struct WAV_HEADER {
    uint8_t riff[4] = { 'R', 'I', 'F', 'F' };
    uint32_t overall_size;
    uint8_t wave[4] = { 'W', 'A', 'V', 'E' };
    uint8_t fmt_chunk_marker[4] = { 'f', 'm', 't', ' ' };
    uint32_t length_of_fmt;
    uint16_t format_type;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byterate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint8_t data_chunk_header[4] = { 'd', 'a', 't', 'a' };
    uint32_t data_size;
} WavHeader;


DWORD recordWAVEFile(DWORD dwMilliSeconds);
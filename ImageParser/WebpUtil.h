#pragma once

#include <webp/decode.h>

typedef struct 
{
    int width;
    int height;
    bool has_alpha;
    uint8_t* rgba;
} WebpImage;

bool ParseWebpImage(const wchar_t* file_name, WebpImage* const image, const int scale_width, const int scale_height);
bool ParseWebpImage(const uint8_t* data, const size_t data_size, WebpImage* const image, const int scale_width, const int scale_height);
void CleanWebpExternalData(WebpImage* const data);

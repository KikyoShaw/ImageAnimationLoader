#pragma once
#include <cstdint>
#include <webp/decode.h>

bool ImgIoUtilReadFile(const wchar_t* const file_name,
                       const uint8_t** data, size_t* data_size);
#include "pch.h"
#include "IOUtil.h"

#include <cstdio>
#include <webp/types.h>

#include "unicode.h"


bool ImgIoUtilReadFile(const wchar_t* const file_name,
                       const uint8_t** data, size_t* data_size)
{
    if (data == nullptr || data_size == nullptr) return false;

    *data = nullptr;
    *data_size = 0;

    const auto in = WFOPEN(file_name, "rb");
    if (in == nullptr) {
        WFPRINTF(L"cannot open input file '%s'\n", file_name);
        return false;
    }
    fseek(in, 0, SEEK_END);
    const size_t file_size = ftell(in);
    fseek(in, 0, SEEK_SET);
    // we allocate one extra byte for the \0 terminator
    const auto file_data = static_cast<uint8_t*>(WebPMalloc(file_size + 1));
    if (file_data == nullptr) {
        fclose(in);
        WFPRINTF(L"memory allocation failure when reading file %s\n",
                    file_name);
        return false;
    }
    const auto ok = fread(file_data, file_size, 1, in) == 1;
    fclose(in);

    if (!ok) {
        WFPRINTF(L"Could not read %d bytes of data from file %s\n",
            static_cast<int>(file_size), file_name);
        WebPFree(file_data);
        return false;
    }
    file_data[file_size] = '\0';  // convenient 0-terminator
    *data = file_data;
    *data_size = file_size;
    return true;
}

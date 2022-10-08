#include "pch.h"
#include "WebpUtil.h"

#include <webp/mux_types.h>

#include "IOUtil.h"
#include "unicode.h"

static bool UseScaling(const int scale_width, const int scale_height)
{
    return scale_width > 0 || scale_height > 0;
}

static void CalculateScaling(const int ori_width, const int ori_height, int& scale_width, int& scale_height)
{
    // 宽高都设置了则不需要等比例缩放
    if (scale_width > 0 && scale_height > 0)
        return;

    // 数据合法校验
    if (ori_width <= 0 && ori_height <= 0)
    {
        WFPRINTF(L"ori_width and ori_height Invalid\n");
        return;
    }

    const auto ratio = static_cast<float>(ori_width) / static_cast<float>(ori_height);
    if (scale_width > 0 && scale_height == 0)
    {
        scale_height = static_cast<int>(static_cast<float>(scale_width) / ratio + 0.5);
    }

    if (scale_width == 0 && scale_height > 0)
    {
        scale_width = static_cast<int>(static_cast<float>(scale_height) * ratio + 0.5);
    }
}

static bool AllocateExternalData(WebpImage* const image, WebPDecoderConfig* config)
{
    auto w = config->input.width;
    auto h = config->input.height;
    if (config->options.use_scaling)
    {
        w = config->options.scaled_width;
        h = config->options.scaled_height;
    }
    else if (config->options.use_cropping)
    {
        w = config->options.crop_width;
        h = config->options.crop_height;
    }

    const uint32_t stride = w * kNumChannels /*+ 7*/;
    const size_t data_size = stride * h;
    const auto external_buffer = static_cast<uint8_t* const>(WebPMalloc(data_size));
    if (external_buffer == nullptr)
    {
        WFPRINTF(L"external_buffer alloc fail.\n");
        return false;
    }

    WebPDecBuffer* const output_buffer = &config->output;
    output_buffer->u.RGBA.stride = stride;
    output_buffer->u.RGBA.size = data_size;
    output_buffer->u.RGBA.rgba = external_buffer;
    output_buffer->is_external_memory = 1;
    image->rgba = external_buffer;
    image->height = h;
    image->width = w;

    return true;
}

void CleanWebpExternalData(WebpImage* const data)
{
    if (data != nullptr)
    {
        WebPFree((void*)data->rgba);
        data->rgba = nullptr;
        data->height = 0; data->width = 0;
        data->has_alpha = false;
    }
}

static bool ReadWebpImage(const wchar_t* file_name, const WebPData* const webp_data, WebpImage* const image, const int scale_width, const int scale_height)
{
    WebPDecoderConfig config{};
    WebPDecBuffer* const output_buffer = &config.output;
    WebPBitstreamFeatures* const bit_stream = &config.input;
    if (!WebPInitDecoderConfig(&config))
    {
        WFPRINTF(L"Library version mismatch!\n");
        return false;
    }

    auto status = WebPGetFeatures(webp_data->bytes, webp_data->size, bit_stream);
    if (status != VP8_STATUS_OK)
    {
        WFPRINTF(L"Decoding of %s failed. Status: %d\n", file_name, status);
        return false;
    }

    // gdi+ bitmap format
    output_buffer->colorspace = MODE_BGRA;

    if (UseScaling(scale_width, scale_height))
    {
        config.options.use_scaling = 1;
        int width = scale_width;
        int height = scale_height;
        CalculateScaling(bit_stream->width, bit_stream->height, width, height);
        config.options.scaled_width = width;
        config.options.scaled_height = height;
    }

    if (!AllocateExternalData(image, &config))
    {
        WFPRINTF(L"AllocateExternalData fail, file: %s\n", file_name);
        return false;
    }

    image->has_alpha = bit_stream->has_alpha == 1;

    status = WebPDecode(webp_data->bytes, webp_data->size, &config);
    WebPFreeDecBuffer(output_buffer);

    return status == VP8_STATUS_OK;
}

bool ParseWebpImage(const wchar_t* file_name, WebpImage* const image, const int scale_width,
    const int scale_height)
{
    WebPData webp_data;
    WebPDataInit(&webp_data);
    if (!ImgIoUtilReadFile(file_name, &webp_data.bytes, &webp_data.size)) {
        WFPRINTF(L"Error reading file: %s\n", file_name);
        return false;
    }

    const auto ok = ReadWebpImage(file_name, &webp_data, image, scale_width, scale_height);
    if (!ok)
        CleanWebpExternalData(image);

    WebPDataClear(&webp_data);

    return ok;
}

bool ParseWebpImage(const uint8_t* data, const size_t data_size, WebpImage* const image,
    const int scale_width, const int scale_height)
{
    WebPData webp_data;
    WebPDataInit(&webp_data);
    webp_data.bytes = data;
    webp_data.size = data_size;

    const auto file_name = L"from_memory_data";
    const auto ok = ReadWebpImage(file_name, &webp_data, image, scale_width, scale_height);
    if (!ok)
        CleanWebpExternalData(image);

    return ok;
}

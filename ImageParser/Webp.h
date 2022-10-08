#pragma once

#include "WebpUtil.h"

using namespace System;

namespace ImageParser
{
    public ref class Webp
    {
    public:
        ~Webp();
        !Webp();

        bool GetOk();
        bool ParseWebpFile(String^ file_name, int decode_width, int decode_height);
        bool ParseWebpMemBytes(array<System::Byte>^ bytes, int decode_width, int decode_height);
        int GetCavWidth();
        int GetCavHeight();
        bool GetFrameData([Runtime::InteropServices::Out] System::IntPtr% data_ptr, [Runtime::InteropServices::Out] int% data_size);
        bool HasAlpha();

    protected:
        void CleanUp();

    private:
        WebpImage* m_pWebpData_ = nullptr;
        bool m_Ok_ = false;
    };
}

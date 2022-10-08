#include "pch.h"
#include "Webp.h"

#include <cstdlib>
#include <vcclr.h>

namespace ImageParser
{
    Webp::~Webp()
    {
        this->!Webp();
    }

    Webp::!Webp()
    {
        CleanUp();
    }

    bool Webp::GetOk()
    {
        return m_Ok_;
    }

    bool Webp::ParseWebpFile(String^ file_name, const int decode_width, const int decode_height)
    {
        CleanUp();
        m_pWebpData_ = new WebpImage();
        pin_ptr<const wchar_t> wfile_name = PtrToStringChars(file_name);
        m_Ok_ = ParseWebpImage(wfile_name, m_pWebpData_, decode_width, decode_height);

        return m_Ok_;
    }

    bool Webp::ParseWebpMemBytes(array<System::Byte>^ bytes, const int decode_width, const int decode_height)
    {
        if (bytes == nullptr)
            return false;

        CleanUp();
        m_pWebpData_ = new WebpImage();
        auto unmanaged_ptr = Runtime::InteropServices::Marshal::UnsafeAddrOfPinnedArrayElement(bytes, 0);
        m_Ok_ = ParseWebpImage(static_cast<uint8_t*>(unmanaged_ptr.ToPointer()), bytes->Length, m_pWebpData_, decode_width, decode_height);

        return m_Ok_;
    }

    int Webp::GetCavWidth()
    {
        if (m_pWebpData_ == nullptr)
            return 0;

        return  m_pWebpData_->width;
    }

    int Webp::GetCavHeight()
    {
        if (m_pWebpData_ == nullptr)
            return 0;

        return m_pWebpData_->height;
    }

    bool Webp::GetFrameData(System::IntPtr% data_ptr, int% data_size)
    {
        if (m_pWebpData_ == nullptr)
            return false;

        if (m_pWebpData_->rgba == nullptr)
            return false;

        data_ptr = IntPtr(m_pWebpData_->rgba);
        data_size = m_pWebpData_->width * kNumChannels * m_pWebpData_->height;

        return true;
    }

    bool Webp::HasAlpha()
    {
        if (m_pWebpData_ == nullptr)
            return false;

        return m_pWebpData_->has_alpha;
    }

    void Webp::CleanUp()
    {
        if (m_pWebpData_ != nullptr)
        {
            CleanWebpExternalData(m_pWebpData_);
            delete m_pWebpData_;
            m_pWebpData_ = nullptr;
            m_Ok_ = false;
        }
    }
}


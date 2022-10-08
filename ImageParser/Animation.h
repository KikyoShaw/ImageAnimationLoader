#pragma once

#include "AnimationUtil.h"
using namespace System;


namespace ImageParser {
	public ref class Animation
	{
	public:
		~Animation();
		!Animation();

		bool GetOk();
		bool ParseAnimationFrames(String^ file_name);
		bool ParseAnimationFrames(array<System::Byte>^ bytes);
		bool GetFrameAt(int index, [Runtime::InteropServices::Out] System::IntPtr% data_ptr,
			[Runtime::InteropServices::Out] int% data_size, [Runtime::InteropServices::Out] int% duration);
		int GetFrameCount();
		int GetCavWidth();
		int GetCavHeight();
		bool HasAlpha();

	protected:
		void CleanUp();

	private:
		AnimatedImage* m_pImage_ = nullptr;
		bool m_Ok_ = false;
	};
}

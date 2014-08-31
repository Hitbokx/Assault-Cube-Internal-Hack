#pragma once

#pragma comment(lib,"OpenGL32.lib")
#include <Windows.h>
#include <iostream>
#include <gl\GL.h>
#include "HookingClass.h"
#include "Vector.h"

namespace GL
{
	class Font
	{
	public:
		bool bBuilt{ false };
		unsigned int base{};
		HDC hDc{ nullptr };
		int height;
		int width;

		void Build( int height );
		void Print( float x, float y, const unsigned char colour[3], const char* format, ... );

		Vector3 centerText( float x, float y, float width, float height, float textWidth, float textHeight );

		float centerText( float x, float width, float textWidth );
	};
}
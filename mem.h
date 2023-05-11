#pragma once

#include <vector>
#include <Windows.h>

namespace mem
{
	void patch( BYTE* dst, BYTE* src, uintptr_t size );
	void nop( BYTE* dst, uintptr_t size );
	uintptr_t findDMAAddy( uintptr_t ptr, std::vector<uintptr_t>offsets );
}
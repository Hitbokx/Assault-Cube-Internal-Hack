#include "mem.h"

void mem::patch( BYTE* dst, BYTE* src, uintptr_t size )
{
	DWORD oldProtect{};
	VirtualProtect( dst, size, PAGE_EXECUTE_READWRITE, &oldProtect );
	memcpy( dst, src, size );
	VirtualProtect( dst, size, oldProtect, &oldProtect );
}

void mem::nop( BYTE* dst, uintptr_t size )
{
	DWORD oldProtect{};
	VirtualProtect( dst, size, PAGE_EXECUTE_READWRITE, &oldProtect );
	memset( dst, 0x90, size );
	VirtualProtect( dst, size, oldProtect, &oldProtect );
}

uintptr_t mem::findDMAAddy( uintptr_t ptr, std::vector<uintptr_t>offsets )
{
	uintptr_t addr{ ptr };
	for ( unsigned int i{ 0 }; i < offsets.size( ); ++i )
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}
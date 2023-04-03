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

bool mem::Detour32( BYTE* src, BYTE* dst, const intptr_t len )
{
	if ( len < 5 )
		return false;

	DWORD oldProtect;

	VirtualProtect( src, len, PAGE_EXECUTE_READWRITE, &oldProtect );

	intptr_t relativeAddress{ (intptr_t)dst - (intptr_t)src - 5 };

	*src = 0xE9;

	*(intptr_t*)(src + 1) = relativeAddress;

	VirtualProtect( src, len, oldProtect, &oldProtect );

	return true;
}

BYTE* mem::TrampHook32( BYTE* src, BYTE* dst, const intptr_t len )
{
	if ( len < 5 )
		return 0;

	// Create Gateway
	BYTE* gateway{ (BYTE*)(VirtualAlloc( 0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE )) };

	memcpy_s( gateway, len, src, len );

	intptr_t gatewayRelativeAddress{ (intptr_t)src - (intptr_t)gateway - 5 };

	*(gateway + len) = 0xE9;

	*(intptr_t*)(gateway + len + 1) = gatewayRelativeAddress;

	Detour32( src, dst, len );

	return gateway;
}

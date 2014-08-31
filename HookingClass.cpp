#include "HookingClass.h"

bool mem::Detour64( BYTE* src, BYTE* dst, const uintptr_t len )
{
	if ( len < 5 ) return false;

	DWORD curProtection;
	VirtualProtect( src, len, PAGE_EXECUTE_READWRITE, &curProtection );

	uintptr_t relativeAddress = dst - src - 5;

	*src = 0xE9;

	*(uintptr_t*)(src + 1) = relativeAddress;

	VirtualProtect( src, len, curProtection, &curProtection );
	return true;
}

BYTE* mem::TrampHook64( BYTE* src, BYTE* dst, const uintptr_t len )
{
	if ( len < 5 ) return 0;

	//Create Gateway
	BYTE* gateway = (BYTE*)VirtualAlloc( 0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );

	//write the stolen bytes to the gateway
	memcpy_s( gateway, len, src, len );

	//Get the gateway to destination address
	uintptr_t gatewayRelativeAddr = src - gateway - 5;

	// add the jmp opcode to the end of the gateway
	*(gateway + len) = 0xE9;

	//Write the address of the gateway to the jmp
	*(uintptr_t*)((uintptr_t)gateway + len + 1) = gatewayRelativeAddr;

	//Perform the detour
	mem::Detour64( src, dst, len );

	return gateway;
}

void mem::Patch( BYTE* dst, BYTE* src, unsigned int size )
{
	DWORD oldprotect;
	VirtualProtect( dst, size, PAGE_EXECUTE_READWRITE, &oldprotect );

	memcpy( dst, src, size );
	VirtualProtect( dst, size, oldprotect, &oldprotect );
}

Hook2::Hook2( BYTE* src, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len )
{
	this->src = src;
	this->dst = dst;
	this->len = len;
	this->PtrToGatewayFnPtr = PtrToGatewayFnPtr;
}

Hook2::Hook2( const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len )
{
	HMODULE hMod = GetModuleHandleA( modName );

	this->src = (BYTE*)GetProcAddress( hMod, exportName );
	this->dst = dst;
	this->len = len;
	this->PtrToGatewayFnPtr = PtrToGatewayFnPtr;
}

void Hook2::Enable( )
{
	memcpy( originalBytes, src, len );
	*(uintptr_t*)PtrToGatewayFnPtr = (uintptr_t)mem::TrampHook64( src, dst, len );
	bStatus = true;
}

void Hook2::Disable( )
{
	mem::Patch( src, originalBytes, len );
	bStatus = false;
}

void Hook2::Toggle( )
{
	if ( !bStatus ) Enable( );
	else Disable( );
}
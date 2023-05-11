#include "HookingClass.h"
/*
bool Hook::Detour32( BYTE* src, BYTE* dst, const uintptr_t len )
{
	try
	{
		if ( len < 5 )
			throw "Connot HOOK a function of length less than 5.\n";
	}

	catch ( const char* error )
	{
		std::cerr << "Error: " << error << '\n';
	}

	DWORD curProtection;
	VirtualProtect( src, len, PAGE_EXECUTE_READWRITE, &curProtection );

	uintptr_t relativeAddress = dst - src - 5;

	*src = 0xE9;

	*(uintptr_t*)(src + 1) = relativeAddress;

	VirtualProtect( src, len, curProtection, &curProtection );
	return true;
}

BYTE* Hook::TrampHook32( BYTE* src, BYTE* dst, const uintptr_t len )
{
	try
	{
		if ( len < 5 )
			throw "Connot HOOK a function of length less than 5.\n";
	}

	catch ( const char* error )
	{
		std::cerr << "Error: " << error << '\n';
	}

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
	Hook::Detour32( src, dst, len );

	return gateway;
}

Hook::Hook( BYTE* src, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len )
{
	this->m_src = src;
	this->m_dst = dst;
	this->m_len = len;
	this->m_gateway = PtrToGatewayFnPtr;
}

Hook::Hook( const char* exportedFuncName, const char* modName, BYTE* dst, BYTE* PtrToGatewayPtr, uintptr_t len )
{
	HMODULE hMod = GetModuleHandleA( modName );

	this->m_src = (BYTE*)GetProcAddress( hMod, exportedFuncName );
	this->m_dst = dst;
	this->m_len = len;
	this->m_gateway = PtrToGatewayPtr;
}

void Hook::startHook( )
{
	memcpy( originalBytes, m_src, m_len );
	*(uintptr_t*)m_gateway = (uintptr_t)Hook::TrampHook32( m_src, m_dst, m_len );
	bStatus = true;
}


template <SIZE_T LENGTH>
void Hook::patchByte( BYTE* lpOriginalFuncAddrs )
{
	DWORD oldprotect;
	VirtualProtect( lpOriginalFuncAddrs, LENGTH, PAGE_EXECUTE_READWRITE, &oldProc );
	RtlMoveMemory( lpOriginalFuncAddrs, m_gateWay, LENGTH );
	VirtualProtect( lpOriginalFuncAddrs, LENGTH, oldProc, &oldProc );
}

template <SIZE_T LENGTH>
void Hook::unHook( BYTE* lpOriginalFuncAddrs )
{
	patchByte<LENGTH>( lpOriginalFuncAddrs );
	if ( m_gateway )
		VirtualFree( m_gateway, 0, MEM_RELEASE );
	m_gateway = nullptr;
	bStatus = false;
}

void Hook::Toggle( )
{
	if ( !bStatus )
		startHook( );
	else
		unHook( );
}*/
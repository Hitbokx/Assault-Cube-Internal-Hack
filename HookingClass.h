#pragma once

#include <Windows.h>
#include <iostream>

class Hook
{
private:

	BYTE* m_gateway{ nullptr };

	BYTE originalBytes[10]{ 0 };

	template <SIZE_T LENGTH>
	void calculateRelativeAddress( BYTE*& src, BYTE*& dst )
	{
		intptr_t relativeAddress{ (intptr_t)dst - (intptr_t)src - 5 };

		*(src + LENGTH) = 0xE9;

		*(intptr_t*)(src + LENGTH + 1) = relativeAddress;
	}

public:
	Hook( ) = default;

	Hook( const Hook& hook ) = delete;                               // copy constructor

	Hook& operator=( const Hook& hook ) = delete;                    // copy assignment

	~Hook( )
	{
		VirtualFree( m_gateway, NULL, MEM_RELEASE );
		m_gateway = nullptr;
	}

	template <SIZE_T LENGTH>
	void saveOrigBytes( void* src )
	{
		m_gateway = (BYTE*)(VirtualAlloc( 0, LENGTH, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE ));
		memcpy_s( m_gateway, LENGTH, src, LENGTH );
	}

	template <SIZE_T LENGTH>
	bool midDetour32( BYTE* src, BYTE* dst )
	{
		try
		{
			if ( LENGTH < 5 )
				throw "Connot HOOK a function of length less than 5.\n";
		}

		catch ( const char* error )
		{
			std::cerr << "Error: " << error << '\n';
		}

		DWORD oldProtect{};
		VirtualProtect( src, LENGTH, PAGE_EXECUTE_READWRITE, &oldProtect );

		memset( src, 0x90, LENGTH );                          // no-op rest of the instructions

		calculateRelativeAddress< 0 >( src, dst );

		VirtualProtect( src, LENGTH, oldProtect, &oldProtect );

		return true;
	}

	template <SIZE_T LENGTH>
	bool Detour32( BYTE* src, BYTE* dst )
	{
		try
		{
			if ( LENGTH < 5 )
				throw "Connot HOOK a function of length less than 5.\n";
		}

		catch ( const char* error )
		{
			std::cerr << "Error: " << error << '\n';
		}

		DWORD oldProtect;
		VirtualProtect( src, LENGTH, PAGE_EXECUTE_READWRITE, &oldProtect );

		calculateRelativeAddress<0>( src, dst );

		VirtualProtect( src, LENGTH, oldProtect, &oldProtect );

		return true;
	}

	template <SIZE_T LENGTH>
	BYTE* TrampHook32( BYTE* src, BYTE* dst )
	{
		try
		{
			if ( LENGTH < 5 )
				throw "Connot HOOK a function of length less than 5.\n";
		}

		catch ( const char* error )
		{
			std::cerr << "Error: " << error << '\n';
		}

		BYTE* gateway = (BYTE*)VirtualAlloc( 0, LENGTH, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );

		//write the stolen bytes to the gateway
		memcpy_s( gateway, LENGTH, src, LENGTH );

		calculateRelativeAddress<LENGTH>( gateway, src );

		//Perform the detour
		Detour32<LENGTH>( src, dst );

		return gateway;
	}

	template <SIZE_T LENGTH>
	void patchByte( BYTE* lpOriginalFuncAddrs )
	{
		DWORD oldprotect{};

		VirtualProtect( lpOriginalFuncAddrs, LENGTH, PAGE_EXECUTE_READWRITE, &oldprotect );
		RtlMoveMemory( lpOriginalFuncAddrs, m_gateway, LENGTH );
		VirtualProtect( lpOriginalFuncAddrs, LENGTH, oldprotect, &oldprotect );
	}

	/*==============================*/
	template <SIZE_T LENGTH>
	void startHook( const char* exportedFuncName, const char* modName, BYTE* dst, BYTE* PtrToGatewayPtr )
	{
		HMODULE hMod{ GetModuleHandleA( modName ) };

		BYTE* src{ (BYTE*)GetProcAddress( hMod, exportedFuncName ) };
		m_gateway = PtrToGatewayPtr;

		memcpy( originalBytes, src, LENGTH );
		*(uintptr_t*)m_gateway = (uintptr_t)TrampHook32<LENGTH>( src, dst );
	}
	/*==============================*/

	template <SIZE_T LENGTH>
	void unHook( )
	{
		patchByte<LENGTH>( originalBytes );

		if ( m_gateway )
			VirtualFree( m_gateway, NULL, MEM_RELEASE );
		m_gateway = nullptr;
	}
};

inline Hook g_hook{};
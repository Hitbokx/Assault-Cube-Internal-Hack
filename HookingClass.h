#pragma once

#include <Windows.h>
#include <iostream>

class Hook
{
private:
	BYTE* m_dst{ nullptr };
	BYTE* m_src{ nullptr };
	BYTE* m_gateway{ nullptr };
	uintptr_t m_len{ 0 };

	template <SIZE_T LENGTH>
	void giveProtection( void* src, DWORD& oldProtect)
	{
		VirtualProtect( src, LENGTH, PAGE_EXECUTE_READWRITE, &oldProtect );
	}

	template <SIZE_T LENGTH>
	void restoreProtection( void* src, DWORD& oldProtect )
	{
		VirtualProtect( src, LENGTH, oldProtect, &oldProtect );
	}

	template <SIZE_T LENGTH>
	void calculateRelativeAddress( BYTE* src, BYTE* dst )
	{
		intptr_t relativeAddress{ (intptr_t)dst - (intptr_t)src - 5 };

		*(src + LENGTH) = 0xE9;

		*(intptr_t*)(src + LENGTH + 1) = relativeAddress;
	}

public:
	Hook( ) = default;

	Hook( const Hook& hook ) = delete;                               // copy constructor

	Hook( const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGatewayPtr, uintptr_t len )
	{
		HMODULE hMod = GetModuleHandleA( modName );

		this->m_src = (BYTE*)GetProcAddress( hMod, exportName );
		this->m_dst = dst;
		this->m_len = len;
		this->m_gateway = PtrToGatewayPtr;
	}

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
		giveProtection<LENGTH>( src, oldProtect );

		memset( src, 0x90, LENGTH );                          // no-op rest of the instructions

		calculateRelativeAddress< 0 >( src, dst );      

		restoreProtection<LENGTH>( src, oldProtect );

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
		giveProtection<LENGTH>( src, oldProtect );

		calculateRelativeAddress<0>( src, dst );

		restoreProtection<LENGTH>( src, oldProtect );

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

		saveOrigBytes<LENGTH>( src );

		calculateRelativeAddress<LENGTH>( m_gateway, src );

		Detour32<LENGTH>( src, dst );

		return m_gateway;
	}

	template <SIZE_T LENGTH>
	void patch( BYTE* dst, BYTE* src, intptr_t len)
	{
		DWORD oldProtect{};
		giveProtection<LENGTH>( dst, oldProtect );

		memcpy( dst, src, len );
		restoreProtection<LENGTH>( dst, oldProtect );
	}

	template <SIZE_T LENGTH>
	void nop( BYTE* dst, intptr_t len )
	{
		DWORD oldProtect{};
		giveProtection<LENGTH>( dst, oldProtect );

		memset( dst, 0x90, len );
		restoreProtection<LENGTH>( dst, oldProtect );
	}
};

inline Hook g_hook{};

namespace mem
{
	bool Detour64( BYTE* src, BYTE* dst, const uintptr_t len );
	

	BYTE* TrampHook64( BYTE* src, BYTE* dst, const uintptr_t len );
	

	void Patch( BYTE* dst, BYTE* src, unsigned int size );
	
}

struct Hook2
{
	bool bStatus{ false };

	BYTE* src{ nullptr };
	BYTE* dst{ nullptr };
	BYTE* PtrToGatewayFnPtr{ nullptr };
	uintptr_t len{ 0 };

	BYTE originalBytes[10]{ 0 };

	Hook2( BYTE* src, BYTE* dst, BYTE* PtrToGatewayPtr, uintptr_t len );
	
	Hook2( const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGatewayPtr, uintptr_t len );

	void Enable( );

	void Disable( );

	void Toggle( );
};
#include<Windows.h>
#include<iostream>
#include "mem.h"

struct Vector3
{
	float x{};
	float y{};
	float z{};
};

class ent
{
public:
	Vector3 headPos; //0x0004
	char pad_0010[36]; //0x0010
	Vector3 playerPos; //0x0034
	Vector3 viewAngles; //0x0040
	char pad_004C[172]; //0x004C
	int32_t Health; //0x00F8
	int32_t Armour; //0x00FC
	char pad_0100[88]; //0x0100
	int32_t numGrenades; //0x0158
	char pad_015C[201]; //0x015C
	char name[16]; //0x0225
	char pad_0235[319]; //0x0235
	class weapon* currentWeapon; //0x0374

	virtual void Function0( );
	virtual void Function1( );
	virtual void Function2( );
	virtual void Function3( );
	virtual void Function4( );
	virtual void Function5( );
	virtual void Function6( );
	virtual void Function7( );
	virtual void Function8( );
	virtual void Function9( );
}; //Size: 0x0378
static_assert(sizeof( ent ) == 0x378);

class weapon
{
public:
	char pad_0000[20]; //0x0000
	class ammoClip* ammoPtr; //0x0014
}; //Size: 0x0018
static_assert(sizeof( weapon ) == 0x18);

class ammoClip
{
public:
	int32_t ammo; //0x0000
}; //Size: 0x0004
static_assert(sizeof( ammoClip ) == 0x4);

DWORD WINAPI myThreadProc( HMODULE hModule )
{
	// Create console

	AllocConsole( ); // To allocate console for logging
	FILE* f;
	freopen_s( &f, "CONOUT$", "w", stdout );
	std::cout << "OG for a fee, stay sippin' fam\n\n";

	// Get module base

	uintptr_t ModuleBase{ (uintptr_t)GetModuleHandle( L"ac_client.exe" ) };
	ent* pLocalPlayerBaseAddr = *(ent**)(ModuleBase + 0x10f4f4);

	bool bHealth{ false };
	bool bAmmo{ false };
	bool bRecoil{ false };
	bool bArmour{ false };
	bool bGrenade{ false };
	bool bOneShotOneKill{ false };

	const int newValue{ 9999 };
	const int HnewValue{ newValue };
	const int ARnewValue{ newValue };
	const int AnewValue{ newValue };
	const int GnewValue{ newValue };
	// Hack loop

	while ( !GetAsyncKeyState( VK_END ) & 1 )
	{
		// key input

		if ( GetAsyncKeyState( VK_NUMPAD1 ) & 1 )
		{
			if ( bHealth )
			{
				std::cout << "Unlimited Health hack disabled!!\n";
				std::cout << '\n';
				bHealth = !bHealth;
			}
			else
			{
				std::cout << "Unlimited Health hack enabled!!\n";
				std::cout << '\n';
				bHealth = !bHealth;
			}
		}

		if ( GetAsyncKeyState( VK_NUMPAD2 ) & 1 )
		{
			if ( bArmour )
			{
				std::cout << "Unlimited Armour hack disabled!!\n";
				std::cout << '\n';
				bArmour = !bArmour;
			}
			else
			{
				std::cout << "Unlimited Armour hack enabled!!\n";
				std::cout << '\n';
				bArmour = !bArmour;
			}
		}

		if ( GetAsyncKeyState( VK_NUMPAD3 ) & 1 )
		{
			if ( bAmmo )
			{
				std::cout << "Unlimited Ammo hack disabled!!\n";
				std::cout << '\n';
				bAmmo = !bAmmo;
			}
			else
			{
				std::cout << "Unlimited Ammo hack enabled!!\n";
				std::cout << '\n';
				bAmmo = !bAmmo;
			}
		}

		if ( GetAsyncKeyState( VK_NUMPAD4 ) & 1 )
		{
			bRecoil = !bRecoil;
			
			if ( bRecoil )
			{
				std::cout << "Recoil hack enabled!\n\n";
				mem::nop( (BYTE*)(ModuleBase + 0x63786), 10 );
			}
			else
			{
				std::cout << "Recoil hack disabled!\n\n";
				mem::patch( (BYTE*)(ModuleBase + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10 );
			}
		}
			
		if ( GetAsyncKeyState( VK_NUMPAD5 ) & 1 )
		{
			if ( bGrenade )
			{
				std::cout << "Unlimited Grenade hack disabled!!\n";
				std::cout << '\n';
				bGrenade = !bGrenade;
			}
			else
			{
				std::cout << "Unlimited Grenade hack enabled!!\n";
				std::cout << '\n';
				bGrenade = !bGrenade;
			}
		}

		// Continuos write/freeze

		// check against 'hackable' state
		if ( pLocalPlayerBaseAddr )
		{
			if ( bHealth )
			{
				pLocalPlayerBaseAddr->Health = HnewValue;
			}

			if ( bAmmo )
			{
				pLocalPlayerBaseAddr->currentWeapon->ammoPtr->ammo = AnewValue;

			}

			if ( bArmour )
			{
				pLocalPlayerBaseAddr->Armour = ARnewValue;
			}

			if ( bGrenade )
			{
				pLocalPlayerBaseAddr->numGrenades = GnewValue;
			}
		}

		Sleep( 5 );
	}   

	// Cleanup & eject
	if ( f )
		fclose( f );
	FreeConsole( );
	FreeLibraryAndExitThread( hModule, 0 );

	return 0;
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	switch ( fdwReason )
	{
		case DLL_PROCESS_ATTACH:

			::DisableThreadLibraryCalls( hinstDLL );
			CloseHandle(CreateThread( nullptr, 0, LPTHREAD_START_ROUTINE( myThreadProc ), hinstDLL, 0, nullptr ));
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			//perform any necesary clean-up
			break;

	}
	return TRUE; //Succesful DLL_PROCESS_ATTACH
}
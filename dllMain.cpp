#include<Windows.h>
#include<iostream>
#include "mem.h"

DWORD WINAPI myThreadProc( HMODULE hModule )
{
	// Create console

	AllocConsole( ); // To allocate console for logging
	FILE* f;
	freopen_s( &f, "CONOUT$", "w", stdout );
	std::cout << "OG for a fee, stay sippin' fam\n";

	// Get module base

	uintptr_t ModuleBase{ (uintptr_t)GetModuleHandle( L"ac_client.exe" ) };
	uintptr_t localPlayerBaseAddr = ModuleBase + 0x10f4f4;//ModuleBase + 0x10f4f4 };
	uintptr_t* pLocalPlayerBaseAddr = (uintptr_t*)(localPlayerBaseAddr) ;

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
				*(int*)(*pLocalPlayerBaseAddr + 0xf8) = HnewValue;
			}

			if ( bAmmo )
			{
				uintptr_t ammoAddr{ mem::findDMAAddy( localPlayerBaseAddr, {0x374, 0x14, 0x0} ) };
				int* ammo{ (int*)ammoAddr };
				*ammo = AnewValue;

				// Or
				//*(int*)(mem::findDMAAddy( pLocalPlayer, { 0x374,0x14,0x0 } )) = 1337;
			}

			if ( bArmour )
			{
				*(int*)(*pLocalPlayerBaseAddr + 0xFC) = ARnewValue;
			}

			if ( bGrenade )
			{
				*(int*)(*pLocalPlayerBaseAddr + 0x158) = GnewValue;
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
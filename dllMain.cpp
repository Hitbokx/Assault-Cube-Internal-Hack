#include<Windows.h>
#include<iostream>
#include "mem.h"
#include "Class.h"
#include "HookingClass.h"
#include <functional>
#include "glDraw.h"
#include "glText.h"

DWORD g_jmpBackAdd{};
DWORD g_team{};

using tGetCrossHairEnt = Player*(__cdecl*)();
using tPrintF = char* (__cdecl*)(const char* format, ...);
using twglSwapBuffer = BOOL( __stdcall* )(HDC hdc);

twglSwapBuffer wglSwapBuffersGateway; 

uintptr_t ModuleBase{ (uintptr_t)GetModuleHandle( L"ac_client.exe" ) };
tGetCrossHairEnt getCrossHairEnt{ (tGetCrossHairEnt)(ModuleBase + 0x607c0) };
tPrintF printF{ (tPrintF)(ModuleBase + 0x6B060) };

Player* localPlayerBaseAddr{ *(Player**)(ModuleBase + 0x10f4f4) };

const char* format{ "%s:\f%d %s" };
bool bInvincible{ false };
bool bRecoil{ false };
bool bOneShotOneKill{ false };
bool bChangeWeapon{ false };
bool bTeam{ false };
bool bJump{ false };
bool bFly{ false };
bool bHop{ false };
bool bSpeed{ false };
bool bTriggerbot{ false };
bool bToggleState{ false };
bool bTeleport{ false };

const int newValue{ 9999 };
const int HnewValue{ newValue };
const int ARnewValue{ newValue };
const int AnewValue{ newValue };
const int GnewValue{ newValue };

void __declspec(naked) ourFunc( )
{
	__asm
	{
		mov eax, g_team
		cmp[ebx + 0x32C], eax
		jne originalcode
		sub[ebx + 04], edi
		mov eax, edi
		jmp exit

		originalcode :
		mov[ebx + 04], 0
			mov eax, edi

			exit :
		jmp[g_jmpBackAdd]
	}
}

GL::Font glFont{};
const int FONT_HEIGHT{ 15 };
const int FONT_WIDTH{ 9 };
const char* example{ "ESP Box" };
const char* example2{ "I'm inside fam" };

void Draw( )
{
	HDC currHDC{ wglGetCurrentDC( ) };
	if ( !glFont.bBuilt || currHDC != glFont.hDc )
	{
		glFont.Build( FONT_HEIGHT );
	}

	GL::setupOrtho( );

	GL::drawOutline( 320, 240, 200, 200, 2.0f, rgb::red );

	float textPointX{ glFont.centerText( 320, 200, strlen( example ) * FONT_WIDTH ) };
	float textPointY{ 240 - FONT_HEIGHT / 2 };

	glFont.Print( textPointX, textPointY, rgb::green, "%s", example );

	Vector3 insideTextPoint{ glFont.centerText( 320, 240 + 100, 200, 200, strlen( example2 ) * FONT_WIDTH, FONT_HEIGHT ) };
	glFont.Print( insideTextPoint.x, insideTextPoint.y, rgb::green, "%s", example2 );

	GL::restoreGL( );
}

BOOL __stdcall hkwglSwapBuffers( HDC hdc )
{

	if ( GetAsyncKeyState( VK_NUMPAD0 ) & 1 )
	{
		if ( bInvincible )
		{
			printF( format, "HACKER", 1, "'INVINCIBILITY' disabled!!" );

			bInvincible = !bInvincible;
		}
		else
		{
			printF( format, "HACKER", 1, "You are now 'INVINCIBLE'!!" );

			bInvincible = !bInvincible;
		}
	}

	if ( GetAsyncKeyState( VK_NUMPAD1 ) & 1 )
	{
		bRecoil = !bRecoil;

		if ( bRecoil )
		{
			printF( format, "HACKER", 1, "Recoil hack enabled!" );

			mem::nop( (BYTE*)(ModuleBase + 0x63786), 10 );
		}
		else
		{
			printF( format, "HACKER", 1, "Recoil hack disabled!" );

			mem::patch( (BYTE*)(ModuleBase + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10 );
		}
	} // Recoil

	if ( GetAsyncKeyState( VK_NUMPAD2 ) & 1 ) // One Shot One kill
	{
		bOneShotOneKill = !bOneShotOneKill;

		if ( bOneShotOneKill )
		{
			printF( format, "HACKER", 1, "One Shot One Kill hack eabled!!" );

			int hookLength{ 5 };
			DWORD hookAdd{ ModuleBase + 0x29D1F };
			g_jmpBackAdd = hookAdd + hookLength;
			g_team = localPlayerBaseAddr->team;

			g_hook.midDetour32<5>( (BYTE*)hookAdd, (BYTE*)&ourFunc );
		}
		else
		{
			printF( format, "HACKER", 1, "One Shot One Kill hack disabled!!" );

			mem::patch( (BYTE*)(ModuleBase + 0x29D1F), (BYTE*)"\x29\x7B\x04\x8B\xC7", 5 );
		}
	}

	if ( GetAsyncKeyState( VK_NUMPAD3 ) & 1 )
	{
		bChangeWeapon = !bChangeWeapon;

		if ( bChangeWeapon )
		{
			std::cout << "To which WEAPON do you want to change?\n";
			std::cout << "Press: \n";
			std::cout << "'0' for KNIFE \n";
			std::cout << "'1' for PISTOL \n";
			std::cout << "'2' for CARBINE \n";
			std::cout << "'3' for SHOTTY \n";
			std::cout << "'4' for SUBMACHINE GUN \n";
			std::cout << "'5' for SNIPER \n";
			std::cout << "'6' for ASSAULT \n";
			std::cout << "'7' for CPISTOL \n";
			std::cout << "'8' for GRENADE \n";
			std::cout << "'9' for AKIMBO \n";
			//std::cout << "'INSERT' for EXIT \n";
			std::cout << '\n';

			while ( !(GetAsyncKeyState( VK_INSERT ) & 1) )
			{
				localPlayerBaseAddr->nextWeapon = localPlayerBaseAddr->currentWeapon;

				if ( GetAsyncKeyState( VK_NUMPAD0 ) & 1 )
				{
					localPlayerBaseAddr->currentWeapon = localPlayerBaseAddr->knifePtr;
					break;
				}

				if ( GetAsyncKeyState( VK_NUMPAD1 ) & 1 )
				{
					localPlayerBaseAddr->currentWeapon = localPlayerBaseAddr->pistolPtr;
					localPlayerBaseAddr->currentWeapon->ammoClip->ammo = AnewValue;
					break;
				}

				if ( GetAsyncKeyState( VK_NUMPAD2 ) & 1 )
				{
					localPlayerBaseAddr->currentWeapon = localPlayerBaseAddr->carbinePtr;
					localPlayerBaseAddr->currentWeapon->ammoClip->ammo = AnewValue;
					break;
				}

				if ( GetAsyncKeyState( VK_NUMPAD3 ) & 1 )
				{
					localPlayerBaseAddr->currentWeapon = localPlayerBaseAddr->shotgunPtr;
					localPlayerBaseAddr->currentWeapon->ammoClip->ammo = AnewValue;
					break;
				}

				if ( GetAsyncKeyState( VK_NUMPAD4 ) & 1 )
				{
					localPlayerBaseAddr->currentWeapon = localPlayerBaseAddr->subGunPtr;
					localPlayerBaseAddr->currentWeapon->ammoClip->ammo = AnewValue;
					break;
				}

				if ( GetAsyncKeyState( VK_NUMPAD5 ) & 1 )
				{
					localPlayerBaseAddr->currentWeapon = localPlayerBaseAddr->sniperPtr;
					localPlayerBaseAddr->currentWeapon->ammoClip->ammo = AnewValue;
					break;
				}

				if ( GetAsyncKeyState( VK_NUMPAD6 ) & 1 )
				{
					localPlayerBaseAddr->currentWeapon = localPlayerBaseAddr->assaultPtr;
					localPlayerBaseAddr->currentWeapon->ammoClip->ammo = AnewValue;
					break;
				}

				if ( GetAsyncKeyState( VK_NUMPAD7 ) & 1 )
				{
					std::cout << "Sorry! cPistol is currently unavailable.\n";
					//localPlayerBaseAddr->currentWeapon = localPlayerBaseAddr->cpistolPtr;
					break;
				}

				if ( GetAsyncKeyState( VK_NUMPAD8 ) & 1 )
				{
					localPlayerBaseAddr->currentWeapon = localPlayerBaseAddr->grenadePtr;
					localPlayerBaseAddr->currentWeapon->ammoClip->ammo = AnewValue;
					break;
				}

				if ( GetAsyncKeyState( VK_NUMPAD9 ) & 1 )
				{
					localPlayerBaseAddr->currentWeapon = localPlayerBaseAddr->akimboPtr;
					localPlayerBaseAddr->currentWeapon->ammoClip->ammo = AnewValue;
					break;
				}
			}
			std::cout << "You now have " << localPlayerBaseAddr->currentWeapon->namePtr->name << ".\n";
		}
		else
		{
			std::cout << "Any Weapon hack disabled!!\n";
			std::cout << '\n';

			if ( localPlayerBaseAddr )
			{
				localPlayerBaseAddr->currentWeapon = localPlayerBaseAddr->nextWeapon;
			}
		}
	}

	if ( GetAsyncKeyState( VK_NUMPAD4 ) & 1 )
	{
		bTeam = !bTeam;

		if ( localPlayerBaseAddr )
		{
			if ( bTeam )
			{
				if ( localPlayerBaseAddr->team )
				{
					localPlayerBaseAddr->team = 0;
				}
				else
				{
					localPlayerBaseAddr->team = 1;
				}

				printF( format, "HACKER", 1, "Team switched." );

				if ( localPlayerBaseAddr->team )
				{
					printF( format, "Now, your current team is ", 1, "blue." );
				}
				else
				{
					printF( format, "Now, your current team is ", 1, "red." );
				}
				bTeam = !bTeam;
			}
		}
	}

	if ( GetAsyncKeyState( VK_NUMPAD5 ) & 1 )
	{
		if ( bTriggerbot )
		{
			printF( format, "HACKER", 1, "Triggerbot disabled." );

			bTriggerbot = !bTriggerbot;
		}
		else
		{
			printF( format, "HACKER", 1, "Triggerbot enabled." );

			bTriggerbot = !bTriggerbot;
		}

	}

	if ( GetAsyncKeyState( VK_NUMPAD6 ) & 1 )
	{
		bHop = !bHop;

		if ( bHop )
		{
			printF( format, "HACKER", 1, "B_HOP hack enabled." );

			mem::patch( (BYTE*)(ModuleBase + 0x5C03D), (BYTE*)"\xB2\x99\x88\x50\x6B\x90\x90", 7 );

		}
		else
		{
			printF( format, "HACKER", 1, "B_HOP hack disabled." );

			mem::patch( (BYTE*)(ModuleBase + 0x5C03D), (BYTE*)"\x8A\x54\x24\x04\x88\x50\x6B", 7 );
		}
	}

	if ( GetAsyncKeyState( VK_NUMPAD7 ) & 1 )
	{
		if ( bJump )
		{
			printF( format, "HACKER", 1, "Continuos JUMP hack disabled." );

			bJump = !bJump;
		}
		else
		{
			printF( format, "HACKER", 1, "Continuos JUMP hack enabled." );

			bJump = !bJump;
		}
	}

	if ( GetAsyncKeyState( VK_NUMPAD8 ) & 1 )
	{
		bSpeed = !bSpeed;

		if ( bSpeed )
		{
			printF( format, "HACKER", 1, "Speed Hack enabled!!" );

			mem::patch( (BYTE*)(ModuleBase + 0x5BEA0), (BYTE*)"\xB8\x03\x00\x00\x00", 5 );
		}
		else
		{
			printF( format, "HACKER", 1, "Speed Hack disabled!!" );

			mem::patch( (BYTE*)(ModuleBase + 0x5BEA0), (BYTE*)"\xB8\x01\x00\x00\x00", 5 );
		}
	}

	if ( GetAsyncKeyState( VK_NUMPAD9 ) & 1 )
	{
		bToggleState = !bToggleState;

		if ( bToggleState )
		{
			printF( format, "HACKER", 1, "GODMODE enabled." );

			localPlayerBaseAddr->entSpecMode = 5;
			localPlayerBaseAddr->playerState = 11;
		}
		else
		{
			printF( format, "HACKER", 1, "GODMODE disabled." );

			localPlayerBaseAddr->entSpecMode = 0;
			localPlayerBaseAddr->playerState = 0;
		}
	}

	if ( GetAsyncKeyState( VK_F12 ) & 1 )
	{
		bTeleport = !bTeleport;

		if ( bTeleport )
		{
			printF( format, "HACKER", 1, "You are telported to enemy spawn." );

			localPlayerBaseAddr->playerCoordinates = { 73.13, 183.82, 0 };
		}
		else
		{
			printF( format, "HACKER", 1, "You are telported back to your spawn." );

			localPlayerBaseAddr->playerCoordinates = { 219.74, 39.83, 1 };
		}
	}
	// Continuos write/freeze
	// check against 'hackable' state
	if ( localPlayerBaseAddr )
	{
		if ( bInvincible )
		{
			localPlayerBaseAddr->health = HnewValue;
			localPlayerBaseAddr->currentWeapon->ammoClip->ammo = AnewValue;
			localPlayerBaseAddr->armour = ARnewValue;
			localPlayerBaseAddr->numGrenades = GnewValue;
		}

		if ( bTriggerbot )
		{
			Player* pCrossHairEnt{ getCrossHairEnt( ) };

			if ( pCrossHairEnt )
			{
				if ( pCrossHairEnt->team != localPlayerBaseAddr->team )
					localPlayerBaseAddr->isFiring = 1;
			}
			else
				localPlayerBaseAddr->isFiring = 0;
		}

		if ( bJump )
		{
			localPlayerBaseAddr->isJumping = true;
		}
	}

	Draw( );
	return wglSwapBuffersGateway( hdc );
}

DWORD WINAPI myThreadProc( HMODULE hInstDLL )
{
	AllocConsole( ); // To allocate console for logging
	FILE* f;
	freopen_s( &f, "CONOUT$", "w", stdout );
	std::cout << "Yo! Hitbokx here.\n";

	g_hook.hookFunc<5>( "wglSwapBuffers", "opengl32.dll", (BYTE*)hkwglSwapBuffers, (BYTE*)&wglSwapBuffersGateway );

	while ( !(GetAsyncKeyState( VK_END ) & 1) )
	{

	}

	
	g_hook.unHook<5>( nullptr );

	if ( f )
		fclose( f );
	FreeConsole( );
	FreeLibraryAndExitThread( hInstDLL, 0 );//to call DLL_PROCESS_DETACH and resource deallocation
	return 0;
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	switch ( fdwReason )
	{
		case DLL_PROCESS_ATTACH:

			::DisableThreadLibraryCalls( hinstDLL );
			CloseHandle( CreateThread( nullptr, 0, LPTHREAD_START_ROUTINE( myThreadProc ), hinstDLL, 0, nullptr ) );
			break;

		case DLL_PROCESS_DETACH:
			//perform any necesary clean-up
			break;

	}
	return TRUE; //Succesful DLL_PROCESS_ATTACH
}
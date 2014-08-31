#include<iostream>
#include "Vector.h"

class Player
{
public:
	Vector3 headCoordinates; //0x0004
	Vector3 velocity; //0x0010
	int32_t directionX; //0x001C
	int32_t directionY; //0x0020
	int32_t JUMP; //0x0024
	Vector3 displacement; //0x0028
	Vector3 playerCoordinates; //0x0034
	Vector3 viewAngles; //0x0040
	float screenMoveUpWhileFiring; //0x004C
	char pad_0050[4]; //0x0050
	int32_t mSecInAir; //0x0054
	char pad_0058[17]; //0x0058
	bool isJumpKeyNotPressed; //0x0069
	char pad_006A[1]; //0x006A
	bool isJumping; //0x006B
	bool isCrouchKeyPressed; //0x006C
	char pad_006D[1]; //0x006D
	bool isCrouching; //0x006E
	char pad_006F[1]; //0x006F
	int32_t isNotMoving; //0x0070
	char pad_0074[12]; //0x0074
	int8_t speedX; //0x0080
	char pad_0081[1]; //0x0081
	int8_t playerState; //0x0082
	bool isInvisible; //0x0083
	char pad_0084[8]; //0x0084
	int32_t speedY; //0x008C
	char pad_0090[104]; //0x0090
	int32_t health; //0x00F8
	int32_t armour; //0x00FC
	char pad_0100[12]; //0x0100
	int32_t hasAikimbo; //0x010C
	int32_t knifeMagazineAmmo; //0x0110
	int32_t pistolMagazineAmmo; //0x0114
	int32_t carbineMagazineAmmo; //0x0118
	int32_t shotgunMagazineAmmo; //0x011C
	int32_t subgunMagazineAmmo; //0x0120
	int32_t sniperMagazineAmmo; //0x0124
	int32_t assaultMagazineAmmo; //0x0128
	int32_t cpistolMagazineAmmo; //0x012C
	int32_t grenadeMagazineAmmo; //0x0130
	int32_t akimboMagazineAmmo; //0x0134
	int32_t knifeClipAmmo; //0x0138
	int32_t pistolClipAmmo; //0x013C
	int32_t carbineClipAmmo; //0x0140
	int32_t shotgunClipAmmo; //0x0144
	int32_t subgunClipAmmo; //0x0148
	int32_t sniperClipAmmo; //0x014C
	int32_t assaultClipAmmo; //0x0150
	int32_t cpistolClipAmmo; //0x0154
	int32_t numGrenades; //0x0158
	int32_t akimboClipAmmo; //0x015C
	int32_t knifeHitDelay; //0x0160
	int32_t pistolFireDelay; //0x0164
	int32_t carbineFireDelay; //0x0168
	int32_t shotgunFireDelay; //0x016C
	int32_t subgunFireDelay; //0x0170
	int32_t sniperFireDelay; //0x0174
	int32_t assaultFireDelay; //0x0178
	int32_t cpistolFireDelay; //0x017C
	int32_t grenadeThrowDelay; //0x0180
	int32_t akimboFireDelay; //0x0184
	int32_t knifeHits; //0x0188
	int32_t pistolShotsTaken; //0x018C
	int32_t carbineShotsTaken; //0x0190
	int32_t shotgunShotsTaken; //0x0194
	int32_t subgunShotsTaken; //0x0198
	int32_t sniperShotsTaken; //0x019C
	int32_t assaultShots; //0x01A0
	int32_t cpistolShotsTaken; //0x01A4
	int32_t grenadeHits; //0x01A8
	int32_t akimboShots; //0x01AC
	char pad_01B0[72]; //0x01B0
	int32_t numDeaths; //0x01F8
	int32_t numKills; //0x01FC
	char pad_0200[36]; //0x0200
	bool isFiring; //0x0224
	char N000001BF[16]; //0x0225
	char pad_0235[247]; //0x0235
	int32_t team; //0x032C
	char pad_0330[8]; //0x0330
	int8_t entSpecMode; //0x0338
	char pad_0339[15]; //0x0339
	class weapon* knifePtr; //0x0348
	class weapon* pistolPtr; //0x034C
	class weapon* carbinePtr; //0x0350
	class weapon* shotgunPtr; //0x0354
	class weapon* subGunPtr; //0x0358
	class weapon* sniperPtr; //0x035C
	class weapon* assaultPtr; //0x0360
	class weapon* cPistolPtr; //0x0364
	class weapon* grenadePtr; //0x0368
	class weapon* akimboPtr; //0x036C
	class weapon* previousWeapon; //0x0370
	class weapon* currentWeapon; //0x0374
	class weapon* nextWeapon; //0x0378
	char pad_037C[260]; //0x037C

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
}; //Size: 0x0480
static_assert(sizeof( Player ) == 0x480);

class weapon
{
public:
	int32_t index; //0x0004
	class Player* ownerPtr; //0x0008
	class name* namePtr; //0x000C
	class ammoPtr* ammoReserve; //0x0010
	class ammoPtr* ammoClip; //0x0014
	class fireDelayPtr* fireDelay; //0x0018
	int32_t shotsHitOnceTriggered; //0x001C
	char pad_0020[16]; //0x0020

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
}; //Size: 0x0030
static_assert(sizeof( weapon ) == 0x30);

class name
{
public:
	char name[16]; //0x0000
}; //Size: 0x0010
static_assert(sizeof( name ) == 0x10);

class ammoPtr
{
public:
	int32_t ammo; //0x0000
}; //Size: 0x0004
static_assert(sizeof( ammoPtr ) == 0x4);

class fireDelayPtr
{
public:
	int32_t fireDelay; //0x0000
}; //Size: 0x0004
static_assert(sizeof( fireDelayPtr ) == 0x4);
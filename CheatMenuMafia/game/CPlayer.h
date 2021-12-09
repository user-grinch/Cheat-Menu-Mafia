#pragma once
#include "CVector.h"

class CPlayer
{
public:
	char pad_0x0000[0x24]; 
	CVector Position; 
	CVector Rotation; 
	CVector InitialPosition; 
	CVector InitialRotation; 
	char pad_0x0054[0x24]; 
	CVector N000001C3; 
	char pad_0x0084[0x8]; 
	float N000001C8; 
	float N000001C9; 
	char pad_0x0094[0x74]; 
	CVector N000001E7; 
	char pad_0x0114[0x38C]; 
	int CurrentGunType; 
	int Ammo; 
	int AmmoInClip; 
	char pad_0x04AC[0x158]; 
	float HealthMax; 
	char pad_0x0608[0x3C]; 
	float Health; 
	char pad_0x0648[0x57C]; 
}; 

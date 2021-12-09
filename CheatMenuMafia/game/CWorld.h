#pragma once
#include "CCamera.h"
#include "CPlayer.h"
#include "CVehicle.h"

class CWorld
{
public:
	char pad_0x0000[0x4]; 
	void* N00000014; 
	void* N00000015; 
	void* N00000016; 
	char pad_0x0010[0x40]; 
	CCamera* pCamera; 
	CPlayer* pPlayer; 
	CVehicle* pVehicle; 
	char pad_0x005C[0x18]; 
	float N00000030; 
	char pad_0x0078[0x6C]; 
	CPlayer* pPlayer2; 
	CPlayer* pPlayer3; 
	char pad_0x00EC[0x8]; 
	void* Interacables; 
	char pad_0x00F8[0x30C]; 

	static CWorld* GetInstance()
	{
		return *reinterpret_cast<CWorld**>(0x647E1C);
	}
}; 


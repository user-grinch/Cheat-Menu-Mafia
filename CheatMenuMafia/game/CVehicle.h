#pragma once
#include "CVector.h"

class CVehicle
{
public:
	char pad_0x0000[0x24]; 
	CVector N000004E5; 
	CVector N00000520; 
	CVector N00000522; 
	CVector N00000524; 
	char pad_0x0054[0x24]; 
	CVector N000004FA; 
	char pad_0x0084[0x10]; 
	CVector N00000501; 
	char pad_0x00A0[0x1C]; 
	CVector N0000050B; 
	CVector N0000050E; 
	char pad_0x00D4[0x4]; 
	CVector N00000512; 
	CVector N00000515; 
	CVector N00000518; 
	CVector N0000057B; 
	char pad_0x0108[0x1A4]; 
	CVector N000005B9; 
	char pad_0x02B8[0x3C]; 
	CVector N000005CB; 
	char pad_0x0300[0x90]; 
	CVector Pos; 
	char pad_0x039C[0x6C]; 
	CVector N00000634; 
	char pad_0x0414[0x170]; 
}; 


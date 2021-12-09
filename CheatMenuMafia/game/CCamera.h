#pragma once
#include "CMatrix.h"

class CCamera
{
public:
	char pad_0x0000[0x10]; 
	CMatrix matrix1; 
	CMatrix matrix2; 
	char pad_0x0090[0xC]; 
	float N0000013D; 
	float N0000013E; 
	float N0000013F; 
	float N00000140; 
	char pad_0x00AC[0xB8]; 
	CMatrix N0000096C; 
	char pad_0x01A4[0x40]; 
	CMatrix N0000098D; 
	CMatrix N0000099D; 
	char pad_0x0264[0x120]; 
}; 


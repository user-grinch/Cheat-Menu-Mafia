#pragma once

class CHud
{
public:
	char pad_0x0000[0x4210]; 
	int Health; 
	int Ammo; 
	int AmmoInClip; 

	static CHud* GetInstance()
	{
		return reinterpret_cast<CHud*>(0x6BF980);
	}
}; 


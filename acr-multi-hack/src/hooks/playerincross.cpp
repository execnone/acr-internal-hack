#include <pch.h>
#include "base.h"

playerent* __cdecl Base::Hooks::playerincrosshair()
{
	playerent* temp;
	int temp2;
	vec temp3;

	Base::Data::o_playerincrosshair(temp, temp2, temp3);
	return (playerent*)*(DWORD*)0x00565770;
}
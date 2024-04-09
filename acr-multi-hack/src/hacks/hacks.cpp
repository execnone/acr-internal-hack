#include <pch.h>
#include "hacks.h"
#include <base.h>
#include <cmath>

void hacks::Init()
{
	offsets::acr_client = (DWORD)mem::in::get_module("acr_client.exe").base;
	Base::Data::rect = *(window_res*)(offsets::acr_client + offsets::resolution);
}

bool IsVisible(vec from, vec to)
{
	bool result = false;

	__asm
	{
		pushad
		push to.z
		push to.y
		push to.x
		push from.z
		push from.y
		push from.x
		xor eax, eax
		call Base::Data::pIsVisible
		mov result, al
		add esp, 24
		popad
	}

	return result;
}

void hacks::Run()
{
	//hacks::local_player = (playerent*)*(DWORD*)(offsets::acr_client + offsets::local_player);
	//hacks::entity_list = (DWORD*)(offsets::entity_list);
	memcpy(&hacks::local_player, (const void*)(offsets::acr_client + offsets::local_player), 4);
	memcpy(&hacks::entity_list, (const void*)offsets::entity_list, 4);
	memcpy(&hacks::entity_number, (const void*)offsets::entity_number, 4);
	//hacks::entity_number = *(DWORD*)(offsets::entity_number);

	ESP();
}

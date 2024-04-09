#pragma once

bool WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight);
namespace hacks
{
	void Init();
	void Run();
	void ESP();

	playerent* local_player;
	DWORD entity_list;
	DWORD entity_number;
	float viewmatrix[16];
}
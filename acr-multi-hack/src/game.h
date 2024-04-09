#pragma once
#include <pch.h>
#include "sdk.h"

struct Vec4 {
	float x, y, z, w;
};

struct Vec3 {
	float x, y, z;
	vec to_vec()
	{
		return vec(x,y,z);
	}
};

struct Vec2 {
	float x, y;
};

struct window_res {
	window_res(int x, int y) {
		x = x;
		y = y;
	}
	int x, y;
};

struct render {
	void Box(Vec2 entPosFeet2D, Vec2 entPosHead2D, ImU32 color);
	void Snapline(ImVec2 to_pos, ImU32 color);
};

/*
struct playerent {
	char unknown3[0x21c];
	int team;
	char unknown[0x74];
	int health;
	char unknown2[0x1b0];
	Vec3 world;

    bool IsValid();
    bool IsVisible(vec from, vec to);
	render draw;
}; */

struct playerent {
	char unknown3[0x34];
	Vec3 world;
	Vec2 screen;
	char unknown2[0xB8];
	int health; // 4294966194 == dead
	char unknown4[0x33C];
	int team;

	bool IsValid();
	bool IsVisible();
	render draw;
}; // team +440

namespace offsets
{
	DWORD acr_client = 0;
	DWORD entity_list = 0x569468;//0x56E7C4;
	DWORD entity_number = 0x56E7CC;
	DWORD local_player = 0x16576C;
	DWORD viewmatrix = 0x005657D0;
	DWORD resolution = 0x169734;
}
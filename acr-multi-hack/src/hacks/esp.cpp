#include <pch.h>
#include <base.h>
#include "hacks.h"

void hacks::ESP()
{
	for (int i = 1; i < hacks::entity_number; i++)
	{
		playerent* player = (playerent*)(*(DWORD*)(hacks::entity_list + i * 4));
		Vec3 head_pos = player->world;
		head_pos.z += 5;

		if (player->IsValid())
		{
			memcpy(&hacks::viewmatrix, (PBYTE*)offsets::viewmatrix, sizeof(hacks::viewmatrix));
			Vec2 entPosHead2D;
			Vec2 entPosFeet2D;

			if (WorldToScreen(head_pos, entPosHead2D, hacks::viewmatrix, Base::Data::rect.x, Base::Data::rect.y) &&
				WorldToScreen(player->world, entPosFeet2D, hacks::viewmatrix, Base::Data::rect.x, Base::Data::rect.y))
			{
				if (player->IsVisible())
				{
					player->draw.Snapline(ImVec2(entPosFeet2D.x, entPosFeet2D.y), ImGui::GetColorU32(ImVec4(0, 255, 0, 255)));
					player->draw.Box(entPosFeet2D, entPosHead2D, ImGui::GetColorU32(ImVec4(0, 255, 0, 255)));
				}
				else
				{
					player->draw.Snapline(ImVec2(entPosFeet2D.x, entPosFeet2D.y), ImGui::GetColorU32(ImVec4(255, 0, 0, 255)));
					player->draw.Box(entPosFeet2D, entPosHead2D, ImGui::GetColorU32(ImVec4(255, 0, 0, 255)));
				}
			}
		}
	}
}


bool WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight)
{
	Vec4 clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}
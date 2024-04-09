#include "pch.h"
#include <base.h>

bool playerent::IsValid()
{
    return (this != NULL && health > 0 && health <= 1200 && team != hacks::local_player->team);
}

bool playerent::IsVisible()
{
	bool result = false;
	vec from = hacks::local_player->world.to_vec(); from.z += 6;
	vec to = this->world.to_vec(); to.z += 6;

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

void render::Box(Vec2 entPosFeet2D, Vec2 entPosHead2D, ImU32 color)
{
	float height = (entPosFeet2D.y - entPosHead2D.y) * 1.09;
	float width = height * 0.6;

	ImVec2 pos = ImVec2(entPosFeet2D.x - width / 2, entPosHead2D.y - height * 0.08);
	ImVec2 size = ImVec2(pos.x + width, pos.y + height);

	ImGui::GetBackgroundDrawList()->AddRect(pos, size, color, 0.0f, NULL, 2.0f);
}

void render::Snapline(ImVec2 to_pos, ImU32 color)
{
	ImGui::GetBackgroundDrawList()->AddLine
		(ImVec2(Base::Data::rect.x / 2, Base::Data::rect.y), to_pos, color);
}

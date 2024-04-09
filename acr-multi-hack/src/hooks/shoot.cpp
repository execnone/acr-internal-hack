#include <pch.h>
#include <base.h>

float DistanceBetweenCross(float X, float Y)
{
	float ydist = (Y - Base::Data::rect.y / 2);
	float xdist = (X - Base::Data::rect.x / 2);
	float Hypotenuse = sqrt(pow(ydist, 2) + pow(xdist, 2));
	return Hypotenuse;
}

playerent* GetNearestEnemy()
{
	playerent* best_enemy = NULL;

	for (int i = 1; i < hacks::entity_number; i++)
	{
		playerent* player = (playerent*)(*(DWORD*)(hacks::entity_list + i * 4));

		if (player->IsValid() && player->IsVisible())
		{
			if (best_enemy == NULL) {
				best_enemy = player;
				continue;
			}

			memcpy(&hacks::viewmatrix, (PBYTE*)offsets::viewmatrix, sizeof(hacks::viewmatrix));
			Vec2 entPosHead2D;
			Vec2 bestPosHead2D;

			Vec3 head_pos = player->world;
			head_pos.z += 4.5f;

			Vec3 best_head_pos = best_enemy->world;
			best_head_pos.z += 4.5f;

			if (WorldToScreen(head_pos, entPosHead2D, hacks::viewmatrix, Base::Data::rect.x, Base::Data::rect.y) &&
				WorldToScreen(best_head_pos, bestPosHead2D, hacks::viewmatrix, Base::Data::rect.x, Base::Data::rect.y))
			{
				float bestfov = fabs(DistanceBetweenCross(bestPosHead2D.x, bestPosHead2D.y));
				float fov = fabs(DistanceBetweenCross(entPosHead2D.x, entPosHead2D.y));

				if (fov < bestfov) {
					best_enemy = player;
				}
			}
		}
	}

	return best_enemy;
}

void Base::Hooks::shoot()
{
	int* addr1 = (int*)hacks::local_player;
	__asm
	{
		push ecx
		push edx
		cmp ecx, addr1
		jne exit_hook
	}

	playerent* player5 = GetNearestEnemy();//(playerent*)(*(DWORD*)(hacks::entity_list + 1 * 4));
	if (!player5->IsValid() || !player5->IsVisible()) {
		__asm{
			pop edx
			jmp exit_hook
		}
	}

	vec test5 = player5->world.to_vec();
	test5.z += 4.5f;
	int* addr2 = &(int&)test5;

	__asm {
		pop edx
		mov edx, addr2
		jmp exit_hook
	}

	__asm
	{
	exit_hook :
		pop ecx
		call Base::Data::oShoot
	}
}
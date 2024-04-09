#include <pch.h>
#include <base.h>

bool Base::Hooks::Init()
{
	Data::pSwapBuffers = mem::in::get_symbol(mem::in::get_module("opengl32.dll"), "wglSwapBuffers");
	Data::oSwapBuffers = (SwapBuffers_t)mem::in::detour_trampoline(Data::pSwapBuffers, Hooks::SwapBuffers, Data::szSwapBuffers, mem::MEM_DT_M1);
	Data::p_glDrawElements = *(mem::voidptr_t*)((DWORD)mem::in::get_module("nvoglv32.dll").base + 0x1C9ECB8);//0x1C96F38);//mem::in::get_symbol(mem::in::get_module("opengl32.dll"), "glDrawElements");
	Data::o_glDrawElements = (glDrawElements_t)mem::in::detour_trampoline(Data::p_glDrawElements, Hooks::glDrawRangeElements, 5, mem::MEM_DT_M1);
	Data::pIsVisible = (mem::voidptr_t)0x00474F40;
	Data::p_playerincrosshair = (mem::voidptr_t)0x00511350;
	Data::o_playerincrosshair = (playerincrosshair_t)mem::in::detour_trampoline(Data::p_playerincrosshair, Hooks::playerincrosshair, 11, mem::MEM_DT_M1);
	Data::pShoot = (mem::voidptr_t)0x00512D70;//;0x004848F1
	Data::oShoot = (shoot_t)mem::in::detour_trampoline(Data::pShoot, shoot, 8, mem::MEM_DT_M1);

	return true;
}

bool Base::Hooks::Shutdown()
{
	if (Data::InitImGui)
	{
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	mem::in::detour_restore(Data::pSwapBuffers, (mem::byte_t*)Data::oSwapBuffers, Data::szSwapBuffers);
	mem::in::detour_restore(Data::p_glDrawElements, (mem::byte_t*)Data::o_glDrawElements, 5);
	mem::in::detour_restore(Data::pShoot, (mem::byte_t*)Data::oShoot, 8);
	SetWindowLongPtr(Data::hWindow, WNDPROC_INDEX, (LONG_PTR)Data::oWndProc);

	return true;
}
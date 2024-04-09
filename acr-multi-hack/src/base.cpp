#include <pch.h>
#include <base.h>
#include "hacks/hacks.h"

//Data
HMODULE           Base::Data::hModule      = (HMODULE)NULL;
HWND              Base::Data::hWindow      = (HWND)NULL;
window_res		  Base::Data::rect		   = window_res(0, 0);
mem::voidptr_t    Base::Data::pSwapBuffers = (mem::voidptr_t)NULL;
SwapBuffers_t     Base::Data::oSwapBuffers = (SwapBuffers_t)NULL;
mem::voidptr_t	  Base::Data::p_glDrawElements = (mem::voidptr_t)NULL;
glDrawElements_t  Base::Data::o_glDrawElements = (glDrawElements_t)NULL;
mem::voidptr_t	  Base::Data::pIsVisible = (mem::voidptr_t)NULL;
mem::voidptr_t	  Base::Data::p_playerincrosshair = (mem::voidptr_t)NULL;
playerincrosshair_t	 Base::Data::o_playerincrosshair = (playerincrosshair_t)NULL;
mem::voidptr_t	  Base::Data::pShoot = (mem::voidptr_t)NULL;
shoot_t			  Base::Data::oShoot = (shoot_t)NULL;

WndProc_t         Base::Data::oWndProc     = (WndProc_t)NULL;
mem::size_t       Base::Data::szSwapBuffers = 5;
UINT              Base::Data::WmKeys[0xFF];
bool              Base::Data::Detached   = false;
bool              Base::Data::ShowMenu   = true;
bool              Base::Data::InitImGui  = false;


//Functions

bool Base::Init()
{
	Hooks::Init();
	hacks::Init();
	return true;
}

bool Base::Shutdown()
{
	Hooks::Shutdown();
	return true;
}

bool Base::Detach()
{
	Base::Shutdown();
	CreateThread(nullptr, 0, ExitThread, Data::hModule, 0, nullptr);
	return true;
}
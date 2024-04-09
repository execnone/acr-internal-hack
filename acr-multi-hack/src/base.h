#pragma once
#ifndef BASE_H
#define BASE_H

#if defined(MEM_86)
#define WNDPROC_INDEX GWL_WNDPROC
#elif defined(MEM_64)
#define WNDPROC_INDEX GWLP_WNDPROC
#endif

#define D3DDEV9_LEN 119

#include "pch.h"

typedef BOOL(__stdcall* SwapBuffers_t) (_In_ HDC hdc);
typedef LRESULT(CALLBACK*  WndProc_t) (HWND, UINT, WPARAM, LPARAM);
typedef void(__cdecl* playerincrosshair_t)(playerent*& pl, int& hitzone, vec& pos);
typedef void(__stdcall* shoot_t)(playerent* p, vec &targ);
typedef void(__stdcall* glDrawElements_t) (GLenum mode,
	GLuint start,
	GLuint end,
	GLsizei count,
	GLenum type,
	const void* indices);

DWORD WINAPI MainThread(LPVOID lpThreadParameter);
DWORD WINAPI ExitThread(LPVOID lpThreadParameter);

namespace Base
{
	bool Init();
	bool Shutdown();
	bool Detach();

	namespace Data
	{
		extern HMODULE           hModule;
		extern HWND              hWindow;
		extern window_res		 rect;
		extern mem::voidptr_t    pSwapBuffers;
		extern SwapBuffers_t     oSwapBuffers;
		extern mem::voidptr_t	 p_glDrawElements;
		extern glDrawElements_t  o_glDrawElements;
		extern mem::voidptr_t	 pIsVisible;
		extern mem::voidptr_t	 p_playerincrosshair;
		extern playerincrosshair_t o_playerincrosshair;
		extern mem::voidptr_t    pShoot;
		extern shoot_t			 oShoot;
		extern WndProc_t         oWndProc;
		extern mem::size_t       szSwapBuffers;
		extern UINT              WmKeys[0xFF];
		extern bool              Detached;
		extern bool              InitImGui;
		extern bool              ShowMenu;

		namespace Keys
		{
			const UINT ToggleMenu = VK_INSERT;
			const UINT DetachDll  = VK_END;
		}
	}

	namespace Hooks
	{
		bool Init();
		bool Shutdown();

		BOOL __stdcall SwapBuffers(_In_ HDC hdc);
		playerent* __cdecl playerincrosshair();;
		void shoot();
		void __stdcall glDrawRangeElements(GLenum mode,GLuint start,GLuint end,GLsizei count,GLenum type,const void* indices);
		LRESULT CALLBACK  WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
}

#endif
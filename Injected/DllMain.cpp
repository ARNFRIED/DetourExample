#pragma once
#ifndef WIN32
#define WIN32
#endif
#include <WinSock2.h>
#include <string>
#include <cstdio>

#include "WindowsConsole.hpp"
#include "EndsceneHandler.hpp"
#include "CommonDetours.hpp"

HINSTANCE instanceDLL = nullptr;

void MainThread(LPVOID lpParm)
{
	CreateConsole();

	detours["CGxDeviceD3d__ISceneEnd"] = new Detour(0x005AB230, (int)EndSceneDetour);
	detours["NetClientProcess"] = new Detour(0x0055F440, (int)NetClientProcessDetour);

	//-----------------------------------  loop here before exit ----------------------------------
	//wait for the numpad 7 to be pressed...
	while (!GetAsyncKeyState(VK_NUMPAD7)) { Sleep(250); }

	//tell the endscene hook to finish
	shouldRemoveEndSceneInjection = true;

	printf("Exiting...\n");

	//wait the endscene hook to finish...
	while (!endSceneUnhooked) {}

	// unhooks all detours we created
	for (auto& det : detours)
		delete det.second;
	detours.clear();

	CloseConsole();

	// exit	
	FreeLibraryAndExitThread((HMODULE)instanceDLL, 0);
}

// entry point of the DLL
bool APIENTRY DllMain(HINSTANCE instDLL, DWORD reason, LPVOID /* reserved */)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		instanceDLL = instDLL;
		DisableThreadLibraryCalls(instDLL);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&MainThread, 0, 0, 0);
	}
	return true;
}


#pragma once
#include <map>
#include <string>
#include "Detour.hpp"
#include "Timer.hpp"
#include "WowFuncs.hpp"

volatile bool shouldRemoveEndSceneInjection{};
volatile bool endSceneUnhooked{};
volatile bool should_exit{};
std::map<std::string, Detour*> detours{};

//---------------- END SCENE DETOUR ------------------
int __fastcall EndSceneDetour(int s_device, int edx)	//is a __thiscall
{
	if (*(int*)(s_device + 0x3864))
	{
		if (std::string((const char*)0x00C07CD0) == "login" && GetAsyncKeyState(0x4C))	//L
		{
			
			printf("logging...\n");
			const char user[]{ "moka118" };
			const char pass[]{ "opopop" };
			((void(__cdecl*)(const char*, const char*))0x0046E560)(user, pass);	//CGlueMgr::DefaultServerLogin				
		}
		
		if(std::string((const char*)0x00C07CD0) == "charselect" && GetAsyncKeyState(0x45))	//E
		{	
			printf("enter world...\n");
			((void(__cdecl*)())0x0046F460)();		//CGlueMgr::EnterWorld()			
		}
	}

	//-------- return to the original function (and remove injection if needed) --------
	auto det = detours["CGxDeviceD3d__ISceneEnd"];
	det->Restore();
	int res = ((int(__fastcall*)(int, int))det->target)(s_device, edx);
	if (shouldRemoveEndSceneInjection)
	{
		auto it = detours.find("CGxDeviceD3d__ISceneEnd");
		delete it->second;
		detours.erase(it);
		
		endSceneUnhooked = true;
	}
	else
	{
		det->Apply();
	}
	return res;
}
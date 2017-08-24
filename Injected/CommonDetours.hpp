#pragma once

#include "CDataStore.hpp"
#include "Opcodes.h"

//it's a a thiscall
int __fastcall NetClientProcessDetour(int connection, int edx, int tickCount, CDataStore* data)
{
	auto ds = DataStore(data);
	auto opcode = ds.ReadFake<unsigned short>();

	if (opcode == SMSG_ATTACKSTART)
	{
		chat("dude, SMSG_ATTACKSTART");
	}

	//---------------- return to the original function ----------------	
	return detours["NetClientProcess"]->Call<decltype(NetClientProcessDetour)>(connection, edx, tickCount, data);
}
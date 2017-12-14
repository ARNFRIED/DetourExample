#pragma once
#include <algorithm>

using uint64 = unsigned long long;

inline auto ClientConnection() { return *(int*)0x00D43318; }

inline auto ObjManager() { return ClientConnection() ? *(int*)(ClientConnection() + 0x2218) : 0; }

inline uint64 GetLocalPlayerGuid()
{
	if (ObjManager())
		return *(uint64*)(ObjManager() + 0xC0);
	return 0;
}

inline auto GetLocalPlayer() { return ((int(__cdecl*)())0x00402F40)(); }

inline auto GetObjectByGuidTypeCheck(uint64 guid, int typemask = -1)
{
	return  guid ? ((int(__cdecl*)(uint64, int, const char*, int))0x0046B610)(guid, typemask, nullptr, 0) : 0;
}

inline auto PerformanceCount() { return ((int(__cdecl*)())0x00749850)(); }

inline auto FramescriptExecute(const char* text)
{
	((void(__cdecl*)(const char*, const char*, int))0x00706C80)(text, text, 0);
}

inline auto chat(const std::string& message)
{
	if (!GetLocalPlayer())
		return;

	time_t rawtime;
	struct tm timeinfo;
	char buffer[80];

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	strftime(buffer, 80, "[%H:%M:%S] ", &timeinfo);

	auto taggedMessage = buffer + message;
	auto to_print = "DEFAULT_CHAT_FRAME:AddMessage(\"" + taggedMessage + "\")";
	FramescriptExecute(to_print.c_str());
}

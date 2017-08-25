#pragma once
#include <vector>

class Detour
{
public:	
	Detour(int target_func, int hook_func)
		: target{ (byte*)target_func }
		, hook{ hook_func }
	{
		new_bytes.push_back(0x68);				// push (the address provided through hook)
		new_bytes.resize(5);
		*(int*)(new_bytes.data() + 1) = hook;	// dirty hack
		new_bytes.push_back(0xc3);				// return

		original_bytes.resize(6);
		VirtualProtect(target, 6, PAGE_EXECUTE_READWRITE, &old_protection);
		memcpy(original_bytes.data(), target, 6);
		VirtualProtect(target, 6, old_protection, &dummy_protection);

		Apply();
	}

	~Detour() 
	{		
		Restore();		
	}

	void Apply()
	{		
		VirtualProtect(target, 6, PAGE_EXECUTE_READWRITE, &old_protection);
		memcpy(target, new_bytes.data(), 6);
		VirtualProtect(target, 6, old_protection, &dummy_protection);
	}

	void Restore()
	{
		VirtualProtect(target, 6, PAGE_EXECUTE_READWRITE, &old_protection);
		memcpy(target, original_bytes.data(), 6);
		VirtualProtect(target, 6, old_protection, &dummy_protection);
	}

	template<typename T, typename... Args >
	decltype(auto) Call(Args... args)
	{
		Restore();
		VirtualProtect(target, 6, PAGE_EXECUTE_READWRITE, &old_protection);
		auto ret = ((T*)target)(args...);
		VirtualProtect(target, 6, old_protection, &dummy_protection);
		Apply();
		return ret;
	}

	std::vector<byte> original_bytes{};
	std::vector<byte> new_bytes{};
	byte* target{};
private:
	int hook{};
	DWORD old_protection;
	DWORD dummy_protection;
};
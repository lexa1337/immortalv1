#pragma once

#define WIN32_LEAN_AND_MEAN

#include <cstdint>
#include <Windows.h>

#include "offsets.hpp"

template <typename T>
T* GetInterface(const char* name, const char* library)
{
	const auto handle = GetModuleHandle(library);

	if (!handle) return nullptr;

	const auto functionAddress = GetProcAddress(handle, "CreateInterface");

	if (!functionAddress) return nullptr;

	using Fn = T * (*)(const char*, int*);

	const auto CreateInterface = reinterpret_cast<Fn>(functionAddress);

	return CreateInterface(name, nullptr);
}

class ClientEntity {
	
public:
	const int& GetHealth()
	{
		return *reinterpret_cast<int*>(std::uintptr_t(this) + offsets::m_iHealth);
	}

	const int& GetGlowIndex()
	{
		return *reinterpret_cast<int*>(std::uintptr_t(this) + offsets::m_iGlowIndex);
	}

	const int& GetTeamNum()
	{
		return *reinterpret_cast<int*>(std::uintptr_t(this) + offsets::m_iTeamNum);
	}

	const int& GetFlags()
	{
		return *reinterpret_cast<int*>(std::uintptr_t(this) + offsets::m_fFlags);
	}
};

class IClientEntityList {
public:
	virtual void* GetClientNetworkable(int entnum) = 0;
	virtual void* GetClientNetworkableFromHandle(unsigned long hEnt) = 0;
	virtual void* GetClientUnknownFromHandle(unsigned long hEnt) = 0;

	virtual ClientEntity* GetClientEntity(int entnum) = 0;
	virtual ClientEntity* GetClientEntityFromHandle(unsigned long hEnt) = 0;

	virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int					GetHighestEntityIndex(void) = 0;
	virtual void				SetMaxEntities(int maxents) = 0;
	virtual int					GetMaxEntities() = 0;
};
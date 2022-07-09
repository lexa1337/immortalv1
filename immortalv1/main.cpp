#define WIN32_LEAN_AND_MEAN

#include <thread>
#include <cstdint>
#include <Windows.h>

#include "offsets.hpp"
#include "interfaces.hpp"

struct Color
{
	constexpr Color(float r, float g, float b, float a = 1.0f) noexcept :
		r(r), g(g), b(b), a(a) {}

	float r, g, b, a;
};

void MainThread(const HMODULE instance) noexcept
{
	const Color red = Color(1.f, 0.f, 0.f);
	const Color green = Color(0.f, 1.f, 0.f);

	const auto client = reinterpret_cast<std::uintptr_t>(GetModuleHandle("client.dll"));

	const auto entityList = GetInterface<IClientEntityList>("VClientEntityList003", "client.dll");

	while (!GetAsyncKeyState(VK_END))
	{
		const auto pLocalPlayer = reinterpret_cast<ClientEntity*>(client + offsets::dwLocalPlayer);

		if (!pLocalPlayer) continue;

		if (GetAsyncKeyState(VK_SPACE))
		{
			const auto health = pLocalPlayer->GetHealth();

			if (!health) continue;

			const auto flags = pLocalPlayer->GetFlags();

			*reinterpret_cast<std::uintptr_t*>(client + offsets::dwForceJump) = ((flags & (1 << 0)) ? 6 : 4);
		}


		const auto glowObjectManager = *reinterpret_cast<std::uintptr_t*>(client + offsets::dwGlowObjectManager);

		for (int i = 0; i < 64; ++i)
		{
			const auto entity = entityList->GetClientEntity(i);

			if (!entity) continue;

			const auto glowIndex = entity->GetGlowIndex();

			*reinterpret_cast<bool*>(glowObjectManager + (glowIndex * 0x38) + 0x27) = true;
			*reinterpret_cast<bool*>(glowObjectManager + (glowIndex * 0x38) + 0x28) = true;

			if (entity->GetTeamNum() != pLocalPlayer->GetTeamNum())
			{
				*reinterpret_cast<Color*>(glowObjectManager + (glowIndex * 0x38) + 0x8) = red;
			}
			else
			{
				*reinterpret_cast<Color*>(glowObjectManager + (glowIndex * 0x38) + 0x8) = green;
			}

		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	}

	FreeLibraryAndExitThread(instance, 0);
}

int __stdcall DllMain(
	const HMODULE instance,  
	const std::uintptr_t reason,     
	const void* reserved)  
{
	if (DLL_PROCESS_ATTACH == reason)
	{
		DisableThreadLibraryCalls(instance);

		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread),
			instance,
			0,
			nullptr
		);

		if (thread) CloseHandle(thread);
	}
	return TRUE;  
}
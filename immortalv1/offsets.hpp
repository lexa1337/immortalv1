#pragma once

#define WIN32_LEAN_AND_MEAN	

#include <cstddef>
#include <Windows.h>

namespace offsets
{
	/* Client */
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDBF4CC;
	constexpr ::std::ptrdiff_t dwForceJump = 0x52858A8;
	constexpr ::std::ptrdiff_t dwGlowObjectManager = 0x5324588;
	constexpr ::std::ptrdiff_t dwEntityList = 0x4DDB92C;

	/* Player */
	constexpr ::std::ptrdiff_t m_iHealth = 0x100;
	constexpr ::std::ptrdiff_t m_fFlags = 0x104;
	constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
	constexpr ::std::ptrdiff_t m_iGlowIndex = 0x10488;
}
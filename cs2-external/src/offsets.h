#pragma once
#include <iostream>

namespace offsets
{
	inline std::ptrdiff_t dwLocalPlayer = 0x16BC5B8;
	inline std::ptrdiff_t dwEntityList = 0x17C18E0;
	inline std::ptrdiff_t dwViewMatrix = 0x1820100;
	inline std::ptrdiff_t dwC4Planted = 0x1824B98;

	inline std::ptrdiff_t m_pGameSceneNode = 0x310;
	inline std::ptrdiff_t m_SanitizedName = 0x738;
	inline std::ptrdiff_t m_bIsDefusing = 0x13B0;
	inline std::ptrdiff_t m_hPlayerPawn = 0x7EC;
	inline std::ptrdiff_t m_iPawnHealth = 0x32C;
	inline std::ptrdiff_t m_vecOrigin = 0x1224;
	inline std::ptrdiff_t iIDEntIndex = 0x1544;
	inline std::ptrdiff_t m_iTeamNum = 0x3BF;
	inline std::ptrdiff_t modelState = 0x160;
	inline std::ptrdiff_t boneArray = 0x1E0;
	inline std::ptrdiff_t m_iHealth = 0x32C;
	inline std::ptrdiff_t m_fFlags = 0x63;
}

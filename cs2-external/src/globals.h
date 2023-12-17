#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include <cstdint>
#include <chrono>

namespace config
{
	namespace esp
	{
		extern bool bBoxes;
		extern bool bSnaplines;
		extern bool bHealthBar;
		extern bool bHealthInt;
		extern bool bHeadCircles;
		extern bool bShowTeam;
		extern bool bSkeleton;
		extern bool bWeapon;
		extern bool bName;
		extern bool bTeam;
		extern bool bDistance;
		extern bool bC4;

		extern float boxColor[];
		extern float SnaplineColor[];
		extern float TextColor[];
		extern float headCirclesColor[];
		extern float skeletonColor[];
		extern float fovCircleColor[];
	}

	namespace aim
	{
		extern bool bTriggerbot;
		extern bool bAimbot;
		extern bool bFovCircle;
		extern float fovSlider;
		extern int smoothingSlider;
		extern int triggerDelay;
	}

	namespace misc
	{
		extern bool bBhop;
		extern bool watermark;
		extern int fovChangerSlider;
	}
}

namespace globals
{
	extern const int windowWidth;
	extern const int windowHeight;
	extern uintptr_t base_module;
}

namespace gui
{
	extern bool isRunning;
	extern bool isOpen;

	extern bool showVisuals;
	extern bool showMisc;
	extern bool showAim;
	extern bool showConfig;
	extern bool showAbout;

	extern int bombOffsetX;
	extern int bombOffsetY;

}

namespace vars
{
	extern std::chrono::seconds remainingTime;
	extern bool isBeingDefused;
	extern bool killTimer;
}

#endif

/*#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

namespace config
{
	namespace esp
	{
		inline bool bBoxes = true;
		inline bool bSnaplines = false;
		inline bool bHealthBar = false;
		inline bool bHealthInt = false;
		inline bool bHeadCircles = false;
		inline bool bShowTeam = false;
		inline bool bSkeleton = false;
		inline bool bWeapon = false;
		inline bool bName = false;
		inline bool bTeam = false;
		inline bool bC4 = false;

		inline float boxColor[] = { 1.f, 1.f, 1.f, 1.f };
		inline float SnaplineColor[] = { 1.f, 1.f, 1.f, 1.f };
		inline float TextColor[] = { 1.f, 1.f, 1.f, 1.f };
		inline float headCirclesColor[] = { 1.f, 1.f, 1.f, 1.f };
		inline float skeletonColor[] = { 1.f, 1.f, 1.f, 1.f };
		inline float fovCircleColor[] = { 0.5f, 1.f, 1.f, 1.f };
	}

	namespace aim
	{
		inline bool bTriggerbot = false;
		inline bool bAimbot = false;
		inline bool bFovCircle = false;
		inline float fovSlider = 10.f;
		inline int smoothingSlider = 1;
		inline int triggerDelay = 1;
	}

	namespace misc
	{
		inline bool bBhop = false;
		inline bool watermark = true;
		inline int fovChangerSlider = 120;
	}
}

namespace globals
{
	const int windowWidth = 1920;
	const int windowHeight = 1080;
	uintptr_t base_module;
}

namespace gui
{
	bool isRunning = true;
	bool isOpen = true;

	bool showVisuals = false;
	bool showMisc = false;
	bool showAim = false;
	bool showConfig = false;
	bool showAbout = false;
}

#endif*/
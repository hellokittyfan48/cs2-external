#include "globals.h"

namespace config
{
    namespace esp
    {
        bool bBoxes = false;
        bool bSnaplines = false;
        bool bHealthBar = false;
        bool bHealthInt = false;
        bool bHeadCircles = false;
        bool bShowTeam = false;
        bool bSkeleton = false;
        bool bWeapon = false;
        bool bName = false;
        bool bTeam = false;
        bool bDistance = false;
        bool bC4 = false;

        float boxColor[] = { 1.f, 1.f, 1.f, 1.f };
        float SnaplineColor[] = { 1.f, 1.f, 1.f, 1.f };
        float TextColor[] = { 1.f, 1.f, 1.f, 1.f };
        float headCirclesColor[] = { 1.f, 1.f, 1.f, 1.f };
        float skeletonColor[] = { 1.f, 1.f, 1.f, 1.f };
        float fovCircleColor[] = { 0.5f, 1.f, 1.f, 1.f };
    }

    namespace aim
    {
        bool bTriggerbot = false;
        bool bAimbot = false;
        bool bFovCircle = false;
        float fovSlider = 10.f;
        int smoothingSlider = 1;
        int triggerDelay = 1;
    }

    namespace misc
    {
        bool bBhop = false;
        bool watermark = true;
        int fovChangerSlider = 120;
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

    int bombOffsetX = 10;
    int bombOffsetY = 560;

}

namespace vars
{
    std::chrono::seconds remainingTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::seconds(0));
    bool isBeingDefused = false;
    bool killTimer = false;
}
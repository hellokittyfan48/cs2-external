#include "hacks.h"
#include "../offsets.h"
#include "../globals.h"

void hacks::c4timer() noexcept
{
    auto start_time = std::chrono::steady_clock::now();
    auto end_time = start_time + std::chrono::seconds(40);

    while (std::chrono::steady_clock::now() < end_time && !vars::killTimer) {
        auto remainingTime = std::chrono::duration_cast<std::chrono::seconds>(end_time - std::chrono::steady_clock::now());
        vars::remainingTime = remainingTime;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    vars::killTimer = false;
}
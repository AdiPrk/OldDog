#include <PCH/dogpch.h>
#include "frameRate.h"

using namespace std::chrono;

namespace Dog {

    FrameRateController::FrameRateController(unsigned int targetFrameRate)
        : targetFPS(targetFrameRate) {
        targetFrameDuration = duration<double>(1.0 / targetFrameRate);
    }

    float FrameRateController::waitForNextFrame() {
        auto now = high_resolution_clock::now();
        auto timeSinceLastFrame = now - m_lastFrameTime;

        // Sleep until approximately just before the targetFrameDuration
        if (timeSinceLastFrame < targetFrameDuration - 1ms) { // Adjust the 1ms according to your needs
            std::this_thread::sleep_for(targetFrameDuration - timeSinceLastFrame - 1ms);
        }

        // Fine-tune with busy-waiting for the last small part
        while (timeSinceLastFrame < targetFrameDuration) {
            now = high_resolution_clock::now();
            timeSinceLastFrame = now - m_lastFrameTime;
        }

        m_lastFrameTime = now;
        return duration<float>(timeSinceLastFrame).count();
    }

    void FrameRateController::setTargetFPS(unsigned int targetFPS) {
        targetFPS = targetFPS;
        targetFrameDuration = duration<double>(1.0 / targetFPS);
    }

}

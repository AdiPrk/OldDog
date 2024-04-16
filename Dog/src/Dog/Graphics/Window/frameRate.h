#pragma once

// Simple frame rate controller.

namespace Dog {

    class FrameRateController {
    public:
        explicit FrameRateController(unsigned int targetFrameRate)
            : targetFPS(targetFrameRate) {
            targetFrameDuration = std::chrono::duration<double>(1.0 / targetFrameRate);
        }

        float waitForNextFrame() {
            auto now = std::chrono::high_resolution_clock::now();
            auto timeSinceLastFrame = now - m_lastFrameTime;
            while (timeSinceLastFrame < targetFrameDuration) {
                // Busy-wait loop to delay until the next frame
                now = std::chrono::high_resolution_clock::now();
                timeSinceLastFrame = now - m_lastFrameTime;
            }
            m_lastFrameTime = now;
            return std::chrono::duration<float>(timeSinceLastFrame).count();
        }

        void setTargetFPS(unsigned int targetFPS) {
            targetFPS = targetFPS;
            targetFrameDuration = std::chrono::duration<double>(1.0 / targetFPS);
        }

    private:
        unsigned int targetFPS;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_lastFrameTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> targetFrameDuration;
    };

}
#pragma once

#include "../IRenderer.h"

namespace Dog {

    class DeferredRenderer : public IRenderer {
    public:
        void initialize() override;
        void shutdown() override;

        void clearFrame() override;
        void beginFrame() override;
        void endFrame() override;

        void geometryPass();
        void lightingPass();

    private:
        // The G-Buffer components
        unsigned int positionBuffer;
        unsigned int normalBuffer;
        unsigned int albedoSpecBuffer;

        // Framebuffer object for geometry pass
        unsigned int gBuffer;

        // Shader programs
        unsigned int geometryShader;
        unsigned int lightingShader;

        // Initialization and setup methods for deferred rendering
        void setupGBuffer();
        void setupShaders();
    };


}

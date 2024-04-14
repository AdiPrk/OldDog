#pragma once

namespace Dog {

	class IRenderer {
	public:
        virtual ~IRenderer() {}

        virtual void initialize() = 0;
        virtual void shutdown() = 0;

        virtual void clearFrame() = 0;
        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;
	};

} // namespace Dog

#pragma once

#include "Dog/core.h"

namespace Dog {

	class DOG_API IRenderer {
	public:
        virtual ~IRenderer() {}

        virtual void initialize() = 0;
        virtual void shutdown() = 0;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;
	};

} // namespace Dog

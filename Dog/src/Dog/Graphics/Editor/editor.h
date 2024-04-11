#pragma once

#include "Dog/core.h"

namespace Dog {

	class DOG_API Editor {
	public:
		Editor();
		~Editor();

		static Editor& Get() {
			static Editor instance;
			return instance;
		}

		Editor(const Editor&) = delete;
		Editor& operator=(const Editor&) = delete;

		void Init(GLFWwindow* window);
		void Exit();

		void beginFrame();
		void endFrame();
	};

} // namespace Dog

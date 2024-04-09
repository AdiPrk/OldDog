#pragma once

#include "Dog/core.h"

struct GLFWwindow;

namespace Dog {

	// V-Sync is on by default.
	class DOG_API IWindow {
	public:
		virtual bool IsRunning() = 0;
		virtual void SetFullscreen(bool fullScreen = true) = 0;
		GLFWwindow* GetWindowHandle() { return window; }
		virtual void SetTitle(const char* title) = 0;
		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;
		virtual float GetAspectRatio() = 0;
	protected:
		GLFWwindow* window = nullptr;
	};

}
#pragma once

struct GLFWwindow;

namespace Dog {

	// V-Sync is on by default.
	class IWindow {
	public:
		IWindow() = default;
		virtual bool IsRunning() = 0;
		virtual void SetFullscreen(bool fullScreen = true) = 0;
		GLFWwindow* GetWindowHandle() { return window; }
		virtual const std::string GetTitle() = 0;
		virtual void SetTitle(const std::string& name) = 0;
		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;
		virtual float GetAspectRatio() = 0;
		virtual void UpdateTitle() = 0;
		virtual void SetVSync(bool enabled = true) = 0;
	protected:
		GLFWwindow* window = nullptr;
	};

}
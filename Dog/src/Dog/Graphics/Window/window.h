#pragma once


#include "Iwindow.h"

namespace Dog {

	class Window : public IWindow {
	public:
		Window(unsigned int screenWidth, unsigned int screenHeight, const std::string& name);
		~Window();

		bool IsRunning();

		void UpdateTitle();

		void ToggleFullscreen();

		void SetVSync(bool enabled = true);

		void SwapBuffers();
		void SetFullscreen(bool fullScreen = true);

		GLFWwindow* GetWindowHandle() { return window; };

		void SetKeyCallback(GLFWkeyfun callback);
		void SetFramebufferSizeCallback(GLFWframebuffersizefun callback);

		const std::string GetTitle();
		void SetTitle(const std::string& title);

		constexpr static double targetFrameLength = 1.0 / 120.0;

		unsigned int GetWidth() { return m_Width; }
		unsigned int GetHeight() { return m_Height; }
		float GetAspectRatio() { return m_AspectRatio; }

	private:
		//std::chrono::high_resolution_clock::time_point m_LastFrameTime;
		std::chrono::high_resolution_clock::time_point m_LastFrameTime;
		unsigned int m_Width = 0;
		unsigned int m_Height = 0;
		float m_AspectRatio = 0;
		bool m_IsFullscreen = false;
	};

}
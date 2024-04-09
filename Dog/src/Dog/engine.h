#pragma once

#include "Dog/core.h"
#include "Graphics/Window/Iwindow.h"
#include <memory>

namespace Dog {

	class Window;
	class Resources;
	class Renderer2D;
	class Camera2D;
	class DeferredRenderer;

	class DOG_API Engine {
	public:
		Engine(unsigned int width, unsigned int height, const char* name);
		~Engine();

		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		void Init();
		void Shutdown();

		int Run();

		IWindow* GetWindow() { return window.get(); }

	private:
		bool running;
		std::unique_ptr<IWindow> window;
		std::unique_ptr<Resources> resources;
		std::unique_ptr<Renderer2D> renderer2D;
		std::unique_ptr<DeferredRenderer> deferredRenderer;
		std::unique_ptr<Camera2D> camera;
	};

}

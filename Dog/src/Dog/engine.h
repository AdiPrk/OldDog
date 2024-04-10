#pragma once

#include "Dog/core.h"

namespace Dog {

	class IWindow;
	class Resources;
	class Renderer2D;
	class Camera2D;
	class DeferredRenderer;
	class Scene;

	class DOG_API Engine {
	public:
		// Creates the Engine! Note, the width and height set here are used as the aspect ratio for everything rendered.
		// width: The width of the window.
		// height: The height of the window.
		// name: The name of the window.
		Engine(unsigned int width, unsigned int height, const char* name);
		~Engine();

		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		void Init();
		void Shutdown();

		int Run(Scene* startScene);

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

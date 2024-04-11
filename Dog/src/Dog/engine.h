#pragma once

#include "Dog/core.h"

namespace Dog {

	class IWindow;
	class Resources;
	class Renderer2D;
	class Camera2D;
	class DeferredRenderer;
	class Scene;
	class Editor;

	class DOG_API Engine {
	public:
		// Creates the Engine! Note, the width and height set here are used as the aspect ratio for everything rendered.
		// width: The width of the window.
		// height: The height of the window.
		// name: The name of the window.
		Engine(unsigned int width, unsigned int height, const char* name);
		~Engine();

		static Engine& Get(unsigned int width = 1280, unsigned int height = 720, const char* name = "Dog Engine")
		{
			static Engine instance(width, height, name);
			return instance;
		}

		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		void Init();
		void Shutdown();

		int Run(Scene* startScene);

		std::shared_ptr<IWindow>& GetWindow() { return window; }
		std::shared_ptr<Resources>& GetResources() { return resources; }
		std::shared_ptr<Renderer2D>& GetRenderer2D() { return renderer2D; }
		std::shared_ptr<DeferredRenderer>& GetDeferredRenderer() { return deferredRenderer; }
		std::shared_ptr<Camera2D>& GetCamera() { return camera; }
		std::shared_ptr<Editor>& GetEditor() { return editor; }

	private:
		bool running;
		std::shared_ptr<IWindow> window;
		std::shared_ptr<Resources> resources;
		std::shared_ptr<Renderer2D> renderer2D;
		std::shared_ptr<DeferredRenderer> deferredRenderer;
		std::shared_ptr<Camera2D> camera;
		std::shared_ptr<Editor> editor;
	};

}

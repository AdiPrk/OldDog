#pragma once

namespace Dog {

	class IWindow;
	class Resources;
	class Renderer2D;
	class DeferredRenderer;
	class Scene;
	class Editor;

	class Engine {
	public:
		// Creates the Engine! Note, the width and height set here are used as the aspect ratio for everything rendered.
		// width: The width of the window.
		// height: The height of the window.
		// name: The name of the window.
		Engine(unsigned int width, unsigned int height, const std::string& name);
		~Engine();

		static Engine& Get(unsigned int width = 1280, unsigned int height = 720, const std::string& name = "Dog Engine")
		{
			static Engine instance(width, height, name);
			return instance;
		}

		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;


		int Run(Scene* startScene);

		static std::shared_ptr<IWindow>& GetWindow() { return Get().window; }
		static std::shared_ptr<Resources>& GetResources() {	return Get().resources;	}
		static std::shared_ptr<Renderer2D>& GetRenderer2D() { return Get().renderer2D; }
		static std::shared_ptr<DeferredRenderer>& GetDeferredRenderer() { return Get().deferredRenderer; }
		static std::shared_ptr<Editor>& GetEditor() { return Get().editor; }

	private:
		void Init(Scene* startScene);
		void Shutdown();

		bool running;
		std::shared_ptr<IWindow> window;
		std::shared_ptr<Resources> resources;
		std::shared_ptr<Renderer2D> renderer2D;
		std::shared_ptr<DeferredRenderer> deferredRenderer;
		std::shared_ptr<Editor> editor;
	};

}

#pragma once

namespace Dog {

	class IWindow;
	class Renderer2D;
	class DeferredRenderer;
	class Scene;
	class Editor;

	struct EngineSpec {
		std::string name = "Dog Engine";
		unsigned width = 1280;
		unsigned height = 720;
		unsigned fps = 60;
	};

	class Engine {
	public:
		// Creates the Engine! Note, the width and height set here are used as the aspect ratio for everything rendered.
		// width: The width of the window.
		// height: The height of the window.
		// name: The name of the window.
		Engine(const EngineSpec& specs);
		~Engine();

		static Engine& Get(const EngineSpec& specs = {})
		{
			static Engine instance(specs);
			return instance;
		}

		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		int Run(Scene* startScene);

		static std::shared_ptr<IWindow>& GetWindow() { return Get().window; }
		static std::shared_ptr<Renderer2D>& GetRenderer2D() { return Get().renderer2D; }
		static std::shared_ptr<DeferredRenderer>& GetDeferredRenderer() { return Get().deferredRenderer; }
		static std::shared_ptr<Editor>& GetEditor() { return Get().editor; }

		std::string GetName() const { return name; }
		unsigned GetTargetFPS() const { return targetFPS; }
		void SetTargetFPS(unsigned fps) { targetFPS = fps; }

	private:
		void Init(Scene* startScene);
		void Shutdown();

		bool running;
		std::string name;
		unsigned targetFPS;

		std::shared_ptr<IWindow> window;
		std::shared_ptr<Renderer2D> renderer2D;
		std::shared_ptr<DeferredRenderer> deferredRenderer;
		std::shared_ptr<Editor> editor;

		Events::Handle<Event::ImageFileCreated> imageFileCreatedHandle;
		Events::Handle<Event::ImageFileDeleted> imageFileDeletedHandle;
		Events::Handle<Event::ImageFileModified> imageFileModifiedHandle;
		Events::Handle<Event::ShaderFileCreated> shaderFileCreatedHandle;
		Events::Handle<Event::ShaderFileDeleted> shaderFileDeletedHandle;
		Events::Handle<Event::ShaderFileModified> shaderFileModifiedHandle;
	};

}

#pragma once

namespace Dog {

	class Window;
	class Renderer2D;
	class DeferredRenderer;
	class Scene;
	class Editor;

	struct EngineSpec {
		std::string name = "Dog Engine"; // The name of the window.
		unsigned width = 1280;           // The width of the window.
		unsigned height = 720;           // The height of the window.
		unsigned fps = 60;			     // The target frames per second.
	};

	class Engine {
	public:
		Engine(const EngineSpec& specs);
		~Engine();

		// Called by client to create the engine.
		static Engine& Create(const EngineSpec& specs = {})
		{
			static Engine instance(specs);
			return instance;
		}

		static Engine& Get()
		{
			return Create();
		}

		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		int Run(const std::string& startScene);

		static glm::vec2 GetSceneSize() { return sceneSize; }
		static std::shared_ptr<Window>& GetWindow() { return Get().window; }
		static std::shared_ptr<Renderer2D>& GetRenderer2D() { return Get().renderer2D; }
		static std::shared_ptr<DeferredRenderer>& GetDeferredRenderer() { return Get().deferredRenderer; }

#ifndef DOG_SHIP
		static std::shared_ptr<Editor>& GetEditor() { return Get().editor; }
#endif

		std::string GetName() const { return name; }
		unsigned GetTargetFPS() const { return targetFPS; }
		void SetTargetFPS(unsigned fps) { targetFPS = fps; }

	private:
		void Init(const std::string& startScene);
		void Shutdown();

		bool running;
		std::string name;
		unsigned targetFPS;

		static glm::vec2 sceneSize;

		std::shared_ptr<Window> window;
		std::shared_ptr<Renderer2D> renderer2D;
		std::shared_ptr<DeferredRenderer> deferredRenderer;

#ifndef DOG_SHIP
		std::shared_ptr<Editor> editor;
#endif

		Events::Handle<Event::ImageFileCreated> imageFileCreatedHandle;
		Events::Handle<Event::ImageFileDeleted> imageFileDeletedHandle;
		Events::Handle<Event::ImageFileModified> imageFileModifiedHandle;
		Events::Handle<Event::ShaderFileCreated> shaderFileCreatedHandle;
		Events::Handle<Event::ShaderFileDeleted> shaderFileDeletedHandle;
		Events::Handle<Event::ShaderFileModified> shaderFileModifiedHandle;
	};

}

#include <PCH/dogpch.h>
#include "engine.h"
#include "Graphics/Window/window.h"
#include "Dog/Logger/logger.h"
#include "Dog/Graphics/Renderer/Renderer2D/renderer2d.h"
#include "Dog/Graphics/Texture/texture2d.h"
#include "Dog/Graphics/Renderer/Renderer3D/DeferredRenderer.h"
#include "Dog/Input/input.h"

#include "Dog/Scene/sceneManager.h"
#include "Dog/Scene/scene.h"

#include "Dog/Graphics/Editor/editor.h"

#include "Dog/Assets/FileWatcher/fileWatcher.h"
#include "Dog/Assets/Packer/assetPacker.h"

#include "Dog/Graphics/Window/frameRate.h"
#include "Dog/Graphics/Renderer/Shaders/shader.h"



namespace Dog {

	glm::vec2 Engine::sceneSize = glm::vec2(1280, 720);

	Engine::Engine(const EngineSpec& specs)
		: running(true)
		, name(specs.name)
		, targetFPS(specs.fps)
	{
		Logger::Init();

		sceneSize = glm::vec2(specs.width, specs.height);

		window = std::make_shared<Window>(specs.width, specs.height, name);
		renderer2D = std::make_shared<Renderer2D>();
		deferredRenderer = std::make_shared<DeferredRenderer>();
		editor = std::make_shared<Editor>();

		Shader::SetupUBO();

		Input::Init(window->GetWindowHandle());

		renderer2D->initialize();
		deferredRenderer->initialize();

		// Setup Editor
		editor->Init(window->GetWindowHandle());

		Assets::Init();

		window->SetVSync(false);

		imageFileCreatedHandle = SUBSCRIBE_EVENT(Event::ImageFileCreated, Assets::OnImageFileCreate);
		imageFileDeletedHandle = SUBSCRIBE_EVENT(Event::ImageFileDeleted, Assets::OnImageFileDelete);
		imageFileModifiedHandle = SUBSCRIBE_EVENT(Event::ImageFileModified, Assets::OnImageFileModify);
		shaderFileCreatedHandle = SUBSCRIBE_EVENT(Event::ShaderFileCreated, Assets::OnShaderFileCreate);
		shaderFileDeletedHandle = SUBSCRIBE_EVENT(Event::ShaderFileDeleted, Assets::OnShaderFileDelete);
		shaderFileModifiedHandle = SUBSCRIBE_EVENT(Event::ShaderFileModified, Assets::OnShaderFileModify);
	}

	Engine::~Engine()
	{
	}

	void Engine::Init(Scene* startScene)
	{
		SceneManager::Init(startScene);
	}

	void Engine::Shutdown()
	{
		// Unsubscribe manually just in case.
		UNSUBSCRIBE_EVENT(Event::ImageFileCreated, imageFileCreatedHandle);  
		UNSUBSCRIBE_EVENT(Event::ImageFileDeleted, imageFileDeletedHandle);
		UNSUBSCRIBE_EVENT(Event::ImageFileModified, imageFileModifiedHandle);
		UNSUBSCRIBE_EVENT(Event::ShaderFileCreated, shaderFileCreatedHandle);
		UNSUBSCRIBE_EVENT(Event::ShaderFileDeleted, shaderFileDeletedHandle);
		UNSUBSCRIBE_EVENT(Event::ShaderFileModified, shaderFileModifiedHandle);

		SceneManager::Exit();
		editor->Exit();
	}

	int Engine::Run(Scene* startScene)
	{
		Init(startScene);

		// Watch directories for assets.
#ifndef DOG_SHIP
		WATCH_DIRECTORY(Image);
		WATCH_DIRECTORY(Shader);
#endif

		/* Loop until the user closes the window */
		const float fixedTimeStep = 1.0f / 60.0f;
		float accumulator = 0.0f;
			
		FrameRateController frameRateController(targetFPS);

		while (running && !glfwWindowShouldClose(window->GetWindowHandle())) 
		{
			// Control FPS
			float deltaTime = frameRateController.waitForNextFrame();

			// Update input
			Input::Update();
			if (Input::isKeyDown(Key::ESCAPE)) {
				running = false;
				break;
			}

			// Update assets
			Assets::UpdateAssets(deltaTime);

			// Swap scenes if necessary (Init/Exit)
			SceneManager::SwapScenes();

			// Update Editor.
#ifndef DOG_SHIP
			editor->UpdateVisibility(window->GetWidth(), window->GetHeight());
			editor->beginFrame();
#endif

			// Just a little window fps counter.
			window->ShowFPSInTitle(Input::isKeyDown(Key::NUM1));
			window->UpdateTitle();

			// Update scenes.
			SceneManager::Update(deltaTime);

			// Render scenes.
#ifndef DOG_SHIP
			SceneManager::Render(deltaTime, editor->IsActive());
#else
			SceneManager::Render(deltaTime, false);
#endif
			// Render Editor (if active)
#ifndef DOG_SHIP
			editor->endFrame();
#endif
			// Swap buffers
			glfwSwapBuffers(window->GetWindowHandle());
		}

		// Clean up assets.
#ifndef DOG_SHIP
		STOP_WATCHING(Image);
		STOP_WATCHING(Shader);
#endif
		Shutdown();

		return 0;
	}

} // namespace Dog
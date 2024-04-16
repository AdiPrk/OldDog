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

#include "Dog/Resources/FileWatcher/fileWatcher.h"

#include "Dog/Graphics/Window/frameRate.h"

namespace Dog {

	Engine::Engine(const EngineSpec& specs)
		: running(true)
		, name(specs.name)
		, targetFPS(specs.fps)
	{
		Logger::Init();
		Resources::Init();

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

		window->SetVSync(false);

		imageFileCreatedHandle = SUBSCRIBE_EVENT(Event::ImageFileCreated, Resources::OnImageFileCreate);
		imageFileDeletedHandle = SUBSCRIBE_EVENT(Event::ImageFileDeleted, Resources::OnImageFileDelete);
		imageFileModifiedHandle = SUBSCRIBE_EVENT(Event::ImageFileModified, Resources::OnImageFileModify);
		shaderFileCreatedHandle = SUBSCRIBE_EVENT(Event::ShaderFileCreated, Resources::OnShaderFileCreate);
		shaderFileDeletedHandle = SUBSCRIBE_EVENT(Event::ShaderFileDeleted, Resources::OnShaderFileDelete);
		shaderFileModifiedHandle = SUBSCRIBE_EVENT(Event::ShaderFileModified, Resources::OnShaderFileModify);
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

		// A test for loading a duplicate resource. Should emit a warning in Debug mode.
		FileWatcher<Event::ImageFileCreated, Event::ImageFileModified, Event::ImageFileDeleted> imageWatcher(Resources::ImagesPath);
		FileWatcher<Event::ShaderFileCreated, Event::ShaderFileModified, Event::ShaderFileDeleted> shaderWatcher(Resources::ShadersPath);
		imageWatcher.start();
		shaderWatcher.start();

		/* Loop until the user closes the window */
		const float fixedTimeStep = 1.0f / 60.0f;
		float deltaTime = 0.0f;
		float lastTime = 0.0f;
		float accumulator = 0.0f;
			
		FrameRateController frameRateController(targetFPS);

		while (running && !glfwWindowShouldClose(window->GetWindowHandle())) 
		{
			float deltaTime = frameRateController.waitForNextFrame();

			Input::Update();
			if (Input::isKeyDown(Key::ESCAPE)) {
				running = false;
				break;
			}

			// Update resources
			Resources::UpdateResources();

			Shader::iTime += deltaTime;
			Shader::SetTimeUBO(Shader::iTime);

			// Initialize/Swap scenes.
			SceneManager::SwapScenes();

			// Update Editor.
			editor->beginFrame();

			// Just a little window fps counter.
			window->ShowFPSInTitle(Input::isKeyDown(Key::NUM1));
			window->UpdateTitle();

			// Update scenes.
			SceneManager::Update(deltaTime);

			// Render scenes.
			SceneManager::Render(deltaTime);

			// Render Editor.
			editor->endFrame();
			
			// Swap buffers
			glfwSwapBuffers(window->GetWindowHandle());
		}

		imageWatcher.stop();
		shaderWatcher.stop();
		Shutdown();

		return 0;
	}

} // namespace Dog
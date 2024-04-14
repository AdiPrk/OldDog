#include <PCH/dogpch.h>
#include "engine.h"
#include "Graphics/Window/window.h"
#include "Dog/Logger/logger.h"
#include "Dog/Graphics/Renderer/Renderer2D/renderer2d.h"
#include "Dog/Graphics/Renderer/Renderer2D/texture2d.h"
#include "Dog/Graphics/Renderer/Renderer3D/DeferredRenderer.h"
#include "Dog/Input/input.h"

#include "Dog/Scene/sceneManager.h"
#include "Dog/Scene/scene.h"

#include "Dog/Graphics/Editor/editor.h"

#include "Dog/Resources/FileWatcher/fileWatcher.h"

namespace Dog {

	Engine::Engine(unsigned int width, unsigned int height, const std::string& name)
		: running(true)
	{
		Logger::Init();
		Resources::Init();

		window = std::make_shared<Window>(width, height, name);
		renderer2D = std::make_shared<Renderer2D>();
		deferredRenderer = std::make_shared<DeferredRenderer>();
		//camera->SetPosition(glm::vec3(width * -0.5f, height * -0.5f, 0.0f));
		editor = std::make_shared<Editor>();

		Shader::SetupUBO();

		Input::Init(window->GetWindowHandle());

		renderer2D->initialize();
		deferredRenderer->initialize();

		std::shared_ptr<Shader> sp = Resources::Load<Shader>("DogAssets/Shaders/defaultsprite");
		Shader::SetShader(sp);

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
		Resources::Load("Texture2D", "DogAssets/Images/square.png");
		Resources::Load<Texture2D>("DogAssets/Images/square.png");
		Resources::Load<Texture2D>("DogAssets/Images/playButton.png");
		Resources::Load<Texture2D>("DogAssets/Images/stopButton.png");
		Resources::Load("Texture2D", "DogAssets/Images/circle.png");
		Resources::Load("Texture2D", "DogAssets/Images/error.png");
		Resources::Load("Shader", "DogAssets/Shaders/error");

		FileWatcher<Event::ImageFileCreated, Event::ImageFileModified, Event::ImageFileDeleted> imageWatcher("DogAssets/Images");
		FileWatcher<Event::ShaderFileCreated, Event::ShaderFileModified, Event::ShaderFileDeleted> shaderWatcher("DogAssets/Shaders");
		imageWatcher.start();
		shaderWatcher.start();

		/* Loop until the user closes the window */
		const float fixedTimeStep = 1.0f / 60.0f;
		float deltaTime = 0.0f;
		float lastTime = 0.0f;
		float accumulator = 0.0f;
				
		while (running && !glfwWindowShouldClose(window->GetWindowHandle())) 
		{
			Input::Update();
			if (Input::isKeyDown(Key::ESCAPE)) {
				running = false;
				break;
			}

			// Update resources
			Resources::UpdateResources();

			// Initialize/Swap scenes.
			SceneManager::SwapScenes();

			// Update Editor.
			editor->beginFrame();

			// Calculate delta time.
			float currentTime = (float)glfwGetTime();
			deltaTime = std::min(currentTime - lastTime, 0.1f); // 10fps min
			accumulator += deltaTime;
			lastTime = currentTime;

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
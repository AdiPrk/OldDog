#include <PCH/dogpch.h>
#include "engine.h"
#include "Graphics/Window/window.h"
#include "Resources/resources.h"
#include "Dog/Logger/logger.h"
#include "Dog/Graphics/Renderer/Renderer2D/renderer2d.h"
#include "Dog/Graphics/Renderer/Renderer2D/texture2d.h"
#include "Dog/Graphics/Renderer/Renderer3D/DeferredRenderer.h"
#include "Dog/Input/input.h"

#include "Dog/Scene/sceneManager.h"
#include "Dog/Scene/scene.h"

#include "Dog/Graphics/Editor/editor.h"

namespace Dog {

	Engine::Engine(unsigned int width, unsigned int height, const std::string& name)
		: running(true)
	{
		Logger::Init();

		window = std::make_shared<Window>(width, height, name);
		resources = std::make_shared<Resources>();
		renderer2D = std::make_shared<Renderer2D>();
		deferredRenderer = std::make_shared<DeferredRenderer>();
		//camera->SetPosition(glm::vec3(width * -0.5f, height * -0.5f, 0.0f));
		editor = std::make_shared<Editor>();

		Shader::SetupUBO();

		Input::Init(window->GetWindowHandle());

		renderer2D->initialize();
		deferredRenderer->initialize();

		std::shared_ptr<Shader> sp = resources->Load<Shader>("DogAssets/Shaders/defaultsprite");
		Shader::SetShader(sp);

		// Setup Editor
		editor->Init(window->GetWindowHandle());

		window->SetVSync(false);
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
		SceneManager::Exit();
		editor->Exit();
	}

	int Engine::Run(Scene* startScene)
	{
		Init(startScene);

		// A test for loading a duplicate resource. Should emit a warning in Debug mode.
		resources->Load("Texture2D", "DogAssets/Images/square.png");
		resources->Load<Texture2D>("DogAssets/Images/square.png");
		resources->Load("Texture2D", "DogAssets/Images/circle.png");

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

		Shutdown();

		return 0;
	}

} // namespace Dog
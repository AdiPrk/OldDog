#include <PCH/dogpch.h>
#include "engine.h"
#include "Graphics/Window/window.h"
#include "Resources/resources.h"
#include "Dog/Logger/logger.h"
#include "Dog/Graphics/Renderer/Renderer2D/renderer2d.h"
#include "Dog/Graphics/Renderer/Renderer2D/texture2d.h"
#include "Dog/Graphics/Renderer/Renderer3D/DeferredRenderer.h"
#include "Dog/Graphics/Camera/camera2d.h"
#include "Dog/Input/input.h"

#include "Dog/Scene/sceneManager.h"
#include "Dog/Scene/scene.h"

namespace Dog {

	Engine::Engine(unsigned int width, unsigned int height, const char* name)
		: running(true)
	{
		Logger::Init();

		window = std::make_unique<Window>(width, height, name);
		resources = std::make_unique<Resources>();
		renderer2D = std::make_unique<Renderer2D>();
		deferredRenderer = std::make_unique<DeferredRenderer>();
		camera = std::make_unique<Camera2D>(width, height);

		Shader::SetupUBO();

		Input::Init(window->GetWindowHandle());

		renderer2D->initialize();
		deferredRenderer->initialize();

		std::shared_ptr<Shader> sp = resources->Load<Shader>("DogAssets/Shaders/defaultsprite");
		renderer2D->SetShader(*sp);

		camera->UpdateUniforms();
	}

	Engine::~Engine()
	{
	}

	void Engine::Init()
	{
	}

	void Engine::Shutdown()
	{
	}

	int Engine::Run(Scene* startScene)
	{
		SceneManager::Init(startScene);

		auto texture = resources->Load<Texture2D>("DogAssets/Images/square.png");

		/* Loop until the user closes the window */
		const float fixedTimeStep = 1.0f / 60.0f;
		float deltaTime = 0.0f;
		float lastTime = 0.0f;
		float accumulator = 0.0f;
				
		while (running && !glfwWindowShouldClose(window->GetWindowHandle())) {

			Input::Update();
			if (Input::isKeyDown(Key::ESCAPE)) {
				running = false;
				break;
			}

			float currentTime = (float)glfwGetTime();
			deltaTime = std::min(currentTime - lastTime, 0.1f); // 10fps min
			accumulator += deltaTime;
			lastTime = currentTime;

			std::string curTitle = window->GetTitle();
			// cut so it only keeps until it finds "- FPS: "
			curTitle = curTitle.substr(0, curTitle.find(" - FPS: "));

			window->SetTitle((curTitle + " - FPS: " + std::to_string(1.0f / deltaTime).c_str()).c_str());

			SceneManager::Update(deltaTime);

			SceneManager::Render(deltaTime);

			renderer2D->beginFrame();

			renderer2D->DrawSprite(*texture, glm::vec2(0.0f, 0.0f), glm::vec2(100.0f, 100.0f));
			
			renderer2D->endFrame();

			glfwSwapBuffers(window->GetWindowHandle());
		}

		SceneManager::Exit();

		return 0;
	}

} // namespace Dog
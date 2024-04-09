#include <PCH/dogpch.h>
#include "engine.h"
#include "Graphics/Window/window.h"
#include "Resources/resources.h"
#include "Dog/Logger/logger.h"
#include "Dog/Graphics/Renderer/Renderer2D/renderer2d.h"
#include "Dog/Graphics/Renderer/Renderer2D/texture2d.h"
#include "Dog/Graphics/Renderer/Renderer3D/DeferredRenderer.h"
#include "Dog/Graphics/Camera/camera2d.h"

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

		renderer2D->initialize();
		deferredRenderer->initialize();

		auto sp = resources->Load("Shader", "DogAssets/Shaders/defaultsprite");


		renderer2D->SetShader(*std::dynamic_pointer_cast<Shader>(sp));

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

	int Engine::Run()
	{
		auto rs = resources->Load("Texture2D", "DogAssets/Images/square.png");
		
		while (running && !glfwWindowShouldClose(window->GetWindowHandle())) {
			glfwPollEvents();
			if (glfwGetKey(window->GetWindowHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				running = false;
			}


			std::shared_ptr<Texture2D> texture = std::dynamic_pointer_cast<Texture2D>(rs);

			renderer2D->beginFrame();

			renderer2D->DrawSprite(*texture, glm::vec2(0.0f, 0.0f), glm::vec2(100.0f, 100.0f));
			
			renderer2D->endFrame();

			glfwSwapBuffers(window->GetWindowHandle());

		}


		return 0;
	}

} // namespace Dog
#include <PCH/dogpch.h>
#include "scene.h"
#include "Dog/Logger/logger.h"

#include "Entity/entity.h"
#include "Entity/components.h"
#include "Dog/engine.h"
#include "Dog/Graphics/Renderer/Renderer2D/renderer2d.h"

#include "Dog/Graphics/Framebuffer/framebuffer.h"

#include "Dog/Graphics/Renderer/Camera/orthoCamera.h"
#include "Dog/Graphics/Renderer/Camera/orthoCameraController.h"

#include "Dog/Graphics/Window/Iwindow.h"

#define DOG_SCENE_LOGGING 0

namespace Dog {

	Scene::Scene(const std::string& name)
	{
		sceneName = name;

		FrameBufferSpecification fbSpec;
		fbSpec.width = 1280;
		fbSpec.height = 720;
		fbSpec.samples = 1;
		frameBuffer = std::make_shared<FrameBuffer>(fbSpec);

		width = Engine::Get().GetWindow()->GetWidth();
		height = Engine::Get().GetWindow()->GetHeight();

		Shader::SetResolutionUBO(glm::vec2(fbSpec.width, fbSpec.height));

		sceneCameraController = std::make_shared<OrthographicCameraController>((float)width / (float)height);
		sceneCameraController->GetCamera()->UpdateUniforms(); // Should be moved to the renderer (?) or somewhere else

		eventSceneFBResize = SUBSCRIBE_EVENT(Event::SceneResize, frameBuffer->OnResize);
		eventSceneCamResize = SUBSCRIBE_EVENT(Event::SceneResize, sceneCameraController->OnResize);
		eventPlayButtonPressed = SUBSCRIBE_EVENT(Event::PlayButtonPressed, OnPlayButtonPressed);
		eventStopButtonPressed = SUBSCRIBE_EVENT(Event::StopButtonPressed, OnStopButtonPressed);
	}

	Scene::~Scene()
	{
	}

	void Scene::OnPlayButtonPressed(const Event::PlayButtonPressed& event)
	{
		DOG_INFO("Play button pressed.");
	}

	void Scene::OnStopButtonPressed(const Event::StopButtonPressed& event)
	{
		DOG_INFO("Stop button pressed.");
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity newEnt(this);

		TagComponent& tg = newEnt.AddComponent<TagComponent>();
		TransformComponent& tr = newEnt.AddComponent<TransformComponent>();
		SpriteComponent& sc = newEnt.AddComponent<SpriteComponent>();
		
		tg.Tag = name;
		tr.Translation -= tr.Scale * 0.5f;

		return newEnt;
	}

	void Scene::InternalInit()
	{
#if DOG_SCENE_LOGGING
		DOG_INFO("Scene {0} init.", sceneName);
#endif
	}

	void Scene::InternalUpdate(float dt)
	{
#if DOG_SCENE_LOGGING
		DOG_INFO("Scene {0} Update.", sceneName);
#endif

		sceneCameraController->OnUpdate(dt);
		sceneCameraController->UpdateUniforms();
	}

	void Scene::InternalRender(float dt)
	{
#if DOG_SCENE_LOGGING
		DOG_INFO("Scene {0} Render.", sceneName);
#endif

		std::shared_ptr<Renderer2D>& renderer2D = Engine::Get().GetRenderer2D();

		frameBuffer->Bind();

		renderer2D->beginFrame();

		registry.view<TransformComponent, SpriteComponent>().each
		([&](const auto& entity, const TransformComponent& transform, const SpriteComponent& sprite) {
			if (registry.all_of<ShaderComponent>(entity)) {
				ShaderComponent& shader = registry.get<ShaderComponent>(entity);
				renderer2D->DrawSprite(sprite.texturePath, transform.GetTransform(), sprite.Color, glm::vec2(0), 0.0f, shader.shaderPath);
			}
			else {
				renderer2D->DrawSprite(sprite.texturePath, transform.GetTransform(), sprite.Color);
			}

		});

		renderer2D->endFrame();

		frameBuffer->Unbind();
	}

	void Scene::InternalExit()
	{
#if DOG_SCENE_LOGGING
		DOG_INFO("Scene {0} exit.", sceneName);
#endif
	}

}

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

		cameraController = std::make_shared<OrthographicCameraController>((float)width / (float)height);
		camera = cameraController->GetCamera();

		camera->UpdateUniforms(); // Should be moved to the renderer (?) or somewhere else

		eventSceneFBResize = SUBSCRIBE_EVENT(Event::SceneResize, frameBuffer->OnResize);
		eventSceneCamResize = SUBSCRIBE_EVENT(Event::SceneResize, cameraController->OnResize);
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

	Entity Scene::CreateEntity()
	{
		Entity newEnt(this);

		TagComponent& tg = newEnt.AddComponent<TagComponent>();
		TransformComponent& tr = newEnt.AddComponent<TransformComponent>();
		tr.Translation -= tr.Scale * 0.5f;
		SpriteComponent& sc = newEnt.AddComponent<SpriteComponent>();

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

		cameraController->OnUpdate(dt);
		camera->UpdateUniforms();
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
		([renderer2D](const auto& entity, const TransformComponent& transform, const SpriteComponent& sprite) {
			renderer2D->DrawSprite(sprite.texturePath, transform.GetTransform(), sprite.Color);
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

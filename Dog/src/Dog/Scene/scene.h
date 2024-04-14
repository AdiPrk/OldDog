#pragma once

#include "sceneManager.h"

namespace Dog {

	class FrameBuffer;
	class OrthographicCamera;
	class OrthographicCameraController;

	class Scene {
	public:
		Scene(const std::string&);
		~Scene();


		virtual void Init() {}
		virtual void Update(float dt) {}
		virtual void Render(float dt) {}
		virtual void Exit() {}

		const std::string& GetName() const { return sceneName; }

		class Entity CreateEntity();

		std::shared_ptr<OrthographicCamera>& GetCamera() { return camera; }
		std::shared_ptr<OrthographicCameraController>& GetCameraController() { return cameraController; }

		const std::shared_ptr<FrameBuffer>& GetFrameBuffer() const { return frameBuffer; }
		std::shared_ptr<FrameBuffer>& GetFrameBuffer() { return frameBuffer; }

		void OnPlayButtonPressed(const Event::PlayButtonPressed& event);
		void OnStopButtonPressed(const Event::StopButtonPressed& event);

	private:
		// Scene name only used for debug purposes
		std::string sceneName;

		// Scene width & height, equal to window width & height
		unsigned int width;
		unsigned int height;
		
		// Entity related stuff
		friend Entity;
		entt::registry& GetRegistry() { return registry; }
		entt::registry registry;

		// Framebuffer for the editor to render the scene
		std::shared_ptr<FrameBuffer> frameBuffer;
		
		// Functions that run before or after the user's scene functions
		friend SceneManager;
		void InternalInit();
		void InternalUpdate(float dt);
		void InternalRender(float dt);
		void InternalExit();

		// Scene camera
		std::shared_ptr<OrthographicCamera> camera;
		std::shared_ptr<OrthographicCameraController> cameraController;

		// Event handles
		Events::Handle<Event::SceneResize> eventSceneFBResize;
		Events::Handle<Event::SceneResize> eventSceneCamResize;
		Events::Handle<Event::PlayButtonPressed> eventPlayButtonPressed;
		Events::Handle<Event::StopButtonPressed> eventStopButtonPressed;
	};

}

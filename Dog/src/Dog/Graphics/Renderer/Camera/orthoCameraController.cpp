#include <PCH/dogpch.h>
#include "orthoCameraController.h"
#include "Dog/Input/input.h"

namespace Dog {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio)
		: aspectRatio(aspectRatio)
	{
		camera = std::make_shared<OrthographicCamera>(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
	}

	void OrthographicCameraController::OnUpdate(float dt)
	{
		float speed = 25.0f * dt;

		if (Input::isKeyDown(Key::W)) {
			camera->SetPosition(camera->GetPosition() - glm::vec3(0.0f, 0.1f, 0.0f) * zoomLevel * speed);
		}
		if (Input::isKeyDown(Key::S)) {
			camera->SetPosition(camera->GetPosition() + glm::vec3(0.0f, 0.1f, 0.0f) * zoomLevel * speed);
		}
		if (Input::isKeyDown(Key::A)) {
			camera->SetPosition(camera->GetPosition() - glm::vec3(0.1f, 0.0f, 0.0f) * zoomLevel * speed);
		}
		if (Input::isKeyDown(Key::D)) {
			camera->SetPosition(camera->GetPosition() + glm::vec3(0.1f, 0.0f, 0.0f) * zoomLevel * speed);
		}

		float zoomSpeed = 0.02f;
		if (Input::isKeyDown(Key::Q)) {
			zoomLevel -= zoomSpeed;
			if (zoomLevel < 0.1f)
				zoomLevel = 0.1f;
			camera->SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		}
		if (Input::isKeyDown(Key::E)) {
			zoomLevel += zoomSpeed;
			if (zoomLevel > 10.0f)
				zoomLevel = 10.0f;
			camera->SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		}
	}

	void OrthographicCameraController::OnResize(const Event::SceneResize& event)
	{
		aspectRatio = float(event.width) / float(event.height);
		camera->SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
	}

} // namespace Dog

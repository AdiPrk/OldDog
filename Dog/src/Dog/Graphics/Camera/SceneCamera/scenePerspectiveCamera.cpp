#include <PCH/dogpch.h>
#include "scenePerspectiveCamera.h"

#include "Dog/Input/input.h"

namespace Dog {

	ScenePerspectiveCamera::ScenePerspectiveCamera()
	{
		camera = std::make_shared<PerspectiveCamera>();
		camera->Position.z = 3.0f;
	}

	void ScenePerspectiveCamera::OnUpdate(float dt)
	{
		float speed = 2.5f * dt;

		glm::vec3 front = glm::normalize(camera->Front);
		glm::vec3 right = glm::normalize(camera->Right);
		glm::vec3 up = glm::normalize(camera->Up);

		// keyboard
		if (Input::isKeyDown(Key::Q)) {
			camera->SetPosition(camera->GetPosition() + front * speed);
		}
		if (Input::isKeyDown(Key::E)) {
			camera->SetPosition(camera->GetPosition() - front * speed);
		}
		if (Input::isKeyDown(Key::A)) {
			camera->SetPosition(camera->GetPosition() - right * speed);
		}
		if (Input::isKeyDown(Key::D)) {
			camera->SetPosition(camera->GetPosition() + right * speed);
		}
		if (Input::isKeyDown(Key::W)) {
			camera->SetPosition(camera->GetPosition() + up * speed);
		}
		if (Input::isKeyDown(Key::S)) {
			camera->SetPosition(camera->GetPosition() - up * speed);
		}

		// mouse
		// print mouse delta x, y
		float xoffset = Input::isMouseDown(Mouse::RIGHT) ? (Input::getMouseScreenXDelta() * 0.2f) : 0.0f;
		float yoffset = Input::isMouseDown(Mouse::RIGHT) ? (Input::getMouseScreenYDelta() * 0.2f) : 0.0f;

		// log it
		// DOG_INFO("Mouse X: {0}, Mouse Y: {1}", xoffset, yoffset);

		camera->Yaw += xoffset;
		camera->Pitch -= yoffset;

		// scroll to move back and forth too
		float zoomSpeed = 1.0f;
		float yoffsetscroll = Input::getMouseScrollDeltaY();

		if (yoffsetscroll != 0) {
			camera->Position += front * yoffsetscroll * zoomSpeed;
		}

		// log mouse world pos
		float screenXDelta = Input::getMouseScreenXDelta();
		float screenYDelta = Input::getMouseScreenYDelta();

		// middle click to pan camera depending on where mouse is dragged after middle click
		if (Input::isMouseDown(Mouse::LEFT)) {
			const float panSpeed = 0.5f * dt;
			glm::vec3 newPos = camera->GetPosition();
			newPos -= right * screenXDelta * panSpeed;
			newPos.y += screenYDelta * panSpeed;
			camera->SetPosition(newPos);
		}
		else {
			isPanning = false;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		camera->updateCameraVectors();
	}

	void ScenePerspectiveCamera::OnSceneResize(const Event::SceneResize& event)
	{
		camera->aspectRatio = float(event.width) / float(event.height);
		camera->UpdateUniforms();
	}

}

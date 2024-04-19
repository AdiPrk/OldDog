#pragma once

#include "../orthoCamera.h"

namespace Dog {

	class SceneOrthographicCamera
	{
	public:
		SceneOrthographicCamera(float aspectRatio);
		~SceneOrthographicCamera() = default;

		void OnUpdate(float dt);
		void OnSceneResize(const Event::SceneResize& event);

		void UpdateUniforms() { camera->UpdateUniforms(); }

		glm::mat4 GetProjectionMatrix() const { return camera->GetProjectionMatrix(); }
		glm::mat4 GetViewMatrix() { return camera->GetViewMatrix(); }

	private:
		std::shared_ptr<OrthographicCamera> camera;
		float aspectRatio;
		float zoomLevel = 1.0f;

		bool isRotating = false;
		bool isPanning = false;
		bool isZooming = false;

		glm::vec2 initialMousePos = { 0.0f, 0.0f };
		glm::vec3 initialCameraPos = { 0.0f, 0.0f, 0.0f };
		float initialZoomLevel = 0.0f;
		float initialRotation = 0.0f;
	};

} // namespace Dog

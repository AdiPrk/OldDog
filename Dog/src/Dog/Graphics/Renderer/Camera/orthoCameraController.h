#pragma once

#include "Dog/Graphics/Renderer/Camera/orthoCamera.h"

namespace Dog {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio);
		~OrthographicCameraController() = default;

		void OnUpdate(float dt);
		void OnResize(const Event::SceneResize& event);

		std::shared_ptr<OrthographicCamera> GetCamera() { return camera; }
		const std::shared_ptr<OrthographicCamera> GetCamera() const { return camera; }

		void UpdateUniforms() { camera->UpdateUniforms(); }

		float GetZoomLevel() const { return zoomLevel; }
		void SetZoomLevel(float level) { zoomLevel = level; }

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

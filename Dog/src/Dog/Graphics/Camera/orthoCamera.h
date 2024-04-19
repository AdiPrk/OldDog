#pragma once

namespace Dog {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		OrthographicCamera(float aspect, float zoom);
		~OrthographicCamera() = default;

		void OnResize(const Event::SceneResize& event);

		void SetZoomLevel(float aspect, float level);
		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const;
		void SetPosition(const glm::vec3& pos);

		float GetRotation() const;
		void SetRotation(float rot);

		const glm::mat4& GetProjectionMatrix() const;
		const glm::mat4& GetViewMatrix(); // Can't be const because might have to recalculate
		const glm::mat4& GetViewProjectionMatrix();

		void UpdateUniforms();

		bool GetDirty() const { return recalculateViewMatrix; }

	private:
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 viewProjectionMatrix;

		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		float rotation = 0.0f;

		float aspectRatio = 16.0f / 9.0f;
		float zoomLevel = 0.8f;

		bool recalculateViewMatrix = true;
	};

} // namespace Dog

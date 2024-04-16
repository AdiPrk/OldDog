#include <PCH/dogpch.h>
#include "orthoCamera.h"
#include "Dog/Graphics/Renderer/Shaders/shader.h"
#include "Dog/engine.h"
#include "Dog/Graphics/Window/window.h"

namespace Dog {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: viewMatrix(1.0f)
		, projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
	{
		//zoomLevel = 0.9f;
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	OrthographicCamera::OrthographicCamera(float aspect, float zoom)
	{
		aspectRatio = aspect;
		zoomLevel = zoom;
		SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
	}

	void OrthographicCamera::OnResize(const Event::SceneResize& event)
	{
		const int& width = event.width;
		const int& height = event.height;
		// DOG_INFO("OrthographicCamera::OnResize({0}, {1})", width, height);

		aspectRatio = (float)width / (float)height;
		zoomLevel *= 0.5f;
		SetProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	const glm::vec3& OrthographicCamera::GetPosition() const
	{
		return position; 
	}

	void OrthographicCamera::SetPosition(const glm::vec3& pos) 
	{
		position = pos; 
		recalculateViewMatrix = true; 
	}

	float OrthographicCamera::GetRotation() const 
	{
		return rotation; 
	}

	void OrthographicCamera::SetRotation(float rot) 
	{ 
		rotation = rot;
		recalculateViewMatrix = true; 
	}

	const glm::mat4& OrthographicCamera::GetProjectionMatrix() const
	{ 
		return projectionMatrix; 
	}

	const glm::mat4& OrthographicCamera::GetViewMatrix()
	{ 
		// log recalc
		if (recalculateViewMatrix)
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
								* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));

			viewMatrix = glm::inverse(transform);
			viewProjectionMatrix = projectionMatrix * viewMatrix;
			
			recalculateViewMatrix = false;
		}

		return viewMatrix;
	}

	const glm::mat4& OrthographicCamera::GetViewProjectionMatrix()
	{ 
		return viewProjectionMatrix;
	}

	void OrthographicCamera::UpdateUniforms()
	{
		// Update the uniforms
		Shader::SetViewAndProjectionView(GetViewMatrix(), GetViewProjectionMatrix());
	}


} // namespace Dog

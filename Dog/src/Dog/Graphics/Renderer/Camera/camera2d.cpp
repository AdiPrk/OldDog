#include <PCH/dogpch.h>
#include "camera2d.h"
#include "Dog/Graphics/Renderer/Shaders/shader.h"
#include "Dog/engine.h"
#include "Dog/Graphics/Window/window.h"

namespace Dog {

    Camera2D::Camera2D(int width, int height)
        : m_CameraSize(width, height)
        , m_BoundingRect() 
        , window_width(width)
        , window_height(height)
    {
        aspect_ratio = (float)width / (float)height;
        left = 0, right = (float)width;
        bottom = (float)height, top = 0;
    }

    void Camera2D::OnResize(const Event::SceneResize& event)
    {
        const float aspectRatio = 16.0f / 9.0f;
        const int& width = event.width;
        const int& height = event.height;

        window_width = width;
        window_height = height;
        m_CameraSize = { window_width, window_height };

        // Calculate the new width and height to maintain the aspect ratio
        float newWidth, newHeight;
        if (height * aspectRatio <= width) {
            // Window is wider than the desired aspect ratio
            newWidth = height * aspectRatio;
            newHeight = (float)height;
        }
        else {
            // Window is taller than the desired aspect ratio
            newWidth = (float)width;
            newHeight = width / aspectRatio;
        }

        // Calculate the left, right, bottom, and top values for the orthographic projection
        left = -newWidth / 2.0f;
        right = newWidth / 2.0f;
        bottom = -newHeight / 2.0f;
        top = newHeight / 2.0f;

        DOG_INFO("Camera2D::OnResize: left: {0}, right: {1}, top: {2}, bottom: {3}", left, right, top, bottom);
    }

    glm::mat4 Camera2D::GetViewMatrix() {
        glm::mat4 view = glm::mat4(1.0f);

        //view = glm::translate(view, { m_CameraSize * 0.5f, 0.f });
        //view = glm::scale(view, glm::vec3(m_Zoom, m_Zoom, 1.0f));
        //view = glm::translate(view, glm::vec3(-m_Position, 0.0f));

        return view;
    }

    void Camera2D::UpdateUniforms() {
        glm::mat4 view = GetViewMatrix();
        glm::mat4 proj = glm::ortho(left, right, top, bottom, -1.0f, 1.0f);
        glm::mat4 projView = proj * view;

        Shader::SetViewAndProjectionView(view, projView);
    }

    void Camera2D::CalculateBoundingRect() {
        m_BoundingRect.width = m_CameraSize.x / m_Zoom;
        m_BoundingRect.height = m_CameraSize.y / m_Zoom;

        m_BoundingRect.left = m_Position.x - m_BoundingRect.width / 2;
        m_BoundingRect.top = m_Position.y - m_BoundingRect.height / 2;

        m_BoundingRect.right = m_BoundingRect.left + m_BoundingRect.width;
        m_BoundingRect.bottom = m_BoundingRect.top + m_BoundingRect.height;
    }

    void Camera2D::MoveTo(glm::vec2 moveTo) {
        m_Position = moveTo;
    }

    void Camera2D::MoveTo(glm::vec2 moveTo, Rect bounds) {
        CalculateBoundingRect();

        if (bounds.width >= m_BoundingRect.width) // to avoid crashing with clamp
        {
            moveTo.x = std::clamp(moveTo.x, bounds.left + m_BoundingRect.width / 2.f, bounds.right - m_BoundingRect.width / 2.f);
            moveTo.y = std::clamp(moveTo.y, bounds.top + m_BoundingRect.height / 2.f, bounds.bottom - m_BoundingRect.height / 2.f);
        }

        m_Position = moveTo;
    }

    void Camera2D::UpdateZoom(float dt) {
        m_Zoom = std::lerp(m_Zoom, m_TargetZoom, 6.0f * dt);
        if (fabsf(m_Zoom - m_TargetZoom) < 0.0005f) m_Zoom = m_TargetZoom;
        CalculateBoundingRect();
    }

    void Camera2D::GlideTo(glm::vec2 glideTo, float dt) {
        const Rect& bounds = m_MovementBounds;

        if (bounds.width >= m_BoundingRect.width) // to avoid crashing with clamp
        {
            glideTo.x = std::clamp(glideTo.x, bounds.left + m_BoundingRect.width / 2.f, bounds.right - m_BoundingRect.width / 2.f);
            glideTo.y = std::clamp(glideTo.y, bounds.top + m_BoundingRect.height / 2.f, bounds.bottom - m_BoundingRect.height / 2.f);
        }
        else {
            glideTo.x = std::clamp(glideTo.x, bounds.right - m_BoundingRect.width / 2.f, bounds.left + m_BoundingRect.width / 2.f);
            glideTo.y = std::clamp(glideTo.y, bounds.bottom - m_BoundingRect.height / 2.f, bounds.top + m_BoundingRect.height / 2.f);
        }

        if (glm::distance(glideTo, m_Position) > 0.1f) {
            float distance = glm::distance(glideTo, m_Position);
            float baseSpeed = 50.0f; // Base speed
            float minSpeed = 10.0f; // Minimum speed to maintain
            float speedFactor = 0.2f; // Adjust this factor to control deceleration

            // Calculate speed based on distance
            float speed = baseSpeed * (1 - powf(distance / speedFactor, 2));
            speed = glm::max(speed, minSpeed); // Ensure speed doesn't fall below minSpeed

            // Update position
            m_Position += (glideTo - m_Position) * speed * dt;
        }
        else {
            m_Position = glideTo;
        }

        CalculateBoundingRect();
    }

} // namespace Dog

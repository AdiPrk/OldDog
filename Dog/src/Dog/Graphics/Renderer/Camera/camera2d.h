#pragma once

/**
 * UNUSED CAM CLASS - Use Ortho Camera for 2D.
 */

namespace Dog {

    class Camera2D {
    public:

        Camera2D(int width, int height);

        void OnResize(const Event::SceneResize& event);

        glm::mat4 GetViewMatrix();

        void UpdateUniforms();

        void CalculateBoundingRect();

        void UpdateZoom(float dt);

        void MoveTo(glm::vec2 moveTo);
        void MoveTo(glm::vec2 moveTo, Rect bounds);

        void GlideTo(glm::vec2 glideTo, float dt);

        const Rect Bounds() const { return m_BoundingRect; }
        void SetScale(float scale) { m_TargetZoom = scale; }
        void SetScaleInstant(float scale) { m_TargetZoom = scale; m_Zoom = scale; }

        const bool IsZoomEqualTarget() const { return m_Zoom == m_TargetZoom; }

        void SetMovementBounds(const Rect& bounds) {
            m_MovementBounds = bounds;
        }

    private:
        Rect m_MovementBounds;
        Rect m_BoundingRect;
        glm::vec2 m_Position = glm::vec2(0.0f, 0.0f);
        glm::vec2 m_CameraSize;
        float m_Zoom = 1.0f;
        float m_TargetZoom = 1.0f;
        int window_width = 0;
        int window_height = 0;
        float aspect_ratio = 16.0f / 9.0f;

        float left, right, top, bottom;
    };

}

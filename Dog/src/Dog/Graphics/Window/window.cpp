#include <PCH/dogpch.h>
#include "window.h"
#include "Dog/engine.h"
#include "Dog/Events/event.h"

namespace Dog {

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    void glfw_error_callback(int error, const char* description)
    {
        std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
    }

#define DEBUG_OPENGL_OUTPUT 1

#if DEBUG_OPENGL_OUTPUT
    void GLAPIENTRY OpenGLDebugCallback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam) {
        // Displaying the debug message
        if (type == GL_DEBUG_TYPE_OTHER) return;

        std::cerr << "OpenGL Debug Message (ID: " << id << "):\n";
        std::cerr << "Message: " << message << "\n";
        std::cerr << "Source: ";

        switch (source) {
        case GL_DEBUG_SOURCE_API: std::cerr << "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cerr << "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: std::cerr << "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION: std::cerr << "Application"; break;
        case GL_DEBUG_SOURCE_OTHER: std::cerr << "Other"; break;
        }

        std::cerr << "\nType: ";

        switch (type) {
        case GL_DEBUG_TYPE_ERROR: std::cerr << "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cerr << "Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY: std::cerr << "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: std::cerr << "Performance"; break;
        case GL_DEBUG_TYPE_OTHER: std::cerr << "Other"; break;
        }

        std::cerr << "\nSeverity: ";

        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: std::cerr << "High"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: std::cerr << "Medium"; break;
        case GL_DEBUG_SEVERITY_LOW: std::cerr << "Low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "Notification"; break;
        }

        std::cerr << "\n\n";

        if (type == GL_DEBUG_TYPE_ERROR) {
             __debugbreak();
        }
    }
#endif

    unsigned int windowWidth = 1600;
    unsigned int windowHeight = 900;
    float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    Window::Window(unsigned int screenWidth, unsigned int screenHeight, const std::string& name)
    {
        m_Width = screenWidth;
        m_Height = screenHeight;
        m_AspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);

        windowWidth = screenWidth;
        windowHeight = screenHeight;
        aspectRatio = m_AspectRatio;

        m_LastFrameTime = std::chrono::high_resolution_clock::now();

        if (!glfwInit()) {
            std::cout << "Oh Noes, no glfw init!";
            return;
        }
        
        glfwSetErrorCallback(glfw_error_callback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        window = glfwCreateWindow(m_Width, m_Height, name.c_str(), NULL, NULL);
        
        if (!window)
        {
            const char* description;
            int error = glfwGetError(&description);
            if (error != GLFW_NO_ERROR)
            {
                fprintf(stderr, "Error Creating Window: %d: %s\n", error, description);
            }

            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);
        SetFramebufferSizeCallback(framebuffer_size_callback);

        if (glewInit() != GLEW_OK) {
            std::cout << "oh noes, glew didn't init properly!" << std::endl;
            return;
        }

        std::cout << glGetString(GL_VERSION) << std::endl;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        glfwSwapInterval(0);
        vSync = false;

#if DEBUG_OPENGL_OUTPUT
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLDebugCallback, nullptr);
        //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

    }

    Window::~Window()
    {
        if (window) {
            glfwDestroyWindow(window);
        }

        glfwTerminate();
    }

    bool Window::IsRunning() {
        return glfwWindowShouldClose(window) == 0;
    }

    int windowedWidth = 1600, windowedHeight = 900;
    int windowedPosX = 100, windowedPosY = 100;

    void Window::ToggleFullscreen()
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        if (glfwGetWindowMonitor(window)) {
            // If in fullscreen mode, switch to windowed mode with previous window size and position
            glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, windowedWidth, windowedHeight, 0);
            m_IsFullscreen = false;
        }
        else {
            // Store the current window size and position before switching to fullscreen
            glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
            glfwGetWindowSize(window, &windowedWidth, &windowedHeight);

            // Switch to fullscreen mode
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            m_IsFullscreen = true;
        }
    }

    void Window::SetVSync(bool enabled)
    {
        glfwSwapInterval(enabled ? 1 : 0);
        vSync = enabled;
    }

    void Window::SetFullscreen(bool fullScreen)
    {
        if (m_IsFullscreen != fullScreen) {
            ToggleFullscreen();
        }
    }

    void Window::SetKeyCallback(GLFWkeyfun callback) {
        glfwSetKeyCallback(window, callback);
    }

    void Window::SetFramebufferSizeCallback(GLFWframebuffersizefun callback) {
        glfwSetFramebufferSizeCallback(window, callback);
    }

    const std::string Window::GetTitle() {
		return glfwGetWindowTitle(window);
	}

    void Window::SetTitle(const std::string& title) {
        glfwSetWindowTitle(window, title.c_str());
    }

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        PUBLISH_EVENT(Event::WindowResize, width, height);

        int newWidth = width;
        int newHeight = (int)(width / aspectRatio);

        if (newHeight > height)
        {
            newHeight = height;
            newWidth = (int)(height * aspectRatio);
        }

        int xOffset = (width - newWidth) / 2;
        int yOffset = (height - newHeight) / 2;

        glViewport(xOffset, yOffset, newWidth, newHeight);
    }

}
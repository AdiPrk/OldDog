#pragma once


#include "inputMap.h"

struct GLFWwindow;
struct GLFWcursor;

namespace Dog {

	class Input {
	public:
		static void Init(GLFWwindow* window);
		static void Update();

		static bool isKeyDown(const Key& key);
		static bool isMouseDown(const Mouse& button);

		static glm::vec2 getMousePosition() { return { mouseX, mouseY }; }
		static float getMouseX() { return mouseX; }
		static float getMouseY() { return mouseY; }

		static void SetKeyInputLocked(bool locked);
		static void SetMouseInputLocked(bool locked);

	private:
		struct KeyStates
		{
			bool keyDown = false;
			bool keyTriggered = false;
			bool keyReleased = false;
			bool prevTriggered = false;
			bool doTriggered = false;
		};

		struct MouseStates
		{
			bool mouseDown = false;
			bool mouseTriggered = false;
			bool mouseReleased = false;
			bool prevTriggered = false;
			bool doTriggered = false;
		};

		static KeyStates keyStates[static_cast<int>(Key::LAST)];
		static MouseStates mouseStates[static_cast<int>(Mouse::LAST)];
		static GLFWcursor* standardCursor;

		static float degree;
		static float mouseX;
		static float mouseY;

		static bool keyInputLocked;
		static bool mouseInputLocked;

		static void keyPressCallback(GLFWwindow* windowPointer, int key, int scanCode, int action, int mod);
		static void mousePressCallback(GLFWwindow* windowPointer, int mouseButton, int action, int mod);
		static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	};

}

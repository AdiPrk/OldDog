#include <PCH/dogpch.h>
#include "input.h"
#include "inputMap.h"
#include "Dog/Logger/logger.h"

#define DO_INPUT_LOGGING 0

namespace Dog {
	
#define IND(x) static_cast<int>(x)

	Input::KeyStates Input::keyStates[static_cast<int>(Key::LAST)];
	Input::MouseStates Input::mouseStates[static_cast<int>(Mouse::LAST)];

	bool Input::keyInputLocked = false;
	bool Input::mouseInputLocked = false;

	float Input::degree = 0;
	float Input::mouseX = 0;
	float Input::mouseY = 0;
	GLFWcursor* Input::standardCursor = nullptr;

	void Input::Init(GLFWwindow* window)
	{
		glfwSetKeyCallback(window, keyPressCallback);
		glfwSetMouseButtonCallback(window, mousePressCallback);
		glfwSetScrollCallback(window, mouseScrollCallback);

		standardCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		glfwSetCursor(window, standardCursor);
	}

	void Input::Update()
	{
		glfwPollEvents();
	}

	bool Input::isKeyDown(const Key& key)
	{
		return keyStates[IND(key)].keyDown;
	}

	bool Input::isMouseDown(const Mouse& button)
	{
		return mouseStates[IND(button)].mouseDown;
	}

	void Input::SetKeyInputLocked(bool locked)
	{
		keyInputLocked = locked;
	}

	void Input::SetMouseInputLocked(bool locked)
	{
		mouseInputLocked = locked;
	}

	void Input::keyPressCallback(GLFWwindow* windowPointer, int key, int scanCode, int action, int mod)
	{
		// check if imgui is capturing the keyboard
		if (keyInputLocked) {
#if DO_INPUT_LOGGING
			DOG_INFO("Key {0} Ignored - ImGui is capturing it.", key);
#endif
			return;
		}

		if (key < 0 || key > static_cast<int>(Key::LAST))
			return;

		if (action == GLFW_PRESS)
		{
			keyStates[key].keyDown = true;
#if DO_INPUT_LOGGING
			DOG_INFO("Key Pressed: {0}", key);
#endif
		}
		else if (action == GLFW_RELEASE)
		{
			keyStates[key].keyDown = false;
#if DO_INPUT_LOGGING
			DOG_INFO("Key Released: {0}", key);
#endif
		}
	}

	void Input::mousePressCallback(GLFWwindow* windowPointer, int mouseButton, int action, int mod)
	{
		// check if imgui is capturing the mouse
		if (mouseInputLocked) {
#if DO_INPUT_LOGGING
			DOG_INFO("Mouse Press {0} Ignored - ImGui is capturing it.", mouseButton);
#endif
			return;
		}

		if (mouseButton < 0 || mouseButton > static_cast<int>(Mouse::LAST))
			return;

		if (action == GLFW_PRESS)
		{
			mouseStates[mouseButton].mouseDown = true;
#if DO_INPUT_LOGGING
			DOG_INFO("Mouse Pressed: {0}", mouseButton);
#endif
		}
		else if (action == GLFW_RELEASE)
		{
			mouseStates[mouseButton].mouseDown = false;
#if DO_INPUT_LOGGING
			DOG_INFO("Mouse Released: {0}", mouseButton);
#endif
		}
	}

	void Input::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (mouseInputLocked) {
#if DO_INPUT_LOGGING
			DOG_INFO("Mouse Scroll Ignored - ImGui is capturing it.");
#endif
			return;
		}

		degree += float(yoffset);
#if DO_INPUT_LOGGING
		DOG_INFO("Mouse Scrolled: {0}", degree);
#endif
	}

}

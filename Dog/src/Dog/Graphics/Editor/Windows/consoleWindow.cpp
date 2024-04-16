#include <PCH/dogpch.h>
#include "consoleWindow.h"

#include "Dog/Graphics/Framebuffer/framebuffer.h"
#include "Dog/Input/input.h"
#include "Dog/Graphics/Texture/texture2d.h"
#include "Dog/engine.h"
#include "Dog/Scene/sceneManager.h"
#include "Dog/Scene/scene.h"

namespace Dog {

	void UpdateConsoleWindow() {
		ImGui::Begin("Console");

		ImGui::Text("Console Window");

		ImGui::End(); // Console
	}

}

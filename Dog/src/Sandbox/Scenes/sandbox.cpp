#include <PCH/pch.h>
#include "sandbox.h"

void SandboxScene::Init() {
	printf("Hello, Woof! ~From Sandbox\n");

	Dog::Entity box = CreateEntity();
}

void SandboxScene::Update(float dt) {
	// printf("Sandbox Update\n");
	
	if (Dog::Input::isKeyDown(Dog::Key::W)) {
		// printf("Key 'W'oof is pressed! ~From Sandbox - Update.\n");
	}
}

void SandboxScene::Render(float dt) {
	// printf("Sandbox Render\n");
}

void SandboxScene::Exit() {
	printf("Woof, Bye! ~From Sandbox\n");
}

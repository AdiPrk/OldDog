#include <PCH/pch.h>
#include "sandbox.h"

void SandboxScene::Init() {
	printf("Hello, Woof! ~From Sandbox\n");

	Dog::Entity whitebox = CreateEntity("White Box");

	Dog::Entity greenbox = CreateEntity("Green Box");
	Dog::Entity redbox = CreateEntity("Red Box");
	Dog::Entity bluebox = CreateEntity("Blue Box");


	whitebox.GetComponent<Dog::SpriteComponent>().Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	redbox.GetComponent<Dog::SpriteComponent>().Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	greenbox.GetComponent<Dog::SpriteComponent>().Color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	bluebox.GetComponent<Dog::SpriteComponent>().Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	whitebox.GetComponent<Dog::TransformComponent>().Translation -= glm::vec3(0.0f, 1.25f, 0.0f);
	redbox.GetComponent<Dog::TransformComponent>().Translation -= glm::vec3(1.25f, 0.0f, 0.0f);
	bluebox.GetComponent<Dog::TransformComponent>().Translation += glm::vec3(1.25f, 0.0f, 0.0f);
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

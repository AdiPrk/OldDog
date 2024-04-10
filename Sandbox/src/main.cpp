#include <common_pch.h>
#include <iostream>
#include <dog.h>

using namespace Dog;

class Sandbox : public Scene {
public:
	Sandbox() : Scene("Sandbox") {}

	void Init() override {
		printf("Hello, Woof! ~From Sandbox\n");
	}

	void Update(float dt) override {
		// printf("Sandbox Update\n");
		if (Input::isKeyDown(Key::W)) {
			printf("Key 'W'oof is pressed! ~From Sandbox - Update.\n");
		}
	}

	void Render(float dt) override {
		// printf("Sandbox Render\n");
	}

	void Exit() override {
		printf("Woof, Bye! ~From Sandbox\n");
	}
};

int main() {
	Dog::Engine Engine(1280, 720, "Woof");

	Sandbox sandbox;
	Entity entity(&sandbox);

	std::vector<int> asdf = { 1, 9, 4, 7, 5 };
	std::sort(asdf.begin(), asdf.end());

	glm::vec4 omg(1.0f);

	entity.AddComponent<TagComponent>();
	entity.GetComponent<TagComponent>().Tag = "noway";

	std::string how;

	return Engine.Run(&sandbox);
}
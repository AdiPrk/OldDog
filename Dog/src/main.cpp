#include <PCH/dogpch.h>
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
	Dog::Engine& Engine = Engine::Get(1280, 720, "Woof");

	Sandbox sandbox;

	Entity se = sandbox.CreateEntity();

	return Engine.Run(&sandbox);
}
#include <PCH/pch.h>
#include "Sandbox/Scenes/sandbox.h"
#include "Dog/dog.h"

int main() {
	Dog::Engine& Engine = Dog::Engine::Get(1280, 720, "Woof");

	SandboxScene sandbox;
	return Engine.Run(&sandbox);
}
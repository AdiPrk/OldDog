#include <PCH/pch.h>
#include "Sandbox/Scenes/sandbox.h"
#include "Dog/dog.h"

int main() {
	Dog::EngineSpec specs;
	specs.name = "Woof";
	specs.width = 1280; 
	specs.height = 720; 
	specs.fps = 60;

	Dog::Engine& Engine = Dog::Engine::Get(specs);

	SandboxScene sandbox;
	return Engine.Run(&sandbox);
}
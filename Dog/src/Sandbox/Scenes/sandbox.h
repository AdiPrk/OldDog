#pragma once
#include "Dog/dog.h"

class SandboxScene : public Dog::Scene {
public:
	SandboxScene() : Scene("Sandbox") {}

	void Init() override;
	void Update(float dt) override;
	void Render(float dt) override;
	void Exit() override;
};
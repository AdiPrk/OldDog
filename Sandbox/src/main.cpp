#include <iostream>
#include <dog.h>

int main() {
	Dog::Engine Engine(1280, 720, "Woof");

	std::cout << "Hello, Woof!" << std::endl;

	return Engine.Run();
}
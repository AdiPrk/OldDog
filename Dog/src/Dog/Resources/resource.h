#pragma once

#include "Dog/core.h"

namespace Dog {

	class DOG_API Resource {
	public:
		virtual ~Resource() {};

		// Must be implemented by every resource.
		// Loads the resource from a given path.
		virtual void load(const std::string& path) = 0;

		// Not yet implemented.
		// To be done later.
		virtual void save(const std::string& path) {}
	};

}

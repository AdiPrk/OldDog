#pragma once



namespace Dog {

	class Resource {
	public:
		virtual ~Resource() {};

		static const std::string GetTypeName() { return "Resource"; }

		// Must be implemented by every resource.
		// Loads the resource from a given path.
		virtual void load(const std::string& path) = 0;

		// Not yet implemented.
		// To be done later.
		virtual void save(const std::string& path) {}
	};

}

#pragma once

namespace Dog {

	class FileBrowser
	{
	public:
		FileBrowser();

		std::filesystem::path baseDir;
		std::filesystem::path currentDir;
	};

	void UpdateResourcesWindow(FileBrowser& browser);

}

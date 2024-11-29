#include "Walnut/Application.h"

class ProjectDirectory : public Walnut::Layer
{
public:

	ProjectDirectory(const std::string& path);

	~ProjectDirectory();

	virtual void OnUIRender() override;

	std::string GetSelectedPath() const;

	void ClearSelectedPath();

private:
	void Window();
	void ProjectDirectory::SetCurrentPath(const std::string& path);

private:
	std::string m_currentPath;
	std::string m_selectedPath;
	std::vector<std::pair<std::shared_ptr<Walnut::Image>, std::filesystem::path>> m_images;
	std::vector < std::filesystem::path > m_folders;
	int itemsPerRow = 10;
	std::shared_ptr<Walnut::Image> folderIcon;
};

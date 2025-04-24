#include "Walnut/Application.h"
#include <GLFW/glfw3.h>
#include <filesystem>

class ProjectDirectory : public Walnut::Layer
{
public:

	ProjectDirectory(const std::string& path);

	~ProjectDirectory();

	void OnUIRender() override;

	void OnAttach() override;


	std::string GetSelectedPath() const;

	void ClearSelectedPath();

	int ShowFoldersInDir();

	int ShowImagesInDir(int columnIndex = 0);

	void ShowMapsInDir(int columnIndex = 0);

	std::string getNewScene();


	std::string getMainPath() const;

	static void UploadFile(std::string path);
	void HandleFile(std::filesystem::directory_entry entry);
private:
	void Window();

	void SetCurrentPath(const std::string& path);

private:
	std::string m_mainPath;
	std::string m_currentPath;
	std::string m_selectedPath;

	std::vector<std::pair<std::shared_ptr<Walnut::Image>, std::filesystem::path>> m_images;
	std::vector < std::filesystem::path > m_folders;
	std::vector < std::filesystem::path > m_maps;

	int itemsPerRow = 10;

	std::shared_ptr<Walnut::Image> m_folderIcon;
	std::shared_ptr<Walnut::Image> m_mapIcon;
	std::shared_ptr<Walnut::Image> m_reloadIcon;
	std::shared_ptr<Walnut::Image> m_returnIcon;


	std::string newScene = "";

	static std::string m_copyPaths[30];
	static int m_copySize;
};

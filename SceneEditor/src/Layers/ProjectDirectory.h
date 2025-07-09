#include "Walnut/Application.h"
#include <GLFW/glfw3.h>
#include <filesystem>

#include "InputManager.h"

class ProjectDirectory : public Walnut::Layer
{
public:

	ProjectDirectory(const std::string& path, bool* inputManagerOpen);

	~ProjectDirectory();

	void OnUIRender() override;
	

	void OnAttach() override;


	std::string GetSelectedPath() const;

	void ClearSelectedPath();

	void MoveDir(std::filesystem::path path);

	int ShowFoldersInDir(int columnIndex = 0);

	int ShowImagesInDir(int columnIndex = 0);

	int ShowMapsInDir(int columnIndex = 0);

	int ShowFiles(int columnIndex = 0);

	std::string GetNewScene();


	std::string GetMainPath() const;

	static void UploadFile(const std::string& path);
	
	void HandleFile(const std::filesystem::directory_entry& entry);

	bool* Open() { return &m_isOpen; }
private:
	void Window();

	void SetCurrentPath(const std::string& path);

	static bool TryDeleteEntry(const std::filesystem::path& path);
	void DrawFolderHierarchy(const std::filesystem::path& path, bool released);

	void CreateNewFolder();

private:
	bool* m_inputManagerOpen;

	//relevant paths
	std::string m_mainPath;
	std::string m_currentPath;
	std::string m_selectedPath;

	//items in dir
	std::vector<std::pair<std::shared_ptr<Walnut::Image>, std::filesystem::path>> m_images;
	std::vector < std::filesystem::path > m_folders;
	std::vector < std::filesystem::path > m_maps;

	int itemsPerRow = 10;


	//icons
	std::shared_ptr<Walnut::Image> m_folderIcon;
	std::shared_ptr<Walnut::Image> m_mapIcon;
	std::shared_ptr<Walnut::Image> m_reloadIcon;
	std::shared_ptr<Walnut::Image> m_returnIcon;
	std::shared_ptr<Walnut::Image> m_soundIcon;
	std::shared_ptr<Walnut::Image> m_fontIcon;
	std::shared_ptr<Walnut::Image> m_fileIcon;


	std::string m_newScene;

	static std::string m_copyPaths[30];
	static int m_copySize;

	std::filesystem::path m_selectedItemPath;

	bool m_isOpen = true;

	//rename vars
	std::filesystem::path m_renameTargetPath;
	std::string m_renameBuffer;
	bool m_renamePopupOpen = false;
};

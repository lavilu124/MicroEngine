#include "ProjectSelector.h"
#include "SceneViewer.h"
#include "InputManager.h"
#include "Menu.h"

#include <filesystem>

static std::string GetRecentProjectsPath()
{
    return "recent.txt";
}

static std::vector<std::string> LoadRecentProjects()
{
    std::vector<std::string> paths;
    std::ifstream in(GetRecentProjectsPath());
    std::string line;
    while (std::getline(in, line))
    {
        if (!line.empty() && std::filesystem::exists(line))
            paths.push_back(line);
    }
    return paths;
}

ProjectSelector::ProjectSelector(Walnut::Application* app) : m_app(app)
{
    m_recentProjects = LoadRecentProjects();
}

static bool CopyDirectory(const std::filesystem::path& source, const std::filesystem::path& destination)
{
    try
    {
        if (!std::filesystem::exists(source) || !std::filesystem::is_directory(source))
            return false;

        std::filesystem::create_directories(destination);

        for (const auto& entry : std::filesystem::recursive_directory_iterator(source))
        {
            const auto& path = entry.path();
            auto relativePath = std::filesystem::relative(path, source);
            auto targetPath = destination / relativePath;

            if (entry.is_directory())
                std::filesystem::create_directories(targetPath);
            else if (entry.is_regular_file())
                std::filesystem::copy_file(path, targetPath, std::filesystem::copy_options::overwrite_existing);
        }
        return true;
    }
    catch (...) { return false; }
}

static bool CreateNewProjectStructure(const std::filesystem::path& basePath, const std::string& projectName)
{
    try
    {
        std::filesystem::path projectRoot = basePath / projectName;
        if (std::filesystem::exists(projectRoot))
            return false;

        std::filesystem::path templatePath = "templates/setupProject";
        std::filesystem::path enginePath = "../MicroEngine";
        std::filesystem::path depPath = "../dependencies";

        if (!CopyDirectory(templatePath, projectRoot)) return false;
        if (!CopyDirectory(enginePath, projectRoot / "MicroEngine")) return false;
        if (!CopyDirectory(depPath, projectRoot / "dependencies")) return false;


        /*std::filesystem::path setupScript = projectRoot / "Scripts" / "Setup-Windows.bat";
        if (!std::filesystem::exists(setupScript)) return false;

        int result = system(setupScript.string().c_str());
        return result == 0;*/
        return true;
    }
    catch (...) { return false; }
}

static void RemoveRecentProject(const std::string& path)
{
    std::vector<std::string> recent = LoadRecentProjects();
    recent.erase(std::remove(recent.begin(), recent.end(), path), recent.end());
    std::ofstream out(GetRecentProjectsPath(), std::ios::trunc);
    for (const auto& p : recent)
        out << p << '\n';
}

static void OnFileDrop(GLFWwindow* window, int count, const char** paths)
{
    for (int i = 0; i < count; i++)
    {
        ProjectDirectory::UploadFile(paths[i]);
    }
}

static void SaveRecentProject(const std::string& path)
{
    std::vector<std::string> recent = LoadRecentProjects();
    recent.erase(std::remove(recent.begin(), recent.end(), path), recent.end());
    recent.insert(recent.begin(), path);
    if (recent.size() > 10)
        recent.resize(10);
    std::ofstream out(GetRecentProjectsPath(), std::ios::trunc);
    for (const auto& p : recent)
        out << p << '\n';
}


static void OpenPoroject(const std::string& path, const std::vector<std::shared_ptr<Walnut::Layer>>& layers);

static void InitMainLayers(Walnut::Application* app, const std::string& path)
{
    std::shared_ptr<ProjectDirectory> dir = std::make_shared<ProjectDirectory>(path + "\\Resources");
    app->PushLayer(dir);

    glfwSetDropCallback(static_cast<GLFWwindow*>(app->GetWindowHandle()), OnFileDrop);

    std::shared_ptr<ObjectViewer> viewer = std::make_shared<ObjectViewer>(dir);
    app->PushLayer(viewer);

    std::shared_ptr<SceneContent> sceneContent = std::make_shared<SceneContent>(viewer, dir);
    app->PushLayer(sceneContent);

    std::shared_ptr<SceneViewer> sceneViewer = std::make_shared<SceneViewer>(sceneContent, (path + "\\Resources").c_str());
    app->PushLayer(sceneViewer);

    std::shared_ptr<Menu> menu = std::make_shared<Menu>(path, viewer);
    app->PushLayer(menu);

    std::shared_ptr<InputManager> inputM = std::make_shared<InputManager>(path + "\\Resources\\settings\\input.cfg");
    app->PushLayer(inputM);

    std::weak_ptr<Menu> weakMenu = menu;
	std::weak_ptr<SceneContent> weakSceneContent = sceneContent;
	std::weak_ptr<InputManager> weakInputM = inputM;
	std::weak_ptr<ProjectDirectory> weakDir = dir;
	std::weak_ptr<ObjectViewer> weakViewer = viewer;
	std::weak_ptr<SceneViewer> weakSceneViewer = sceneViewer;
    app->SetMenubarCallback([app, weakMenu, weakSceneContent, weakInputM, weakDir, weakViewer, weakSceneViewer]()
        {
            auto dir = weakDir.lock();
            auto viewer = weakViewer.lock();
            auto sceneViewer = weakSceneViewer.lock();
            auto inputM = weakInputM.lock();
            auto sceneContent = weakSceneContent.lock();

            static bool openCreatePorject = false;


            bool newPorject = false;
            if (ImGui::BeginMenu("file"))
            {
	            if (ImGui::MenuItem("New Project"))
	            {
                  openCreatePorject = true;
	            }
                if (ImGui::MenuItem("Open Project"))
                {
                    const char* selectedFolder = tinyfd_selectFolderDialog("Select Project Folder", "");
                    if (selectedFolder)
                    {
                        std::string path = selectedFolder;
                        if (std::filesystem::exists(path + "\\Resources"))
                        {
                            OpenPoroject(path, {dir, viewer, sceneViewer, inputM, sceneContent});
                            newPorject = true;
                        }
                        else
                        {
                            ImGui::OpenPopup("Wrong File");
                        }
                    }
                }
                if (ImGui::MenuItem("Exit"))
                {
                    app->Close();
                }

                ImGui::EndMenu();
            }
            if (newPorject) return;
			if (openCreatePorject)
                ImGui::OpenPopup("Create New Project");

            if (ImGui::BeginPopupModal("Create New Project", NULL, ImGuiWindowFlags_NoResize))
            {

                static char basePathBuffer[512] = {};
				static char projectNameBuffer[256] = {};
                std::string errorMsg;

                std::string createProjectPath;
				std::string createProjectName;

                ImGui::Text("Select base folder:");
                ImGui::SameLine();
                if (ImGui::Button("Browse..."))
                {
                    const char* folder = tinyfd_selectFolderDialog("Select Base Folder", "");
                    if (folder)
                    {
                        strncpy(basePathBuffer, folder, sizeof(basePathBuffer) - 1);
                        createProjectPath = folder;
                    }
                }
                ImGui::InputText("Base Path", basePathBuffer, sizeof(basePathBuffer));
                ImGui::InputText("Project Name", projectNameBuffer, sizeof(projectNameBuffer));

                if (ImGui::Button("Create"))
                {
                    createProjectPath = basePathBuffer;
                    createProjectName = projectNameBuffer;

                    if (!createProjectPath.empty() && !createProjectName.empty())
                    {
                        bool success = CreateNewProjectStructure(createProjectPath, createProjectName);
                        if (success)
                        {
							OpenPoroject(createProjectPath + "\\" + createProjectName, { dir, viewer, sceneViewer, inputM, sceneContent });
							SaveRecentProject(createProjectPath + "\\" + createProjectName);
                            ImGui::CloseCurrentPopup();
                            openCreatePorject = false;
                            newPorject = true;
                        }
                    	else errorMsg = "Failed to create project (folder may already exist).";
                    }
                    else errorMsg = "Please specify both base path and project name.";
                }

                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    ImGui::CloseCurrentPopup();
                    errorMsg.clear();
                    openCreatePorject = false;
                }

                if (!errorMsg.empty())
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", errorMsg.c_str());

                ImGui::EndPopup();

                if (!ImGui::IsPopupOpen("Create New Project"))
                    openCreatePorject = false;
            }
            if (newPorject) return;

    		
            if (ImGui::BeginMenu("View"))
            {
                /*ImGui::MenuItem("Content Browser", nullptr, &m_showContentBrowser);
                ImGui::MenuItem("Scene Hierarchy", nullptr, &m_showSceneHierarchy);*/
                ImGui::MenuItem("Scene Content", nullptr, sceneContent->Open());
                ImGui::MenuItem("Input Manager", nullptr, inputM->Open());
                ImGui::MenuItem("Project Directory", nullptr, dir->Open());
                ImGui::MenuItem("Object Viewer", nullptr, viewer->Open());
                ImGui::MenuItem("Scene Viewer", nullptr, sceneViewer->Open());
                ImGui::EndMenu();
            }

            auto menu = weakMenu.lock();
            if (ImGui::BeginMenu("Custom Object") && menu)
            {
                if (ImGui::MenuItem("Create Object"))
                {
                    menu->NewObjectPressed();
                }

                ImGui::EndMenu();
            }
			
        });

}

static void OpenPoroject(const std::string& path, const std::vector<std::shared_ptr<Walnut::Layer>>& layers)
{
    if (!std::filesystem::exists(path + "\\Resources"))
        return;


    for (auto& layer : layers)
    {
        layer->OnDetach();
        Walnut::Application::Get().PopLayer(layer);
    }

    InitMainLayers(&Walnut::Application::Get(), path);

    // Resize and center window
    GLFWwindow* window = static_cast<GLFWwindow*>(Walnut::Application::Get().GetWindowHandle());
    int newWidth = 1280, newHeight = 720;
    glfwSetWindowSize(window, newWidth, newHeight);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (mode)
    {
        int xpos = (mode->width - newWidth) / 2;
        int ypos = (mode->height - newHeight) / 2;
        glfwSetWindowPos(window, xpos, ypos);
    }
}

void ProjectSelector::OnUIRender()
{
    bool openConfirmDeletePopup = false;

    if (m_state == AppState::ProjectSelection)
    {

        ImGui::SetNextWindowSize(ImVec2(SELECTOR_WIDTH, SELECTOR_HEIGHT), ImGuiCond_FirstUseEver);
        ImGui::Begin("Select Project", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar  | ImGuiWindowFlags_NoResize);


        ImGui::Text("Choose a project directory:");
        ImGui::Separator();

        if (ImGui::Button("Open Project..."))
        {
            const char* selectedFolder = tinyfd_selectFolderDialog("Select Project Folder", "");
            if (selectedFolder)
            {
                if (std::filesystem::exists(std::string(selectedFolder) + "\\Resources"))
                {
                    m_projectPath = selectedFolder;
                    m_needsResize = true;
                }
                else
                {
                    ImGui::OpenPopup("Wrong File");
                }
            }
        }

        if (ImGui::BeginPopupModal("Wrong File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Invalid folder chosen.");
            if (ImGui::Button("OK"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }


        ImGui::SameLine();
        if (ImGui::Button("Create New Project"))
        {
            CreatePorject();
        }

        ImGui::Separator();
        ImGui::Text("Recent Projects:");

        for (size_t i = 0; i < m_recentProjects.size(); ++i)
        {
            const std::string& path = m_recentProjects[i];
            ImGui::PushID(static_cast<int>(i));
            if (ImGui::Button(path.c_str()))
            {
                m_projectPath = path;
                m_needsResize = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("X"))
            {
                m_projectToDeleteIndex = static_cast<int>(i);
                m_projectToDeletePath = path;
                openConfirmDeletePopup = true;
            }
            ImGui::PopID();
        }

        if (!m_projectPath.empty())
            ImGui::Text("Selected: %s", m_projectPath.c_str());

        if (openConfirmDeletePopup)
            ImGui::OpenPopup("Confirm Delete");

        if (ImGui::BeginPopupModal("Create New Project", NULL, ImGuiWindowFlags_NoResize))
        {
            ImGui::Text("Select base folder:");
            ImGui::SameLine();
            if (ImGui::Button("Browse..."))
            {
                const char* folder = tinyfd_selectFolderDialog("Select Base Folder", "");
                if (folder)
                {
                    strncpy(m_basePathBuffer, folder, sizeof(m_basePathBuffer) - 1);
                    m_createProjectPath = folder;
                }
            }
            ImGui::InputText("Base Path", m_basePathBuffer, sizeof(m_basePathBuffer));
            ImGui::InputText("Project Name", m_projectNameBuffer, sizeof(m_projectNameBuffer));

            if (ImGui::Button("Create"))
            {
                m_createProjectPath = m_basePathBuffer;
                m_createProjectName = m_projectNameBuffer;

                if (!m_createProjectPath.empty() && !m_createProjectName.empty())
                {
                    bool success = CreateNewProjectStructure(m_createProjectPath, m_createProjectName);
                    if (success)
                    {
                        m_projectPath = (std::filesystem::path(m_createProjectPath) / m_createProjectName).string();
                        m_needsResize = true;
                        ImGui::CloseCurrentPopup();
                    }
                    else m_errorMessage = "Failed to create project (folder may already exist).";
                }
                else m_errorMessage = "Please specify both base path and project name.";
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
                m_errorMessage.clear();
            }

            if (!m_errorMessage.empty())
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", m_errorMessage.c_str());

            ImGui::EndPopup();
        }

        if (ImGui::BeginPopupModal("Confirm Delete", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Are you sure you want to delete this project?\n%s", m_projectToDeletePath.c_str());
            ImGui::Separator();

            if (ImGui::Button("Yes"))
            {
                std::filesystem::remove_all(m_projectToDeletePath);
                RemoveRecentProject(m_projectToDeletePath);
                m_recentProjects = LoadRecentProjects();
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();
            if (ImGui::Button("No"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::End();
    }

    if (m_needsResize)
    {
        SaveRecentProject(m_projectPath);
        InitMainLayers(m_app, m_projectPath);
        m_state = AppState::MainApp;
        m_needsResize = false;

        GLFWwindow* window = static_cast<GLFWwindow*>(m_app->GetWindowHandle());
        int newWidth = 1280, newHeight = 720;
        glfwSetWindowSize(window, newWidth, newHeight);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (mode)
        {
            int xpos = (mode->width - newWidth) / 2;
            int ypos = (mode->height - newHeight) / 2;
            glfwSetWindowPos(window, xpos, ypos);
        }
    }
}

void ProjectSelector::CreatePorject()
{
    m_createProjectPath.clear();
    m_createProjectName.clear();
    memset(m_basePathBuffer, 0, sizeof(m_basePathBuffer));
    memset(m_projectNameBuffer, 0, sizeof(m_projectNameBuffer));
    ImGui::OpenPopup("Create New Project");
}

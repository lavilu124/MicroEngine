#include "ProjectSelector.h"
#include "SceneViewer.h"
#include "InputManager.h"
#include "Menu.h"

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

ProjectSelector::ProjectSelector(Walnut::Application* app) : m_App(app)
{
    m_RecentProjects = LoadRecentProjects();
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


        std::filesystem::path setupScript = projectRoot / "Scripts" / "Setup-Windows.bat";
        if (!std::filesystem::exists(setupScript)) return false;

        int result = system(setupScript.string().c_str());
        return result == 0;
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

void OnFileDrop(GLFWwindow* window, int count, const char** paths)
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

void InitMainLayers(Walnut::Application* app, const std::string& path)
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
    app->SetMenubarCallback([app, weakMenu]()
        {
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

void ProjectSelector::OnUIRender()
{
    bool openConfirmDeletePopup = false;

    if (m_State == AppState::ProjectSelection)
    {
        ImGui::SetNextWindowSize(ImVec2(SELECTOR_WIDTH, SELECTOR_HEIGHT), ImGuiCond_Always);
        ImGui::Begin("Select Project", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

        ImGui::Text("Choose a project directory:");
        ImGui::Separator();

        if (ImGui::Button("Open Project..."))
        {
            const char* selectedFolder = tinyfd_selectFolderDialog("Select Project Folder", "");
            if (selectedFolder) {
                m_ProjectPath = selectedFolder;
                m_NeedsResize = true;
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Create New Project"))
        {
            m_CreateProjectPath.clear();
            m_CreateProjectName.clear();
            memset(m_BasePathBuffer, 0, sizeof(m_BasePathBuffer));
            memset(m_ProjectNameBuffer, 0, sizeof(m_ProjectNameBuffer));
            ImGui::OpenPopup("Create New Project");
        }

        ImGui::Separator();
        ImGui::Text("Recent Projects:");

        for (size_t i = 0; i < m_RecentProjects.size(); ++i)
        {
            const std::string& path = m_RecentProjects[i];
            ImGui::PushID(static_cast<int>(i));
            if (ImGui::Button(path.c_str()))
            {
                m_ProjectPath = path;
                m_NeedsResize = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("X"))
            {
                m_ProjectToDeleteIndex = static_cast<int>(i);
                m_ProjectToDeletePath = path;
                openConfirmDeletePopup = true;
            }
            ImGui::PopID();
        }

        if (!m_ProjectPath.empty())
            ImGui::Text("Selected: %s", m_ProjectPath.c_str());

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
                    strncpy(m_BasePathBuffer, folder, sizeof(m_BasePathBuffer) - 1);
                    m_CreateProjectPath = folder;
                }
            }
            ImGui::InputText("Base Path", m_BasePathBuffer, sizeof(m_BasePathBuffer));
            ImGui::InputText("Project Name", m_ProjectNameBuffer, sizeof(m_ProjectNameBuffer));

            if (ImGui::Button("Create"))
            {
                m_CreateProjectPath = m_BasePathBuffer;
                m_CreateProjectName = m_ProjectNameBuffer;

                if (!m_CreateProjectPath.empty() && !m_CreateProjectName.empty())
                {
                    bool success = CreateNewProjectStructure(m_CreateProjectPath, m_CreateProjectName);
                    if (success)
                    {
                        m_ProjectPath = (std::filesystem::path(m_CreateProjectPath) / m_CreateProjectName).string();
                        m_NeedsResize = true;
                        ImGui::CloseCurrentPopup();
                    }
                    else m_ErrorMessage = "Failed to create project (folder may already exist).";
                }
                else m_ErrorMessage = "Please specify both base path and project name.";
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
                m_ErrorMessage.clear();
            }

            if (!m_ErrorMessage.empty())
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", m_ErrorMessage.c_str());

            ImGui::EndPopup();
        }

        if (ImGui::BeginPopupModal("Confirm Delete", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Are you sure you want to delete this project?\n%s", m_ProjectToDeletePath.c_str());
            ImGui::Separator();

            if (ImGui::Button("Yes"))
            {
                std::filesystem::remove_all(m_ProjectToDeletePath);
                RemoveRecentProject(m_ProjectToDeletePath);
                m_RecentProjects = LoadRecentProjects();
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

    if (m_NeedsResize)
    {
        SaveRecentProject(m_ProjectPath);
        InitMainLayers(m_App, m_ProjectPath);
        m_State = AppState::MainApp;
        m_NeedsResize = false;

        GLFWwindow* window = static_cast<GLFWwindow*>(m_App->GetWindowHandle());
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

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/UI/UI.h"

#include "Layers/SceneViewer.h"
#include "Layers/Saves.h"

#include <filesystem>
#include <GLFW/glfw3.h>
#include <fstream>
#include <vector>
#include <algorithm>

#include <iostream>

#define TINYFILEDIALOGS_IMPLEMENTATION
#include "fileManage/tinyfiledialogs.h"

#define SELECTOR_HEIGHT 600
#define SELECTOR_WIDTH 800

// Forward declare
void InitMainLayers(Walnut::Application* app, const std::string& projectPath);

void OnFileDrop(GLFWwindow* window, int count, const char** paths)
{
    for (int i = 0; i < count; i++)
    {
        ProjectDirectory::UploadFile(paths[i]);
    }
}

enum class AppState
{
    ProjectSelection,
    MainApp
};

// Recent projects utility functions
static std::string GetRecentProjectsPath()
{
    return "recent.txt"; // Could be improved to use platform-specific app data folder
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

static void SaveRecentProject(const std::string& path)
{
    std::vector<std::string> recent = LoadRecentProjects();

    // Remove the path if it already exists to avoid duplicates
    recent.erase(std::remove(recent.begin(), recent.end(), path), recent.end());

    // Add to front
    recent.insert(recent.begin(), path);

    // Limit to max 10 entries
    if (recent.size() > 10)
        recent.resize(10);

    std::ofstream out(GetRecentProjectsPath(), std::ios::trunc);
    for (const auto& p : recent)
        out << p << '\n';
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
            {
                std::filesystem::create_directories(targetPath);
            }
            else if (entry.is_regular_file())
            {
                std::filesystem::copy_file(path, targetPath, std::filesystem::copy_options::overwrite_existing);
            }
        }
        return true;
    }
    catch (...)
    {
        return false;
    }
}

static bool CreateNewProjectStructure(const std::filesystem::path& basePath, const std::string& projectName)
{
    try
    {
        std::filesystem::path projectRoot = basePath / projectName;

        if (std::filesystem::exists(projectRoot))
            return false; // Project already exists

        // Define template paths (you may want to make these configurable)
        std::filesystem::path templatePath = "setupProject";
        std::filesystem::path enginePath = "../MicroEngine";
        std::filesystem::path depPath = "../dependencies";

        // Copy setupProject contents
        if (!CopyDirectory(templatePath, projectRoot))
            return false;

        // Copy MicroEngine folder inside new project
        if (!CopyDirectory(enginePath, projectRoot / "MicroEngine"))
            return false;

        if (!CopyDirectory(depPath, projectRoot / "dependencies"))
            return false;

        return true;
    }
    catch (...)
    {
        return false;
    }
}

class ProjectSelector : public Walnut::Layer
{
public:
    ProjectSelector(Walnut::Application* app)
        : m_App(app)
    {
        m_RecentProjects = LoadRecentProjects();
    }

    virtual void OnUIRender() override
    {
        if (m_State == AppState::ProjectSelection)
        {
            ImGui::SetNextWindowSize(ImVec2(SELECTOR_WIDTH, SELECTOR_HEIGHT), ImGuiCond_Always);
            ImGui::Begin("Select Project", nullptr,
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoScrollbar);

            ImGui::Text("Choose a project directory:");
            ImGui::Separator();

            if (ImGui::Button("Open Project..."))
            {
                const char* selectedFolder = tinyfd_selectFolderDialog("Select Project Folder", "");
                if (selectedFolder)
                {
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

            for (const auto& path : m_RecentProjects)
            {
                if (ImGui::Button(path.c_str()))
                {
                    m_ProjectPath = path;
                    m_NeedsResize = true;
                }
            }

            if (!m_ProjectPath.empty())
                ImGui::Text("Selected: %s", m_ProjectPath.c_str());

            


            ImGui::SetNextWindowSize(ImVec2(400, 160), ImGuiCond_Appearing);
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
                    m_CreateProjectPath = std::string(m_BasePathBuffer);
                    m_CreateProjectName = std::string(m_ProjectNameBuffer);

                    if (!m_CreateProjectPath.empty() && !m_CreateProjectName.empty())
                    {

                        bool success = CreateNewProjectStructure(m_CreateProjectPath, m_CreateProjectName);
                        if (success)
                        {
                            m_ProjectPath = (std::filesystem::path(m_CreateProjectPath) / m_CreateProjectName).string();
                            m_NeedsResize = true;
                            ImGui::CloseCurrentPopup();
                        }
                        else
                        {
                            m_ErrorMessage = "Failed to create project (folder may already exist).";
                        }
                    }
                    else
                    {
                        m_ErrorMessage = "Please specify both base path and project name.";
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    ImGui::CloseCurrentPopup();
                    m_ErrorMessage.clear();
                }

                if (!m_ErrorMessage.empty())
                {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", m_ErrorMessage.c_str());
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

private:
    AppState m_State = AppState::ProjectSelection;
    std::string m_ProjectPath;
    Walnut::Application* m_App = nullptr;
    bool m_NeedsResize = false;
    std::vector<std::string> m_RecentProjects;

    std::string m_CreateProjectPath;
    std::string m_CreateProjectName;
    std::string m_ErrorMessage;

    char m_BasePathBuffer[512] = {};
    char m_ProjectNameBuffer[256] = {};
};

// Function to initialize actual app layers
void InitMainLayers(Walnut::Application* app, const std::string& path)
{
    std::shared_ptr<ProjectDirectory> dir = std::make_shared<ProjectDirectory>(path);
    app->PushLayer(dir);

    glfwSetDropCallback(static_cast<GLFWwindow*>(app->GetWindowHandle()), OnFileDrop);

    std::shared_ptr<ObjectViewer> viewer = std::make_shared<ObjectViewer>(dir);
    app->PushLayer(viewer);

    std::shared_ptr<SceneContent> sceneContent = std::make_shared<SceneContent>(viewer, dir);
    app->PushLayer(sceneContent);

    std::shared_ptr<SceneViewer> sceneViewer = std::make_shared<SceneViewer>(sceneContent, path.c_str());
    app->PushLayer(sceneViewer);
}

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
    Walnut::ApplicationSpecification spec;
    spec.Name = "Micro Engine";
    spec.CustomTitlebar = true;

    Walnut::Application* app = new Walnut::Application(spec);

    app->PushLayer(std::make_shared<ProjectSelector>(app));

    return app;
}

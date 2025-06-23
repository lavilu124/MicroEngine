#pragma once
#include <filesystem>
#include <GLFW/glfw3.h>
#include <fstream>
#include <vector>
#include <algorithm>



#define TINYFILEDIALOGS_IMPLEMENTATION
#include "../fileManage/tinyfiledialogs.h"
#include "Walnut/Application.h"

#define SELECTOR_HEIGHT 600
#define SELECTOR_WIDTH 800

enum class AppState
{
    ProjectSelection,
    MainApp
};

class ProjectSelector : public Walnut::Layer
{
public:
    ProjectSelector(Walnut::Application* app);


    virtual void OnUIRender() override;

private:
    AppState m_state = AppState::ProjectSelection;
    std::string m_projectPath;
    Walnut::Application* m_app = nullptr;
    bool m_needsResize = false;
    std::vector<std::string> m_recentProjects;

    std::string m_createProjectPath;
    std::string m_createProjectName;
    std::string m_errorMessage;

    std::string m_projectToDeletePath;
    int m_projectToDeleteIndex = -1;

    char m_basePathBuffer[512] = {};
    char m_projectNameBuffer[256] = {};
};


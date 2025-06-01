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
    AppState m_State = AppState::ProjectSelection;
    std::string m_ProjectPath;
    Walnut::Application* m_App = nullptr;
    bool m_NeedsResize = false;
    std::vector<std::string> m_RecentProjects;

    std::string m_CreateProjectPath;
    std::string m_CreateProjectName;
    std::string m_ErrorMessage;

    std::string m_ProjectToDeletePath;
    int m_ProjectToDeleteIndex = -1;

    char m_BasePathBuffer[512] = {};
    char m_ProjectNameBuffer[256] = {};
};


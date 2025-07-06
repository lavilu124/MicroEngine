#include "Console.h"
#include <filesystem>
#include <fstream>
#include <regex>

namespace fs = std::filesystem;

Console::Console(const std::shared_ptr<SceneViewer>& sceneViewer)
    : m_sceneViewer(sceneViewer)
{
}

void Console::OnUIRender()
{
    if (!m_isOpen)
        return;

    if (m_sceneViewer->DoneRun() && !m_logLoaded)
    {
        ReadConsole();
        m_logLoaded = true;
    }

    ImGui::Begin("Console", &m_isOpen,  ImGuiWindowFlags_NoCollapse);

    for (const auto& line : m_logLines)
    {
        ImGui::TextUnformatted(line.c_str());
    }

    ImGui::End();
}

void Console::ReadConsole()
{
    std::string basePath = m_sceneViewer->sceneContent->GetDir()->GetMainPath();
    fs::path logsDir = fs::path(basePath) / ".." / "Logs";

    int maxLogNumber = -1;
    fs::path maxLogPath;

    std::regex logPattern(R"(log(\d+)\.log)");

    for (const auto& entry : fs::directory_iterator(logsDir))
    {
        if (!entry.is_regular_file()) continue;

        std::smatch match;
        std::string filename = entry.path().filename().string();

        if (std::regex_match(filename, match, logPattern))
        {
            int num = std::stoi(match[1].str());
            if (num > maxLogNumber)
            {
                maxLogNumber = num;
                maxLogPath = entry.path();
            }
        }
    }

    if (maxLogNumber == -1)
        return; // No log file found

    std::ifstream file(maxLogPath);
    std::string line;
    m_logLines.clear();

    while (std::getline(file, line))
    {
        m_logLines.push_back(line);
    }
}

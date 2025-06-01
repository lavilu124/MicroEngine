#include "Menu.h"
#include <fstream>

Menu::Menu(std::string ProjectRootPath) : m_rootPath(ProjectRootPath)
{
    memset(m_ClassNameBuffer, 0, sizeof(m_ClassNameBuffer));
}

void Menu::NewObjectPressed()
{
	m_newObjectOpen = true;
}

void Menu::OnUIRender()
{
	if (m_newObjectOpen) {
		NewObjectWindow();
	}
}

void Menu::NewObjectWindow()
{
	ImGui::OpenPopup("Create New Class");

    if (ImGui::BeginPopupModal("Create New Class", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Enter class name:");
        ImGui::InputText("##", m_ClassNameBuffer, sizeof(m_ClassNameBuffer));

        if (ImGui::Button("Create"))
        {
            std::string className = m_ClassNameBuffer;
            if (!className.empty())
            {
                std::filesystem::path currDir = std::filesystem::current_path().string();
                std::filesystem::path headerSrc = currDir / "templates" / "Template.h";
                std::filesystem::path cppSrc = currDir / "templates" / "Template.cpp";

                std::filesystem::path projDir = m_rootPath;
                std::filesystem::path headerDest = projDir / "Game" / "src" / "Objects" / (className + ".h");
                std::filesystem::path cppDest = projDir / "Game" / "src" / "Objects" / (className + ".cpp");

                try
                {
                    std::filesystem::copy_file(headerSrc, headerDest, std::filesystem::copy_options::overwrite_existing);
                    std::filesystem::copy_file(cppSrc, cppDest, std::filesystem::copy_options::overwrite_existing);

                    auto replaceInFile = [&](const std::filesystem::path& filePath)
                        {
                            std::ifstream in(filePath);
                            std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
                            in.close();

                            size_t pos;
                            while ((pos = content.find("Template")) != std::string::npos)
                                content.replace(pos, 8, className);

                            std::ofstream out(filePath);
                            out << content;
                            out.close();
                        };

                    replaceInFile(headerDest);
                    replaceInFile(cppDest);
                }
                catch (const std::exception& e)
                {
                    
                }

                m_newObjectOpen = false;
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            m_newObjectOpen = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

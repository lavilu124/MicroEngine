#include "Menu.h"
#include <fstream>
#include <iostream>

Menu::Menu(const std::string& projectRootPath, std::shared_ptr<ObjectViewer> objectViewer) : m_rootPath(projectRootPath), m_objectViewer(objectViewer)
{
    memset(m_ClassNameBuffer, 0, sizeof(m_ClassNameBuffer));
}

void Menu::NewObjectPressed()
{
	m_newObjectOpen = true;
}

void Menu::DeleteObjectPressed(const char* name)
{
	m_deleteObjectName = name;
	m_deleteObjectOpen = true;
}

void Menu::OnUIRender()
{
	if (m_newObjectOpen) {
		NewObjectWindow();
	}
    if (m_deleteObjectOpen)
    {
        DeleteObjectWindow();
    }
}

void Menu::OnAttach()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.FramePadding = ImVec2(4.0f, 2.0f);   
    style.ItemSpacing = ImVec2(8.0f, 4.0f);    
    style.WindowPadding = ImVec2(8.0f, 4.0f);
    style.WindowPadding = ImVec2(8, 8);
    style.ChildBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(8, 6);
    style.IndentSpacing = 16.0f;
    style.FrameRounding = 4.0f;
    style.ChildRounding = 4.0f;
    style.GrabRounding = 4.0f;

    //// Colors for a clean modern look
    ImVec4* colors = style.Colors;
    //colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    //colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    //colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    //colors[ImGuiCol_Header] = ImVec4(0.20f, 0.22f, 0.25f, 1.00f);
    //colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.30f, 0.35f, 1.00f);
    //colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.40f, 0.45f, 1.00f);

    style.Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.22f, 0.25f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.40f, 0.45f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);

    colors[ImGuiCol_TabActive] = ImVec4(0.26f, 0.59f, 0.98f, .20f); // Unity blue

    // ?? Inactive tab (not focused but visible)
    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, .10f);

    // ?? Hovered tab
    colors[ImGuiCol_TabHovered] = ImVec4(0.30f, 0.60f, 1.00f, 1.00f);

    // ?? Unfocused but selected tab
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.30f);
}

bool Menu::ObjectExist(const std::string& name) const{
    std::filesystem::path projDir = m_rootPath;
    std::ifstream classes(projDir / "projectData.txt");
    std::string content((std::istreambuf_iterator<char>(classes)), std::istreambuf_iterator<char>());
    classes.close();

    return content.find(name) != std::string::npos;
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
            if (!className.empty() && !ObjectExist(className))
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

                    std::ofstream classes(projDir / "projectData.txt", std::ios::app);
                    classes << className << "\n";
                    classes.close();

                    m_objectViewer->NewObject();
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

void Menu::DeleteObjectWindow()
{
    ImGui::OpenPopup("Delete Class");

    if (ImGui::BeginPopupModal("Delete Class", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
		ImGui::Text("Are you sure you want to delete the class '%s'?", m_deleteObjectName);
        if (ImGui::Button("Yes"))
        {
            std::filesystem::path projDir = m_rootPath;
            std::filesystem::path headerDest = projDir / "Game" / "src" / "Objects" / (std::string(m_deleteObjectName) + ".h");
            std::filesystem::path cppDest = projDir / "Game" / "src" / "Objects" / (std::string(m_deleteObjectName) + ".cpp");

            try
            {
                if (std::filesystem::exists(headerDest))
					std::filesystem::remove(headerDest);
				if (std::filesystem::exists(cppDest))
                    std::filesystem::remove(cppDest);

				std::ifstream classes(projDir / "projectData.txt");
                std::string content((std::istreambuf_iterator<char>(classes)), std::istreambuf_iterator<char>());
				classes.close();
				size_t pos = content.find(m_deleteObjectName);
				if (pos != std::string::npos)
				{
					content.erase(pos, std::string(m_deleteObjectName).length() + 1);
					std::ofstream out(projDir / "projectData.txt");
				}

				m_objectViewer->DeletedObjectType();
                m_deleteObjectOpen = false;
            }
            catch (const std::exception& e)
            {

            }

        }
        ImGui::SameLine();
        if (ImGui::Button("no"))
        {
            m_deleteObjectOpen = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

#include "ProjectDirectory.h"
#include <filesystem>

ProjectDirectory::ProjectDirectory(const std::string& path)
{
    m_currentPath = path;
    
}

void ProjectDirectory::OnAttach()
{
    SetCurrentPath(m_currentPath);
    folderIcon = std::make_shared<Walnut::Image>("appGui\\folderIcon.png");
    mapIcon = std::make_shared<Walnut::Image>("appGui\\mapIcon.png");
}

ProjectDirectory::~ProjectDirectory()
{
	m_images.clear();
}

void ProjectDirectory::OnUIRender()
{
	Window();
}

void ProjectDirectory::SetCurrentPath(const std::string& path)
{
    m_currentPath = path;

	m_images.clear();
    m_folders.clear();
    m_maps.clear();


    for (const auto& entry : std::filesystem::directory_iterator(m_currentPath)) {
        if (entry.is_directory())
        {
            m_folders.push_back(entry);
        }
        else if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg" || entry.path().extension() == ".jpeg") {
            try {
                m_images.push_back({ std::make_shared<Walnut::Image>(entry.path().string()), entry.path() });
            }
            catch (const std::exception& e) {
                // Handle image loading failure
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to load: %s", entry.path().filename().c_str());
            }
        }
        else if (entry.path().extension() == ".McScene") {
            m_maps.push_back(entry);
        }
    }
}

int ProjectDirectory::ShowFoldersInDir()
{
    int columnIndex = 0;
    const float spacing = 10.0f;
    const float boxPadding = 5.0f;
    float windowWidth = ImGui::GetContentRegionAvail().x;
    const float thumbnailSize = 100.0f;
    const int imagesPerRow = (int)(windowWidth / (thumbnailSize + 2 * boxPadding + spacing));

    for (int i = 0; i < m_folders.size(); i++) {
        const auto& folderPath = m_folders[i];
        if (columnIndex >= imagesPerRow) {
            columnIndex = 0;
            ImGui::NewLine();
        }

        ImGui::BeginChild(folderPath.filename().string().c_str(), ImVec2(thumbnailSize + 2 * boxPadding, thumbnailSize + 2 * boxPadding + ImGui::GetTextLineHeightWithSpacing()));
        {
            ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            ImGui::SetCursorPosX(boxPadding);

            // Render folder icon as a button
            ImGui::Image(folderIcon->GetDescriptorSet(), ImVec2(thumbnailSize, thumbnailSize / 1.25));

            if (ImGui::IsItemHovered()) {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
            if (ImGui::IsItemClicked())
            {
                SetCurrentPath(m_currentPath + "\\" + folderPath.filename().string().c_str());
            }

            // Render folder name below the icon
            ImGui::SetCursorPosX(boxPadding);
            ImGui::TextWrapped(folderPath.filename().string().c_str());

            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }
        ImGui::EndChild(); //check
        ImGui::SameLine();
        columnIndex++;
    }
}

int ProjectDirectory::ShowImagesInDir(int columnIndex)
{
    const float spacing = 10.0f;
    const float boxPadding = 5.0f;
    float windowWidth = ImGui::GetContentRegionAvail().x;
    const float thumbnailSize = 100.0f;
    const int imagesPerRow = (int)(windowWidth / (thumbnailSize + 2 * boxPadding + spacing));

    for (auto& [image, imagePath] : m_images) {
        if (columnIndex >= imagesPerRow) {
            columnIndex = 0;
            ImGui::NewLine();
        }

        if (image) {
            ImGui::BeginChild(imagePath.filename().string().c_str(), ImVec2(thumbnailSize + 2 * boxPadding, thumbnailSize + 2 * boxPadding + ImGui::GetTextLineHeightWithSpacing()));
            {
                ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                ImGui::SetCursorPosX(boxPadding);
                ImGui::Image(image->GetDescriptorSet(), ImVec2(thumbnailSize, thumbnailSize));
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("%s", imagePath.filename().string().c_str());
                    ImGui::EndTooltip();
                }
                if (ImGui::IsItemClicked()) {
                    m_selectedPath = imagePath.string();
                }
                ImGui::PopStyleVar();
                ImGui::PopStyleColor();
            }
            ImGui::EndChild();
            ImGui::SameLine();
            columnIndex++;
        }
    }

    return columnIndex;
}

void ProjectDirectory::ShowMapsInDir(int columnIndex)
{
    const float spacing = 10.0f;
    const float boxPadding = 5.0f;
    float windowWidth = ImGui::GetContentRegionAvail().x;
    const float thumbnailSize = 100.0f;
    const int imagesPerRow = (int)(windowWidth / (thumbnailSize + 2 * boxPadding + spacing));

    for (int i = 0; i < m_maps.size(); i++) {
        const auto& mapPath = m_maps[i];
        if (columnIndex >= imagesPerRow) {
            columnIndex = 0;
            ImGui::NewLine();
        }

        ImGui::BeginChild(mapPath.filename().string().c_str(), ImVec2(thumbnailSize + 2 * boxPadding, thumbnailSize + 2 * boxPadding + ImGui::GetTextLineHeightWithSpacing()));
        {
            ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            ImGui::SetCursorPosX(boxPadding);

            // Render folder icon as a button
            ImGui::Image(mapIcon->GetDescriptorSet(), ImVec2(thumbnailSize, thumbnailSize / 1.25));

            // Render folder name below the icon
            ImGui::SetCursorPosX(boxPadding);

            if (ImGui::IsItemHovered()) {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            }
            if (ImGui::IsItemClicked())
            {
                newScene = mapPath.string();
            }

            std::string fileName = mapPath.filename().string();
            fileName.erase(fileName.size() - 8);
            ImGui::TextWrapped(fileName.c_str());

            

            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }
        ImGui::EndChild(); //check
        ImGui::SameLine();
        columnIndex++;
    }
}

std::string ProjectDirectory::getNewScene()
{
    std::string returnFile = newScene;
    newScene = "";
    return returnFile;
}

void ProjectDirectory::Window()
{
    ImGui::Begin("Project Directory");
	if (ImGui::Button("return"))
	{
		SetCurrentPath(m_currentPath.substr(0, m_currentPath.find_last_of('\\')));
	}
    ImGui::SameLine();
    ImGui::Text("Current Directory:");
    ImGui::SameLine();
    ImGui::TextWrapped(m_currentPath.c_str());

    if (m_selectedPath != "") {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    
    int columnIndex = ShowFoldersInDir();
    columnIndex = ShowImagesInDir(columnIndex);
    ShowMapsInDir(columnIndex);

    ImGui::End();
}

std::string ProjectDirectory::GetSelectedPath() const
{
	return m_selectedPath;
}

void ProjectDirectory::ClearSelectedPath()
{
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
		m_selectedPath = "";
        ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	}
}



#include "ProjectDirectory.h"

ProjectDirectory::ProjectDirectory(const std::string& path)
{
    SetCurrentPath(path);
    folderIcon = std::make_shared<Walnut::Image>("C:/github/MicroEngine/SceneEditor/folderIcon.png");
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
    for (const auto& entry : std::filesystem::directory_iterator(m_currentPath)) {
        if (entry.is_directory())
        {
            m_folders.push_back(entry);
        }
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg" || entry.path().extension() == ".jpeg") {
            try {
                m_images.push_back({ std::make_shared<Walnut::Image>(entry.path().string()), entry.path() });
            }
            catch (const std::exception& e) {
                // Handle image loading failure
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to load: %s", entry.path().filename().c_str());
            }
        }
    }
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

    if (true) {
        int columnIndex = 0;
        const float thumbnailSize = 100.0f;
        const float spacing = 10.0f;
        const float boxPadding = 5.0f;
        float windowWidth = ImGui::GetContentRegionAvail().x;
        const int imagesPerRow = (int)(windowWidth / (thumbnailSize + 2 * boxPadding + spacing));

        for (const auto& folderPath : m_folders) { // Assuming m_folders is a std::vector<std::filesystem::path>
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
                ImGui::Image(folderIcon->GetDescriptorSet(), ImVec2(thumbnailSize, thumbnailSize/1.25)); // Use the folder icon

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
            ImGui::EndChild();
            ImGui::SameLine();
            columnIndex++;
        }

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
    }
    else {
        ImGui::Text("No images found in the directory.");
    }

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
	}
}

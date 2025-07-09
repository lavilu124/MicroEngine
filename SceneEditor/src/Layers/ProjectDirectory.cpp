// Revised ProjectDirectory UI code with centered thumbnails, styled layout, and selected highlighting

#include "ProjectDirectory.h"

std::string ProjectDirectory::m_copyPaths[30];
int ProjectDirectory::m_copySize = 0;

ProjectDirectory::ProjectDirectory(const std::string& path, bool* inputManagerOpen) : m_inputManagerOpen(inputManagerOpen){
    m_currentPath = path;
    m_mainPath = path;
}

ProjectDirectory::~ProjectDirectory()
{
    for (int i = 0; i < m_images.size(); i++)
    {
        m_images[i].first->~Image();
    }
    m_images.clear();
}

void ProjectDirectory::OnAttach()
{
    SetCurrentPath(m_currentPath);
    m_folderIcon = std::make_shared<Walnut::Image>("appGui\\folderIcon.png");
    m_mapIcon = std::make_shared<Walnut::Image>("appGui\\sceneIcon.png");
    m_reloadIcon = std::make_shared<Walnut::Image>("appGui\\reloadIcon.png");
    m_returnIcon = std::make_shared<Walnut::Image>("appGui\\returnIcon.png");
    m_soundIcon = std::make_shared<Walnut::Image>("appGui\\soundIcon.png");
	m_fontIcon = std::make_shared<Walnut::Image>("appGui\\fontIcon.png");
    m_fileIcon = std::make_shared<Walnut::Image>("appGui\\fileIcon.png");
}

void ProjectDirectory::SetCurrentPath(const std::string& path)
{
    if (path.find(m_mainPath) == std::string::npos)
        return;

    m_currentPath = path;

    for (int i = 0; i < m_images.size(); i++)
    {
        m_images[i].first->~Image();
    }
    m_images.clear();
    m_folders.clear();
    m_maps.clear();


    for (const auto& entry : std::filesystem::directory_iterator(m_currentPath)) {
        HandleFile(entry);
    }
}

void ProjectDirectory::HandleFile(const std::filesystem::directory_entry& entry)
{


    if (entry.is_directory())
    {
        for (auto folder : m_folders)
            if (folder == entry) return;
        m_folders.push_back(entry);
    }
    else if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg" || entry.path().extension() == ".jpeg") {
        for (auto img : m_images)
            if (img.second == entry) return;
        try {
            m_images.emplace_back(std::make_shared<Walnut::Image>(entry.path().string()), entry.path());
        }
        catch (const std::exception& e) {
            // Handle image loading failure
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to load: %s", entry.path().filename().c_str());
        }
    }
    else if (entry.path().extension() == ".McScene") {
        if (entry.path().parent_path() == m_currentPath) {
            for (int i = 0; i < m_maps.size(); i++)
                if (m_maps[i].string() == entry.path().string()) return;
            m_maps.push_back(entry);
        }
    }
}

void ProjectDirectory::OnUIRender()
{
    Window();
}

void ProjectDirectory::CreateNewFolder()
{
    std::string newFolderName = "New Folder";
    std::filesystem::path newPath = std::filesystem::path(m_currentPath) / newFolderName;

    int counter = 1;
    while (std::filesystem::exists(newPath))
    {
        newFolderName = "New Folder (" + std::to_string(counter++) + ")";
        newPath = std::filesystem::path(m_currentPath) / newFolderName;
    }

    std::filesystem::create_directory(newPath);
    HandleFile(std::filesystem::directory_entry(newPath));
}

void ProjectDirectory::Window()
{
    if (!m_isOpen) return;

    bool released = false;
    if (!m_selectedItemPath.empty())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !m_selectedItemPath.empty())
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
        released = true;
    }

    ImGui::Begin("Content Browser", &m_isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

    // Create two columns, left for folder tree, right for file content
    ImGui::Columns(2, "ContentBrowserColumns", true);
    ImGui::SetColumnWidth(0, 240.0f); // Sidebar width like Unity

    ImVec2 cursor = ImGui::GetCursorScreenPos();
    ImVec2 top = ImVec2(cursor.x - 1.0f, cursor.y);
    ImVec2 bottom = ImVec2(cursor.x - 1.0f, cursor.y + ImGui::GetWindowHeight());
    ImU32 shadowColor = IM_COL32(0, 0, 0, 90);
    ImGui::GetWindowDrawList()->AddRectFilled(top, bottom, shadowColor);

    // LEFT: Folder Hierarchy (Sidebar)
    {
        ImGui::BeginChild("Hierarchy", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        DrawFolderHierarchy(m_mainPath, released);
        ImGui::EndChild();
    }

    ImGui::NextColumn();

    // RIGHT: Content Explorer
    {
        const float iconSize = 24.0f;
        if (ImGui::ImageButton(m_reloadIcon->GetDescriptorSet(), ImVec2(iconSize, iconSize)))
            SetCurrentPath(m_currentPath);
        ImGui::SameLine();
        if (ImGui::ImageButton(m_returnIcon->GetDescriptorSet(), ImVec2(iconSize, iconSize)))
            SetCurrentPath(m_currentPath.substr(0, m_currentPath.find_last_of('\\')));

		ImGui::SameLine();
		static char searchBuffer[256] = "";
        if (ImGui::InputText("##search", searchBuffer, sizeof(searchBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(m_mainPath)) {
                if (entry.path().filename().string().find(searchBuffer) != std::string::npos)
                {
                    SetCurrentPath(entry.path().parent_path().string());

                    break;
                }
            }
        }

        // Handle file uploads
        if (m_copySize > 0)
        {
            for (int i = 0; i < m_copySize; ++i)
            {
                if (std::filesystem::exists(m_copyPaths[i]) && std::filesystem::exists(m_currentPath))
                {
                    auto fileName = std::filesystem::path(m_copyPaths[i]).filename();
                    auto destination = std::filesystem::path(m_currentPath) / fileName;
                    std::filesystem::copy_file(m_copyPaths[i], destination, std::filesystem::copy_options::overwrite_existing);
                    HandleFile(std::filesystem::directory_entry(destination));
                }
            }
            m_copySize = 0;
        }

        // Right-click context menu (only on empty space)
        if (ImGui::BeginPopupContextWindow("ContentBrowserPopup", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
        {
            if (ImGui::MenuItem("New Folder"))
            {
                CreateNewFolder(); 
            }
            ImGui::EndPopup();
        }

        int columnIndex = 0;
        columnIndex = ShowFoldersInDir(columnIndex);
        columnIndex = ShowImagesInDir(columnIndex);
        columnIndex = ShowMapsInDir(columnIndex);
        ShowFiles(columnIndex);


    }

    ImGui::Columns(1); // End 2-column layout
    ImGui::End();
    
}


int ProjectDirectory::ShowFoldersInDir(int columnIndex)
{
    const float spacing = 12.0f;
    const float boxPadding = 6.0f;
    float windowWidth = ImGui::GetContentRegionAvail().x;
    const float thumbnailSize = 100.0f;
    const int imagesPerRow = (int)(windowWidth / (thumbnailSize + 2 * boxPadding + spacing));


    bool deleted = false;
    for (int i = 0; i < m_folders.size(); i++) {
        const auto& folderPath = m_folders[i];
        if (columnIndex >= imagesPerRow) {
            columnIndex = 0;
            ImGui::NewLine();
        }

        ImGui::BeginChild(folderPath.filename().string().c_str(), ImVec2(thumbnailSize + 2 * boxPadding, thumbnailSize + 2 * boxPadding + ImGui::GetTextLineHeightWithSpacing() + 10));
        {
            ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            ImVec2 iconSize(thumbnailSize, thumbnailSize / 1.25f);
            ImVec2 cursorPos = ImGui::GetCursorPos();
            ImGui::SetCursorPosX((thumbnailSize + 2 * boxPadding - iconSize.x) * 0.5f + cursorPos.x);

            // Render folder icon
            ImGui::Image(m_folderIcon->GetDescriptorSet(), iconSize);

            if (ImGui::IsItemHovered()) {
                if (ImGui::IsMouseReleased(0) && !m_selectedItemPath.empty()) {
                    MoveDir(folderPath);
                }
                else {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
            }

            if (ImGui::IsItemClicked()) {
                SetCurrentPath(m_currentPath + "\\" + folderPath.filename().string().c_str());
            }

            if (ImGui::BeginPopupContextItem("FolderContext")) {
                if (ImGui::MenuItem("Delete")) {
                    if (TryDeleteEntry(folderPath)) {
                        deleted = true;
                    }
                }

                if (ImGui::MenuItem("Rename")) {
                    m_renameTargetPath = folderPath;
                    m_renameBuffer = folderPath.filename().string();
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            
            if (folderPath == m_renameTargetPath) {
                ImGui::PushItemWidth(thumbnailSize + boxPadding * 2);
                char buffer[256];
                strcpy(buffer, m_renameBuffer.c_str());

                if (ImGui::InputText("##Rename", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    std::filesystem::path newPath = folderPath.parent_path() / buffer;

                    if (!std::filesystem::exists(newPath)) {
                        std::filesystem::rename(folderPath, newPath);
                        SetCurrentPath(m_currentPath);
                    }

                    m_renameTargetPath.clear(); 
                }

                m_renameBuffer = buffer;

                
                if (!ImGui::IsItemActive() && !ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
                    m_renameTargetPath.clear();
                }

                ImGui::PopItemWidth();
            }
            else {
                std::string name = folderPath.filename().string();
                ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
                ImGui::SetCursorPosX((thumbnailSize + 2 * boxPadding - textSize.x) * 0.5f + cursorPos.x);
                ImGui::TextWrapped(name.c_str());
            }

            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        columnIndex++;
    }

    if (deleted)
        SetCurrentPath(m_currentPath);

    return columnIndex;
}

int ProjectDirectory::ShowImagesInDir(int columnIndex)
{
    const float spacing = 12.0f;
    const float boxPadding = 6.0f;
    const float thumbnailSize = 100.0f;
    float windowWidth = ImGui::GetContentRegionAvail().x;
    const int imagesPerRow = (int)(windowWidth / (thumbnailSize + 2 * boxPadding + spacing));

    bool deleted = false;
    for (auto& [image, imagePath] : m_images)
    {
        if (!image) continue;

        if (columnIndex >= imagesPerRow)
        {
            columnIndex = 0;
            ImGui::NewLine();
        }

        bool isSelected = (m_selectedItemPath == imagePath);
        if (isSelected)
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.26f, 0.59f, 0.98f, 0.25f)); // Unity-blue glow
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        }

        ImGui::PushID(columnIndex);
        ImGui::BeginChild(imagePath.filename().string().c_str(), ImVec2(thumbnailSize + 2 * boxPadding, thumbnailSize + 30.0f), false, ImGuiWindowFlags_NoScrollbar);
        {
            ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

            ImVec2 cursorPos = ImGui::GetCursorPos();
            ImVec2 size(thumbnailSize, thumbnailSize);
            ImGui::SetCursorPosX((thumbnailSize + 2 * boxPadding - size.x) * 0.5f + cursorPos.x);
            ImVec2 shadowOffset = ImVec2(2, 2);
            ImVec2 imagePos = ImGui::GetCursorScreenPos();
			ImVec2 final = { imagePos.x + shadowOffset.x, imagePos.y + shadowOffset.y };
            ImGui::GetWindowDrawList()->AddImage(
                image->GetDescriptorSet(),
                final,
                { final.x + size.x, final.y + size.y },
                ImVec2(0, 0),
                ImVec2(1, 1),
                IM_COL32(0, 0, 0, 50) // Transparent black
            );
            ImGui::Image(image->GetDescriptorSet(), size);

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("%s", imagePath.filename().string().c_str());
                ImGui::EndTooltip();
            }

            if (ImGui::IsItemClicked())
            {
                m_selectedItemPath = imagePath;
                m_selectedPath = imagePath.string();
				
            }

            if (ImGui::BeginPopupContextItem("ImageContext"))
            {
                if (ImGui::MenuItem("Delete"))
                {
                    if (TryDeleteEntry(imagePath))
                        deleted = true;
                }

                if (ImGui::MenuItem("Rename")) {
                    m_renameTargetPath = imagePath;
                    m_renameBuffer = imagePath.filename().string();
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            
            if (imagePath == m_renameTargetPath) {
                ImGui::PushItemWidth(thumbnailSize + boxPadding * 2);
                char buffer[256];
                strcpy(buffer, m_renameBuffer.c_str());

                if (ImGui::InputText("##Rename", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    std::filesystem::path newPath = imagePath.parent_path() / buffer;

                    if (!std::filesystem::exists(newPath)) {
                        std::filesystem::rename(imagePath, newPath);
                        SetCurrentPath(m_currentPath);
                    }

                    m_renameTargetPath.clear(); 
                }

                m_renameBuffer = buffer;

                
                if (!ImGui::IsItemActive() && !ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
                    m_renameTargetPath.clear();
                }

                ImGui::PopItemWidth();
            }
            else {
                std::string name = imagePath.filename().string();
                ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
                ImGui::SetCursorPosX((thumbnailSize + 2 * boxPadding - textSize.x) * 0.5f + cursorPos.x);
                ImGui::TextWrapped(name.c_str());
            }

            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();
        ImGui::PopID();

        if (isSelected) {
            ImGui::PopStyleVar();
            ImGui::PopStyleColor(2);
        }

        ImGui::SameLine();
        columnIndex++;
    }

    if (deleted)
        SetCurrentPath(m_currentPath);

    return columnIndex;
}

int ProjectDirectory::ShowMapsInDir(int columnIndex)
{
    const float spacing = 12.0f;
    const float boxPadding = 6.0f;
    const float thumbnailSize = 100.0f;
    float windowWidth = ImGui::GetContentRegionAvail().x;
    const int imagesPerRow = (int)(windowWidth / (thumbnailSize + 2 * boxPadding + spacing));

    bool deleted = false;
    for (const auto& mapPath : m_maps)
    {
        if (columnIndex >= imagesPerRow)
        {
            columnIndex = 0;
            ImGui::NewLine();
        }

        bool isSelected = (m_selectedItemPath == mapPath);
        if (isSelected) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.26f, 0.59f, 0.98f, 0.25f)); // Unity-blue glow
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        }

        ImGui::BeginChild(mapPath.filename().string().c_str(), ImVec2(thumbnailSize + 2 * boxPadding, thumbnailSize + 30.0f), false, ImGuiWindowFlags_NoScrollbar);
        {
            ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

            ImVec2 iconSize(thumbnailSize, thumbnailSize / 1.25f);
            ImVec2 cursorPos = ImGui::GetCursorPos();
            ImGui::SetCursorPosX((thumbnailSize + 2 * boxPadding - iconSize.x) * 0.5f + cursorPos.x);
            ImGui::Image(m_mapIcon->GetDescriptorSet(), iconSize);

            if (ImGui::IsItemClicked())
            {
                m_selectedItemPath = mapPath;
                m_newScene = mapPath.string();
            }

            if (ImGui::BeginPopupContextItem("MapContext"))
            {
                if (ImGui::MenuItem("Delete"))
                {
                    if (TryDeleteEntry(mapPath))
                        deleted = true;
                }

                if (ImGui::MenuItem("Rename")) {
                    m_renameTargetPath = mapPath;
                    m_renameBuffer = mapPath.filename().string();
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            if (mapPath == m_renameTargetPath) {
                ImGui::PushItemWidth(thumbnailSize + boxPadding * 2);
                char buffer[256];
                strcpy(buffer, m_renameBuffer.c_str());

                if (ImGui::InputText("##Rename", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    std::filesystem::path newPath = mapPath.parent_path() / buffer;

                    if (!std::filesystem::exists(newPath)) {
                        std::filesystem::rename(mapPath, newPath);
                        SetCurrentPath(m_currentPath);
                    }

                    m_renameTargetPath.clear();
                }

                m_renameBuffer = buffer;


                if (!ImGui::IsItemActive() && !ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
                    m_renameTargetPath.clear();
                }

                ImGui::PopItemWidth();
            }
            else {
                std::string name = mapPath.filename().string();
                ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
                ImGui::SetCursorPosX((thumbnailSize + 2 * boxPadding - textSize.x) * 0.5f + cursorPos.x);
                ImGui::TextWrapped(name.c_str());
            }

            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();

        if (isSelected) {
            ImGui::PopStyleVar();
            ImGui::PopStyleColor(2);
        }

        ImGui::SameLine();
        columnIndex++;
    }

    if (deleted)
        SetCurrentPath(m_currentPath);

    return columnIndex;
}


int ProjectDirectory::ShowFiles(int columnIndex)
{
    const float spacing = 10.0f;
    const float boxPadding = 5.0f;
    const float thumbnailSize = 100.0f;
    float windowWidth = ImGui::GetContentRegionAvail().x;
    const int imagesPerRow = (int)(windowWidth / (thumbnailSize + 2 * boxPadding + spacing));

    bool deleted = false;
    for (const auto& entry : std::filesystem::directory_iterator(m_currentPath))
    {
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg" || entry.path().extension() == ".jpeg" || entry.path().extension() == ".McScene" || entry.is_directory())
        {
            continue;
        }

        if (columnIndex >= imagesPerRow)
        {
            columnIndex = 0;
            ImGui::NewLine();
        }

        bool isSelected = (m_selectedItemPath == entry.path());
        if (isSelected) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.26f, 0.59f, 0.98f, 0.25f)); // Unity-blue glow
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        }

        ImGui::BeginChild(entry.path().filename().string().c_str(), ImVec2(thumbnailSize + 2 * boxPadding, thumbnailSize + 30.0f), false, ImGuiWindowFlags_NoScrollbar);
        {
            ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

            ImVec2 iconSize(thumbnailSize, thumbnailSize / 1.25f);
            ImVec2 cursorPos = ImGui::GetCursorPos();
            ImGui::SetCursorPosX((thumbnailSize + 2 * boxPadding - iconSize.x) * 0.5f + cursorPos.x);
            if (entry.path().extension() == ".ogg" || entry.path().extension() == ".wav" || entry.path().extension() == ".mp3")
            {
                ImGui::Image(m_soundIcon->GetDescriptorSet(), iconSize);
            }else if (entry.path().extension() == ".ttf")
            {
            	ImGui::Image(m_fontIcon->GetDescriptorSet(), iconSize);
            }else if (entry.path().extension() == ".cfg")
            {
                ImGui::Image(m_fileIcon->GetDescriptorSet(), iconSize);
                if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    *m_inputManagerOpen = true;
                    ImGui::SetWindowFocus("Input Manager");
                }
            }
        	else
            {

				ImGui::Image(m_fileIcon->GetDescriptorSet(), iconSize);
            }
            

            if (ImGui::IsItemClicked())
            {
                m_selectedItemPath = entry.path();
            }

            if (ImGui::BeginPopupContextItem("SoundContext"))
            {
                if (ImGui::MenuItem("Delete"))
                {
                    if (TryDeleteEntry(entry.path()))
                        deleted = true;
                }

                if (ImGui::MenuItem("Rename")) {
                    m_renameTargetPath = entry.path();
                    m_renameBuffer = entry.path().filename().string();
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            if (entry.path() == m_renameTargetPath) {
                ImGui::PushItemWidth(thumbnailSize + boxPadding * 2);
                char buffer[256];
                strcpy(buffer, m_renameBuffer.c_str());

                if (ImGui::InputText("##Rename", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    std::filesystem::path newPath = entry.path().parent_path() / buffer;

                    if (!std::filesystem::exists(newPath)) {
                        std::filesystem::rename(entry.path(), newPath);
                        SetCurrentPath(m_currentPath);
                    }

                    m_renameTargetPath.clear();
                }

                m_renameBuffer = buffer;


                if (!ImGui::IsItemActive() && !ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
                    m_renameTargetPath.clear();
                }

                ImGui::PopItemWidth();
            }
            else {
                std::string name = entry.path().filename().string();
                ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
                ImGui::SetCursorPosX((thumbnailSize + 2 * boxPadding - textSize.x) * 0.5f + cursorPos.x);
                ImGui::TextWrapped(name.c_str());
            }

            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();

        if (isSelected) {
            ImGui::PopStyleVar();
            ImGui::PopStyleColor(2);
        }

        ImGui::SameLine();
        columnIndex++;
        
    }

    if (deleted)
        SetCurrentPath(m_currentPath);

    return columnIndex;
}


void ProjectDirectory::DrawFolderHierarchy(const std::filesystem::path& path, bool released)
{
    std::vector<std::filesystem::directory_entry> folders;

    // Gather and sort folders
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory())
            folders.push_back(entry);
    }

    std::sort(folders.begin(), folders.end(), [](const auto& a, const auto& b) {
        return a.path().filename().string() < b.path().filename().string();
        });

    for (const auto& entry : folders)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

        if (entry.path() == m_currentPath)
            flags |= ImGuiTreeNodeFlags_Selected;

        ImGui::PushID(entry.path().string().c_str());

        // Calculate icon size
        const float iconSize = 16.0f;

        // Get TreeNode label (without padding spaces)
        const std::string& folderName = "    " + entry.path().filename().string();

        // Start the TreeNode
        bool open = ImGui::TreeNodeEx(folderName.c_str(), flags);
        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !m_selectedItemPath.empty())
        {
            MoveDir(entry.path().string());
            m_selectedItemPath.clear();
        }

        // Get the start of the TreeNode item (where arrow is)
        ImVec2 iconPos = ImGui::GetItemRectMin();
        iconPos.y += (ImGui::GetTextLineHeight() - iconSize) * 0.5f;
        iconPos.x += ImGui::GetTreeNodeToLabelSpacing(); // space between arrow and label

        // Draw the icon on top
        ImGui::SetCursorScreenPos(iconPos);
        ImGui::Image(m_folderIcon->GetDescriptorSet(), { iconSize, iconSize });

        // Clicking folder node selects it (unless toggling open/close)
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            SetCurrentPath(entry.path().string());
        }
        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !m_selectedItemPath.empty())
        {
            MoveDir(entry.path().string());  
            m_selectedItemPath.clear();
        }
        

        // Recursively draw children if open
        if (open)
        {
            DrawFolderHierarchy(entry.path(), released);
            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}

std::string ProjectDirectory::GetNewScene()
{
    std::string returnFile = m_newScene;
    m_newScene = "";
    return returnFile;
}

std::string ProjectDirectory::GetMainPath() const
{
    return m_mainPath;
}

void ProjectDirectory::UploadFile(const std::string& path)
{
    if (m_copySize > 30)
        return;

    m_copySize++;
    m_copyPaths[m_copySize - 1] = path;
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

void ProjectDirectory::MoveDir(std::filesystem::path path)
{
    auto newPath = path += "\\" + m_selectedItemPath.filename().string();

    std::filesystem::rename(m_selectedItemPath, newPath.string().c_str());

    SetCurrentPath(m_currentPath);
}
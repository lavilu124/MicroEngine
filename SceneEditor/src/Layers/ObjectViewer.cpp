#include "ObjectViewer.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

template <class T>
T min(T x, T y)
{
    return (x < y) ? x : y;
}

ObjectViewer::ObjectViewer(std::shared_ptr<ProjectDirectory> projectDirectory)
    : m_projectDirectory(projectDirectory)
{
}

void ObjectViewer::OnUIRender()
{
    Window();
}

void ObjectViewer::Window()
{
    ImGui::Begin("Object Viewer");

    if (m_currentObject == nullptr) {
        ImGui::Text("No Object Was Selected");
    }
    else {
        ImGui::Text("Selected Object");
        ImGui::Separator();

        ImGui::SameLine();
        // Delete icon button
        if (m_deleteIcon && m_deleteIcon->GetDescriptorSet()) {
            if (ImGui::ImageButton(m_deleteIcon->GetDescriptorSet(), ImVec2(15.5, 25))) {
                m_toDelete = true;
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Delete this object");
        }
        else {
            if (ImGui::Button("Delete Object", ImVec2(120, 0))) {
                m_toDelete = true;
            }
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        switch (m_currentObjectType) {
        case currentObjectType::game:
            if (ImGui::CollapsingHeader("Game Object", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
                ImGui::Indent();
                DisplayGameObject();
                ImGui::Unindent();
                ImGui::PopStyleVar();
            }
            break;
        case currentObjectType::light:
            if (ImGui::CollapsingHeader("Light Object", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
                ImGui::Indent();
                DisplayLightObject();
                ImGui::Unindent();
                ImGui::PopStyleVar();
            }
            break;
        case currentObjectType::text:
            if (ImGui::CollapsingHeader("Text Object", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
                ImGui::Indent();
                DisplayTextObject();
                ImGui::Unindent();
                ImGui::PopStyleVar();
            }
            break;
        case currentObjectType::button:
            if (ImGui::CollapsingHeader("Button Object", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
                ImGui::Indent();
                DisplayButtonObject();
                ImGui::Unindent();
                ImGui::PopStyleVar();
            }
        }
    }

    ImGui::End();
}

void ObjectViewer::DisplayGameObject()
{
    ImGui::Text("Object Details");
    ImGui::Separator();

    GameObject* GameOj = (GameObject*)m_currentObject;

    ImGui::Text("Name:");
    ImGui::Indent();
    static char nameBuffer[128];
    strcpy_s(nameBuffer, sizeof(nameBuffer), m_currentObject->name.c_str());
    if (ImGui::InputText("##GameName", nameBuffer, sizeof(nameBuffer))) {
        m_currentObject->name = nameBuffer;
    }

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::Checkbox("Visible", &m_currentObject->isVisable);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::Checkbox("Scene Object", &m_currentObject->IsSceneObject);

    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Position:");
    ImGui::Indent();
    float positionValues[2] = { m_currentObject->position.x, m_currentObject->position.y };
    if (ImGui::InputFloat2("##GamePosition", positionValues)) {
        m_currentObject->position.x = positionValues[0];
        m_currentObject->position.y = positionValues[1];
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Rotation (Degrees):");
    ImGui::Indent();
    if (ImGui::InputFloat("##GameRotation", &m_currentObject->rotation)) {
        m_currentObject->rotation = std::min<float>(m_currentObject->rotation, 360.0f);
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Scale:");
    ImGui::Indent();
    float scaleValues[2] = { GameOj->scale.x, GameOj->scale.y };
    if (ImGui::InputFloat2("##GameScale", scaleValues)) {
        GameOj->scale.x = scaleValues[0];
        GameOj->scale.y = scaleValues[1];
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Collison Layer:");
    ImGui::Indent();
    static const char* layerItems[] = {
        "0:collider", "1:custom", "2:custom", "3:custom", "4:custom", "5:custom", "6:trigger", "7:ignore",
        "8:custom", "9:custom", "10:custom", "11:custom", "12:custom", "13:all"
    };
    static int currentLayerIndex = GameOj->layer;

    if (ImGui::Combo("##LayerCombo", &currentLayerIndex, layerItems, IM_ARRAYSIZE(layerItems))) {
        GameOj->layer = currentLayerIndex;
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Level:");
    ImGui::Indent();
    if (ImGui::InputInt("##Level", &GameOj->level)) {
        GameOj->LevelChange();
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Type:");
    ImGui::Indent();
    
    static std::vector<const char*> typeCStrs;
    

    if (!m_ObjectsLoaded) {
        m_typeList.clear();
        typeCStrs.clear();
        m_typeList.push_back("none");

        std::filesystem::path rootPath = m_projectDirectory->getMainPath();
        rootPath.remove_filename();
        std::ifstream file(rootPath / "projectData.txt");
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                m_typeList.push_back(line);
            }
        }
        for (const auto& str : m_typeList) {
            typeCStrs.push_back(str.c_str());
        }
        m_ObjectsLoaded = true;
    }

    // Find current type index
    int currentIndex = 0;
    for (size_t i = 0; i < m_typeList.size(); ++i) {
        if (m_typeList[i] == GameOj->type) {
            currentIndex = static_cast<int>(i);
            break;
        }
    }

    if (ImGui::Combo("##GameType", &currentIndex, typeCStrs.data(), (int)typeCStrs.size())) {
        GameOj->type = m_typeList[currentIndex];
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Sprite:");
    ImGui::Indent();

    ImVec2 dropAreaSize = ImVec2(200, 200);
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    drawList->AddRectFilled(cursorPos, ImVec2(cursorPos.x + dropAreaSize.x, cursorPos.y + dropAreaSize.y), IM_COL32(50, 50, 50, 100));
    drawList->AddRect(cursorPos, ImVec2(cursorPos.x + dropAreaSize.x, cursorPos.y + dropAreaSize.y), IM_COL32(255, 255, 255, 255));

    ImGui::InvisibleButton("##SpriteDropArea", dropAreaSize);

    if (ImGui::IsItemHovered() && m_projectDirectory->GetSelectedPath() != "" && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        GameOj->SetPath(m_projectDirectory->GetSelectedPath());
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SetTooltip("Drop a sprite from the File Explorer here");
    }
    m_projectDirectory->ClearSelectedPath();

    if (GameOj->sprite && GameOj->sprite->GetDescriptorSet()) {
        float imgWidth = (float)GameOj->sprite->GetWidth();
        float imgHeight = (float)GameOj->sprite->GetHeight();
        float areaWidth = dropAreaSize.x;
        float areaHeight = dropAreaSize.y;

        // Calculate scale factor to fit the sprite within the drop area
        float scale = std::min(areaWidth / imgWidth, areaHeight / imgHeight);

        // Scaled image size
        ImVec2 imageSize = ImVec2(imgWidth * scale, imgHeight * scale);

        // Centered position
        ImVec2 imagePos = ImVec2(
            cursorPos.x + (areaWidth - imageSize.x) * 0.5f,
            cursorPos.y + (areaHeight - imageSize.y) * 0.5f
        );

        drawList->AddImage(
            GameOj->sprite->GetDescriptorSet(),
            imagePos,
            ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y)
        );
    }


    ImGui::Unindent();
}

void ObjectViewer::DisplayLightObject()
{
    ImGui::Text("Object Details: ");
    ImGui::Separator();

    LightObject* Light = (LightObject*)m_currentObject;

    ImGui::Text("Name:");
    ImGui::Indent();
    static char nameBuffer[128];
    strcpy_s(nameBuffer, sizeof(nameBuffer), m_currentObject->name.c_str());
    if (ImGui::InputText("##LightName", nameBuffer, sizeof(nameBuffer))) {
        m_currentObject->name = nameBuffer;
    }

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::Checkbox("Visible", &m_currentObject->isVisable);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::Checkbox("Scene Object", &m_currentObject->IsSceneObject);

    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Position:");
    ImGui::Indent();
    float positionValues[2] = { m_currentObject->position.x, m_currentObject->position.y };
    if (ImGui::InputFloat2("##LightPosition", positionValues)) {
        m_currentObject->position.x = positionValues[0];
        m_currentObject->position.y = positionValues[1];
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Rotation (Degrees):");
    ImGui::Indent();
    if (ImGui::InputFloat("##LightRotation", &m_currentObject->rotation)) {
        m_currentObject->rotation = min<float>(m_currentObject->rotation, 360.0f);
        Light->UpdateVal();
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Color:");
    ImGui::Indent();
    float colorValues[3] = { Light->color.x / 255.0f, Light->color.y / 255.0f, Light->color.z / 255.0f };
    if (ImGui::ColorEdit3("##LightColor", colorValues)) {
        Light->color.x = colorValues[0] * 255;
        Light->color.y = colorValues[1] * 255;
        Light->color.z = colorValues[2] * 255;
        Light->UpdateVal();
    }
    ImGui::Unindent();

    ImGui::Spacing();


    ImGui::Text("Intensity:");
    ImGui::Indent();
    float displayedInt = Light->color.w;
    if (ImGui::InputFloat("##LightIntensity", &displayedInt)) {
        Light->color.w = min<float>(displayedInt, 1.0f);
        Light->UpdateVal();
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Type:");
    ImGui::Indent();
    if (ImGui::RadioButton("Radial", Light->type == 0)) {
        Light->type = 0;
        Light->UpdateVal();
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Directed", Light->type == 1)) {
        Light->type = 1;
        Light->UpdateVal();
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Fade:");
    ImGui::Indent();
    if (ImGui::RadioButton("true", Light->fade)) {
        Light->fade = true;
        Light->UpdateVal();
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("false", !Light->fade)) {
        Light->fade = false;
        Light->UpdateVal();
    }
    ImGui::Unindent();

    ImGui::Spacing();

    if (Light->type == 0) {
        ImGui::Text("Radius:");
        ImGui::Indent();
        float displayRad = Light->radius;
        if (ImGui::InputFloat("##LightRadius", &displayRad)) {
            Light->radius = min<float>(displayRad, 512.0f);
            Light->UpdateVal();
        }
        ImGui::Unindent();

        ImGui::Spacing();

        ImGui::Text("Beam Angle:");
        ImGui::Indent();
        if (ImGui::InputFloat("##BeamAngle", &Light->beamAngle)) {
            Light->beamAngle = min<float>(Light->beamAngle, 360.0f);
            Light->UpdateVal();
        }
        ImGui::Unindent();
    }
    else if (Light->type == 1) {
        ImGui::Text("Length:");
        ImGui::Indent();
        float displayRad = Light->radius;
        if (ImGui::InputFloat("##LightLength", &displayRad)) {
            Light->radius = min<float>(displayRad, 512.0f);
            Light->UpdateVal();
        }
        ImGui::Unindent();

        ImGui::Text("Width:");
        ImGui::Indent();
        float displayWi = Light->width;
        if (ImGui::InputFloat("##LightWidth", &displayWi)) {
            Light->width = min<float>(displayWi, 1024.0f);
            Light->UpdateVal();
        }
        ImGui::Unindent();
    }
}

void ObjectViewer::DisplayTextObject()
{
    ImGui::Text("Text Object Details:");
    ImGui::Separator();

    TextObject* textObj = (TextObject*)m_currentObject;

    ImGui::Text("Name:");
    ImGui::Indent();
    static char nameBuffer[128];
    strcpy_s(nameBuffer, sizeof(nameBuffer), m_currentObject->name.c_str());
    if (ImGui::InputText("##TextName", nameBuffer, sizeof(nameBuffer))) {
        m_currentObject->name = nameBuffer;
    }

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::Checkbox("Visible", &m_currentObject->isVisable);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::Checkbox("Scene Object", &m_currentObject->IsSceneObject);

    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Value:");
    ImGui::Indent();
    static char valueBuffer[512];
    strcpy_s(valueBuffer, sizeof(valueBuffer), textObj->value.c_str());
    if (ImGui::InputTextMultiline("##TextValue", valueBuffer, sizeof(valueBuffer), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 4))) {
        textObj->value = valueBuffer;
        textObj->UpdateVal();
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Font:");
    ImGui::Indent();
    static char fontBuffer[128];
    strcpy_s(fontBuffer, sizeof(fontBuffer), textObj->font.c_str());
    if (ImGui::InputText("##TextFont", fontBuffer, sizeof(fontBuffer))) {
        textObj->font = fontBuffer;
        textObj->UpdateVal();
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Size:");
    ImGui::Indent();
    if (ImGui::InputInt("##TextSize", (int*)&textObj->size)) {
        textObj->UpdateVal();
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Scale:");
    ImGui::Indent();
    float scale[2] = { textObj->scale.x, textObj->scale.y };
    if (ImGui::InputFloat2("##TextScale", scale)) {
        textObj->scale = ImVec2(scale[0], scale[1]);
        textObj->UpdateVal();
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Position:");
    ImGui::Indent();
    float pos[2] = { m_currentObject->position.x, m_currentObject->position.y };
    if (ImGui::InputFloat2("##TextPosition", pos)) {
        m_currentObject->position = ImVec2(pos[0], pos[1]);
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Rotation (Degrees):");
    ImGui::Indent();
    if (ImGui::InputFloat("##TextRotation", &m_currentObject->rotation)) {
        m_currentObject->rotation = std::min<float>(m_currentObject->rotation, 360.0f);
        textObj->UpdateVal();
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Color:");
    ImGui::Indent();
    float color[4] = { textObj->color.x / 255.0f, textObj->color.y / 255.0f, textObj->color.z / 255.0f, textObj->color.w / 255.0f };
    if (ImGui::ColorEdit4("##TextColor", color)) {
        textObj->color = ImVec4(color[0] * 255.0f, color[1] * 255.0f, color[2] * 255.0f, color[3] * 255.0f);
        textObj->UpdateVal();
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Outline Color:");
    ImGui::Indent();
    float outlineColor[4] = { textObj->outlineColor.x / 255.0f, textObj->outlineColor.y / 255.0f, textObj->outlineColor.z / 255.0f, textObj->outlineColor.w / 255.0f };
    if (ImGui::ColorEdit4("##TextOutlineColor", outlineColor)) {
        textObj->outlineColor = ImVec4(outlineColor[0] * 255.0f, outlineColor[1] * 255.0f, outlineColor[2] * 255.0f, outlineColor[3] * 255.0f);
        textObj->UpdateVal();
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Outline Thickness:");
    ImGui::Indent();
    if (ImGui::InputFloat("##TextOutlineThickness", &textObj->outlineThickness)) {
        textObj->UpdateVal();
    }
    ImGui::Unindent();
}

void ObjectViewer::DisplayButtonObject() {
    ImGui::Text("Button Object Details:");
    ImGui::Separator();

    ButtonObject* buttonObj = (ButtonObject*)m_currentObject;

    // Name
    ImGui::Text("Name:");
    ImGui::Indent();
    static char nameBuffer[128];
    strcpy_s(nameBuffer, sizeof(nameBuffer), m_currentObject->name.c_str());
    if (ImGui::InputText("##ButtonName", nameBuffer, sizeof(nameBuffer))) {
        m_currentObject->name = nameBuffer;
    }

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::Checkbox("Visible", &m_currentObject->isVisable);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::Checkbox("Scene Object", &m_currentObject->IsSceneObject);

    ImGui::Unindent();

    ImGui::Spacing();

    // Position
    ImGui::Text("Position:");
    ImGui::Indent();
    float pos[2] = { m_currentObject->position.x, m_currentObject->position.y };
    if (ImGui::InputFloat2("##ButtonPosition", pos)) {
        m_currentObject->position = ImVec2(pos[0], pos[1]);
    }
    ImGui::Unindent();

    ImGui::Spacing();

    // Rotation
    ImGui::Text("Rotation (Degrees):");
    ImGui::Indent();
    if (ImGui::InputFloat("##ButtonRotation", &m_currentObject->rotation)) {
        m_currentObject->rotation = std::min<float>(m_currentObject->rotation, 360.0f);
    }
    ImGui::Unindent();

    ImGui::Spacing();

    // Scale
    ImGui::Text("Scale:");
    ImGui::Indent();
    float scale[2] = { buttonObj->scale.x, buttonObj->scale.y };
    if (ImGui::InputFloat2("##ButtonScale", scale)) {
        buttonObj->scale = ImVec2(scale[0], scale[1]);
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("image:");
    ImGui::Indent();

    ImVec2 dropAreaSize = ImVec2(200, 200);
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    drawList->AddRectFilled(cursorPos, ImVec2(cursorPos.x + dropAreaSize.x, cursorPos.y + dropAreaSize.y), IM_COL32(50, 50, 50, 100));
    drawList->AddRect(cursorPos, ImVec2(cursorPos.x + dropAreaSize.x, cursorPos.y + dropAreaSize.y), IM_COL32(255, 255, 255, 255));

    ImGui::InvisibleButton("##SpriteDropArea", dropAreaSize);

    if (ImGui::IsItemHovered() && m_projectDirectory->GetSelectedPath() != "" && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        buttonObj->SetPathReg(m_projectDirectory->GetSelectedPath());
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SetTooltip("Drop a sprite from the File Explorer here");
    }

    if (buttonObj->image && buttonObj->image->GetDescriptorSet()) {
        float imgWidth = (float)buttonObj->image->GetWidth();
        float imgHeight = (float)buttonObj->image->GetHeight();
        float areaWidth = dropAreaSize.x;
        float areaHeight = dropAreaSize.y;

        // Calculate scaling factor to fit image in drop area while keeping aspect ratio
        float scale = std::min(areaWidth / imgWidth, areaHeight / imgHeight);

        // Scaled image size
        ImVec2 imageSize = ImVec2(imgWidth * scale, imgHeight * scale);

        // Center the image in the drop area
        ImVec2 imagePos = ImVec2(
            cursorPos.x + (areaWidth - imageSize.x) * 0.5f,
            cursorPos.y + (areaHeight - imageSize.y) * 0.5f
        );

        drawList->AddImage(
            buttonObj->image->GetDescriptorSet(),
            imagePos,
            ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y)
        );
    }


    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("on click image:");
    ImGui::Indent();

    cursorPos = ImGui::GetCursorScreenPos();

    drawList->AddRectFilled(cursorPos, ImVec2(cursorPos.x + dropAreaSize.x, cursorPos.y + dropAreaSize.y), IM_COL32(50, 50, 50, 100));
    drawList->AddRect(cursorPos, ImVec2(cursorPos.x + dropAreaSize.x, cursorPos.y + dropAreaSize.y), IM_COL32(255, 255, 255, 255));

    ImGui::InvisibleButton("##Sprite2DropArea", dropAreaSize);

    if (ImGui::IsItemHovered() && m_projectDirectory->GetSelectedPath() != "" && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        buttonObj->SetPathClick(m_projectDirectory->GetSelectedPath());
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SetTooltip("Drop a sprite from the File Explorer here");
    }
    m_projectDirectory->ClearSelectedPath();

    if (buttonObj->clickImage && buttonObj->clickImage->GetDescriptorSet()) {
        float imgWidth = (float)buttonObj->clickImage->GetWidth();
        float imgHeight = (float)buttonObj->clickImage->GetHeight();
        float areaWidth = dropAreaSize.x;
        float areaHeight = dropAreaSize.y;

        // Calculate scale to fit while preserving aspect ratio
        float scale = std::min(areaWidth / imgWidth, areaHeight / imgHeight);

        // Scaled image size
        ImVec2 imageSize = ImVec2(imgWidth * scale, imgHeight * scale);

        // Centered image position
        ImVec2 imagePos = ImVec2(
            cursorPos.x + (areaWidth - imageSize.x) * 0.5f,
            cursorPos.y + (areaHeight - imageSize.y) * 0.5f
        );

        drawList->AddImage(
            buttonObj->clickImage->GetDescriptorSet(),
            imagePos,
            ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y)
        );
    }


    ImGui::Unindent();

    ImGui::Spacing();

    // OnClick Function Name
    ImGui::Text("OnClick Function Name:");
    ImGui::Indent();
    static char funcBuffer[256];
    strcpy_s(funcBuffer, sizeof(funcBuffer), buttonObj->onClickFunc.c_str());
    if (ImGui::InputText("##ButtonOnClickFunction", funcBuffer, sizeof(funcBuffer))) {
        buttonObj->onClickFunc = funcBuffer;
    }
    ImGui::Unindent();

    ImGui::Spacing();
}

void ObjectViewer::SetObject(Object* newObject, currentObjectType type)
{
    m_currentObject = newObject;
    m_currentObjectType = type;
}

currentObjectType ObjectViewer::GetCurrentObjectType()
{
    return m_currentObjectType;
}

Object* ObjectViewer::GetObject() const
{
    return m_currentObject;
}

bool ObjectViewer::Delete()
{
    if (m_toDelete) {
        m_toDelete = false;
        return true;
    }
    return false;
}

void ObjectViewer::OnAttach()
{
    m_deleteIcon = std::make_shared<Walnut::Image>("appGui\\deleteIcon.png");
}

void ObjectViewer::NewObject()
{
    m_ObjectsLoaded = false;
}
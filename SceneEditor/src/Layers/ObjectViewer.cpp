#include "ObjectViewer.h"

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
        m_currentObject->rotation = min<float>(m_currentObject->rotation, 360.0f);
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

    ImGui::Text("Sprite:");
    ImGui::Indent();

    ImVec2 dropAreaSize = ImVec2(200, 200); 
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Draw the drop area background
    drawList->AddRectFilled(cursorPos, ImVec2(cursorPos.x + dropAreaSize.x, cursorPos.y + dropAreaSize.y), IM_COL32(50, 50, 50, 100));
    drawList->AddRect(cursorPos, ImVec2(cursorPos.x + dropAreaSize.x, cursorPos.y + dropAreaSize.y), IM_COL32(255, 255, 255, 255));

    ImGui::InvisibleButton("##SpriteDropArea", dropAreaSize);

    // Handle drop
    if (ImGui::IsItemHovered() && m_projectDirectory->GetSelectedPath() != "" && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        GameOj->SetPath(m_projectDirectory->GetSelectedPath());
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::SetTooltip("Drop a sprite from the File Explorer here");
    }
    m_projectDirectory->ClearSelectedPath();

    // Display the sprite if available
    if (GameOj->sprite && GameOj->sprite->GetDescriptorSet()) {
        ImVec2 imageSize = ImVec2((float)GameOj->sprite->GetWidth(), (float)GameOj->sprite->GetHeight());
        ImVec2 imagePos = ImVec2(cursorPos.x + (dropAreaSize.x - imageSize.x) / 2, cursorPos.y + (dropAreaSize.y - imageSize.y) / 2);
        drawList->AddImage(GameOj->sprite->GetDescriptorSet(), imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));
    }

    ImGui::Unindent();


    ImGui::Indent();
    try {
        if (GameOj->sprite)
            ImGui::Image(GameOj->sprite->GetDescriptorSet(), { (float)GameOj->sprite->GetWidth(), (float)GameOj->sprite->GetHeight() });
    }
    catch (std::exception) {
        // Fail silently
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
    float colorValues[3] = { Light->color.Value.x, Light->color.Value.y, Light->color.Value.z };
    if (ImGui::ColorEdit3("##LightColor", colorValues)) {
        Light->color.Value.x = colorValues[0];
        Light->color.Value.y = colorValues[1];
        Light->color.Value.z = colorValues[2];
        Light->UpdateVal();
    }
    ImGui::Unindent();

    ImGui::Spacing();

    ImGui::Text("Intensity:");
    ImGui::Indent();
    if (ImGui::InputFloat("##LightIntensity", &Light->color.Value.w)) {
        Light->color.Value.w = min<float>(Light->color.Value.w, 1.0f);
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

    if (Light->type == 0) {
        ImGui::Text("Radius:");
        ImGui::Indent();
        float displayRad = Light->radius * 1.5f;
        if (ImGui::InputFloat("##LightRadius", &displayRad)) {
            Light->radius = min<float>(displayRad / 1.5f, 512.0f);
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
        float displayRad = Light->radius * 1.5f;
        if (ImGui::InputFloat("##LightLength", &displayRad)) {
            Light->radius = min<float>(displayRad / 1.5f, 512.0f);
            Light->UpdateVal();
        }
        ImGui::Unindent();
    }
}

void ObjectViewer::SetObject(Object* newObject, currentObjectType type)
{
    m_currentObject = newObject;
    m_currentObjectType = type;
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

#include "SceneViewer.h"
#include <filesystem>

#include "../fileManage/FileManage.h"
#include <iostream>

SceneViewer::SceneViewer(std::shared_ptr<SceneContent> sceneContent, const char* mainPath) : m_sceneContent(sceneContent), m_mainPath(mainPath)
{
}

void SceneViewer::OnAttach()
{
    renderTexture.create(1024, 1024);
    renderTexture.clear(sf::Color::Transparent);

    std::string playImagePath = "appGui/playIcon.png";
    m_playButtonImage = std::make_shared<Walnut::Image>(playImagePath.c_str());
    std::string saveImagePath = "appGui/saveIcon.png";
    m_saveButtonImage = std::make_shared<Walnut::Image>(saveImagePath.c_str());
}

void SceneViewer::OnUIRender()
{
    Window();
}

void SceneViewer::RenderHeader(const ImVec2& contentRegion) {
    constexpr float buttonWidth = 25.0f;
    constexpr float buttonHeight = 25.0f;

    ImVec2 buttonPos((contentRegion.x - buttonWidth) / 2.0f, 0.0f);

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4 originalBgColor = style.Colors[ImGuiCol_ChildBg];
    ImVec4 originalBorderColor = style.Colors[ImGuiCol_Border];
    float originalBorderSize = style.WindowBorderSize;
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0, 0, 0, 0);
    style.Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);
    style.WindowBorderSize = 0.0f;

    ImGui::BeginChild("##header", ImVec2(contentRegion.x, buttonHeight * 1.4));

    ImGui::SetCursorPos({ 0.0f, 0.0f });
    if (ImGui::ImageButton(m_saveButtonImage->GetDescriptorSet(), ImVec2(buttonWidth * 0.8, buttonHeight * 0.8)))
    {
        if (m_sceneContent->GetCurrentScene() == "") {
            //open save window
            m_saving = true;
        }
        else {
            FileManage::SaveScene(m_sceneContent->GetCurrentScene(), m_sceneContent.get());
        }
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::BeginTooltip();
        ImGui::Text("%s", "Save");
        ImGui::EndTooltip();
    }

    ImGui::SetCursorPos(buttonPos);
    if (ImGui::ImageButton(m_playButtonImage->GetDescriptorSet(), ImVec2(buttonWidth * 0.8, buttonHeight * 0.8)))
        ExecutePlayCommand();

    if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::BeginTooltip();
        ImGui::Text("%s", "Run");
        ImGui::EndTooltip();
    }

    ImGui::EndChild();

    style.Colors[ImGuiCol_ChildBg] = originalBgColor;
    style.Colors[ImGuiCol_Border] = originalBorderColor;
    style.WindowBorderSize = originalBorderSize;
}

void SceneViewer::ExecutePlayCommand() const {
    std::string command = std::filesystem::current_path().string();


    if (command.find("SceneEditor") != std::string::npos) 
        command = command.substr(0, command.find_last_of('\\'));
    command += "\\binaries\\windows-x86_64\\Release\\sendbox\\sendbox.exe ";

    std::string scene = m_sceneContent->GetCurrentScene();
    scene = scene.substr(scene.find_last_of('\\') + 1, scene.size() - scene.find_last_of('\\') - 9);
    command += scene;

    system(command.c_str());
}

void SceneViewer::Window()
{
    ImGui::Begin("Scene Viewer");
    if (m_saving)
        SaveWindow();

    ImVec2 contentRegion = ImGui::GetContentRegionAvail();
    RenderHeader(contentRegion);
    ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + contentRegion.x, ImGui::GetCursorScreenPos().y + contentRegion.y), IM_COL32(0, 0, 0, 255));
    RenderGameObjects();
    RenderLights();
    ImGui::End();
}

void SceneViewer::SaveWindow()
{
    ImGui::Begin("Save");
    ImGui::Text("Save Scene");

    static bool initialized = false;
    static char pathBuffer[1024];
    static char fileNameBuffer[128];

    if (!initialized)
    {
        strcpy_s(pathBuffer, sizeof(pathBuffer), m_mainPath.c_str());
        std::cout << m_mainPath;
        std::string fileName = m_savePath.substr(m_savePath.find_last_of('\\') + 1);
        strcpy_s(fileNameBuffer, sizeof(fileNameBuffer), fileName.c_str());
        initialized = true;
    }

    ImGui::Text("Path:");
    ImGui::Indent();

    if (ImGui::InputText("##Path", pathBuffer, sizeof(pathBuffer)))
    {
        m_savePath = pathBuffer;
    }

    ImGui::Unindent();

    ImGui::Text("File Name:");
    ImGui::Indent();

    if (ImGui::InputText("##FileName", fileNameBuffer, sizeof(fileNameBuffer)))
    {
        m_saveName = fileNameBuffer;
    }

    ImGui::Unindent();

    if (ImGui::Button("Save"))
    {
        if (m_savePath.empty())
            m_savePath = m_mainPath;

        std::string fullPath = m_savePath + "\\" + m_saveName + ".McScene";
        FileManage::SaveScene(fullPath, m_sceneContent.get());

        m_doneSaving = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("Close"))
    {
        m_saving = false;
        m_doneSaving = false;
        initialized = false; // Reset buffer on close
        ImGui::End();
        return;
    }

    if (m_doneSaving)
        ImGui::Text("Scene saved successfully");

    ImGui::End();
}

void SceneViewer::RenderGameObjects() const
{
    ImVec2 contentRegion = ImGui::GetContentRegionAvail();
    for (auto& gameObject : m_sceneContent->GetGameObjects()) {
        if (gameObject.sprite) {
            ImVec2 relativePos(gameObject.position.x / 2, (gameObject.position.y + 25.0f) / 2);
            ImVec2 position(relativePos.x + contentRegion.x / 2, relativePos.y + contentRegion.y / 2);
            ImGui::SetCursorPos(position);
            ImGui::Image(gameObject.sprite->GetDescriptorSet(), ImVec2(gameObject.scale.x / 1.5 * gameObject.sprite->GetWidth(), gameObject.scale.y / 1.5 * gameObject.sprite->GetHeight()));
        }
    }
}

void SceneViewer::RenderLights()
{
    ImVec2 contentRegion = ImGui::GetContentRegionAvail();
    for (auto& light : m_sceneContent->GetLights()) {
        ImGui::SetCursorPos(
            ImVec2(light.position.x /2 + contentRegion.x / 2 - 1024/2, light.position.y + contentRegion.y / 2 - 1024/2 + 75));

    	if (!light.image || !light.isUpdated()) {
            light.image = GenerateLightImage(light);
        }

        ImGui::Image(light.image->GetDescriptorSet(), ImVec2(1024, 1024));
    }
}

std::shared_ptr<Walnut::Image> SceneViewer::GenerateLightImage(LightObject& light)
{
    light.UpdateVal();

    Micro::RadialLight radialLight(light.name, 0);
    radialLight.setRange(light.radius);
    radialLight.setColor(sf::Color(static_cast<int>(light.color.Value.x * 255), static_cast<int>(light.color.Value.y * 255), static_cast<int>(light.color.Value.z * 255)));
    radialLight.setIntensity(light.color.Value.w);
    radialLight.setPosition(1024/2, 1024 / 2);

    radialLight.setBeamAngle(light.beamAngle);
    std::vector<sfu::Line> vec;
    radialLight.castLight(vec.begin(), vec.end());

    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(radialLight);
    renderTexture.display();
    const sf::Texture& texture = renderTexture.getTexture();

    std::string path = std::filesystem::current_path().string() + "\\temp.png";
    texture.copyToImage().saveToFile("temp.png");
    auto returnVal = std::make_shared<Walnut::Image>(path);
    std::remove("temp.png");

    return returnVal;
}

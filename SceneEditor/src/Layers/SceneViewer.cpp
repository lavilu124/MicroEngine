#include "SceneViewer.h"
#include <filesystem>

SceneViewer::SceneViewer(std::shared_ptr<SceneContent> sceneContent) : m_sceneContent(sceneContent)
{
}

void SceneViewer::OnAttach()
{
    renderTexture.create(1024, 1024);
    renderTexture.clear(sf::Color::Transparent);

    std::string playImagePath = "appGui/play.png";
    m_playButtonImage = std::make_shared<Walnut::Image>(playImagePath.c_str());
}

void SceneViewer::OnUIRender()
{
    Window();
}

void SceneViewer::RenderPlayButton(const ImVec2& contentRegion) {
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

    ImGui::BeginChild("##header", ImVec2(contentRegion.x, buttonHeight));
    ImGui::SetCursorPos(buttonPos);
    ImGui::Image(m_playButtonImage->GetDescriptorSet(), ImVec2(buttonWidth, buttonHeight));

    if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    if (ImGui::IsItemClicked()) ExecutePlayCommand();

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
    command += m_sceneContent->GetCurrentScene();

    system(command.c_str());
}

void SceneViewer::Window()
{
    ImGui::Begin("Scene Viewer");
    ImVec2 contentRegion = ImGui::GetContentRegionAvail();
    RenderPlayButton(contentRegion);
    ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + contentRegion.x, ImGui::GetCursorScreenPos().y + contentRegion.y), IM_COL32(0, 0, 0, 255));
    RenderGameObjects();
    RenderLights();
    ImGui::End();
}

void SceneViewer::RenderGameObjects() const
{
    ImVec2 contentRegion = ImGui::GetContentRegionAvail();
    for (auto& gameObject : m_sceneContent->GetGameObjects()) {
        if (gameObject.sprite) {
            ImVec2 relativePos(gameObject.position.x / 2.4, gameObject.position.y / 2.4);
            ImVec2 position(relativePos.x + contentRegion.x / 2, relativePos.y + contentRegion.y / 2);
            ImGui::SetCursorPos(position);
            ImGui::Image(gameObject.sprite->GetDescriptorSet(), ImVec2(gameObject.scale.x * gameObject.sprite->GetWidth(), gameObject.scale.y * gameObject.sprite->GetHeight()));
        }
    }
}

void SceneViewer::RenderLights()
{
    ImVec2 contentRegion = ImGui::GetContentRegionAvail();
    for (auto& light : m_sceneContent->GetLights()) {
        ImGui::SetCursorPos(
            ImVec2(light.position.x + contentRegion.x / 2 - 1024/2, light.position.y + contentRegion.y / 2 - 1024/2 + 75));

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
    radialLight.setPosition(1024 / 2, 1024 / 2);

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

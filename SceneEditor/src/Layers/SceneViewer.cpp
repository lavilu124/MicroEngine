#include "SceneViewer.h"
#include <filesystem>

#include "../fileManage/FileManage.h"

SceneViewer::SceneViewer(std::shared_ptr<SceneContent> sceneContent, const char* mainPath) : m_sceneContent(sceneContent), m_mainPath(mainPath)
{
}

void SceneViewer::OnDetach()
{
    renderTexture.clear();
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
    ImGui::Begin("Scene Viewer", nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
    if (m_saving)
        SaveWindow();

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 mousePos = io.MousePos;
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    ImVec2 relativeMousePos = { mousePos.x - cursorPos.x, mousePos.y - cursorPos.y };

    // Zoom (scroll wheel)
    if (ImGui::IsWindowHovered() && io.MouseWheel != 0.0f)
    {
        float zoomFactor = 1.1f;
        float oldZoom = m_zoom;
        m_zoom *= (io.MouseWheel > 0) ? zoomFactor : (1.0f / zoomFactor);

        // Zoom towards mouse position
        m_offset.x -= (relativeMousePos.x - m_offset.x) * (1.0f - m_zoom / oldZoom);
        m_offset.y -= (relativeMousePos.y - m_offset.y) * (1.0f - m_zoom / oldZoom);
    }

    // Panning (mouse drag)
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
    {
        ImVec2 delta = io.MouseDelta;
        m_offset.x += delta.x;
        m_offset.y += delta.y;
    }

    ImVec2 contentRegion = ImGui::GetContentRegionAvail();
    RenderHeader(contentRegion);
    ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + contentRegion.x, ImGui::GetCursorScreenPos().y + contentRegion.y), IM_COL32(0, 0, 0, 255));
    RenderGameObjects(contentRegion);
    RenderLights(contentRegion);
    renderTexts(contentRegion);
    renderButtons(contentRegion);
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

void SceneViewer::RenderGameObjects(ImVec2 contentRegion) const
{
    for (auto& gameObject : m_sceneContent->GetGameObjects()) {
        if (gameObject.sprite) {
            sf::Vector2f scaledPos = sf::Vector2f(gameObject.position.x * m_zoom, gameObject.position.y * m_zoom) + sf::Vector2f(m_offset.x, m_offset.y);
            ImVec2 position(scaledPos.x + contentRegion.x / 2, scaledPos.y + contentRegion.y / 2);
            ImVec2 size = {
                gameObject.scale.x * gameObject.sprite->GetWidth() * m_zoom,
                gameObject.scale.y * gameObject.sprite->GetHeight() * m_zoom
            };

            if (position.y <= 75.f) {
                continue;
            }
            ImGui::SetCursorPos(position);
            ImGui::Image(gameObject.sprite->GetDescriptorSet(), size);
        }
    }
}

void SceneViewer::RenderLights(ImVec2 contentRegion)
{
    sf::Vector2f textureOffset = sf::Vector2f(renderTexture.getSize().x / 2, renderTexture.getSize().x / 2);
    for (auto& light : m_sceneContent->GetLights()) {
        sf::Vector2f pos = sf::Vector2f(light.position.x - textureOffset.x, light.position.y - textureOffset.y);
        (pos.x > 0) ? pos.x += 25 : pos.x -= 25;
        (pos.y > 0) ? pos.y -= 25 : pos.y += 25;
        sf::Vector2f scaledPos = (pos * m_zoom) + sf::Vector2f(m_offset.x, m_offset.y);
        ImVec2 position(scaledPos.x + contentRegion.x / 2 , scaledPos.y + contentRegion.y / 2);
        ImVec2 size = {
                renderTexture.getSize().x * m_zoom,
                renderTexture.getSize().y * m_zoom
        };

        if (position.y <= (75.f - 512)) {
            continue;
        }

        ImGui::SetCursorPos(position);
        

    	if (!light.isUpdated()) {
           GenerateLightImage(light);
        }

        if (light.image) {
            ImVec4 brightTint = ImVec4(1.5f, 1.5f, 1.5f, 1.0f); 
            ImGui::Image(light.image->GetDescriptorSet(), size, ImVec2(0, 0), ImVec2(1, 1), brightTint);
        }  
    }
}

void SceneViewer::renderTexts(ImVec2 contentRegion) {
    sf::Vector2f textureOffset = sf::Vector2f(renderTexture.getSize().x / 2, renderTexture.getSize().y / 2);
    for (auto& text : m_sceneContent->GetTexts()) {
        sf::Vector2f pos = sf::Vector2f(text.position.x - textureOffset.x, text.position.y - textureOffset.y);
        sf::Vector2f scaledPos = (pos * m_zoom) + sf::Vector2f(m_offset.x, m_offset.y);
        ImVec2 position(scaledPos.x + contentRegion.x / 2, scaledPos.y + contentRegion.y / 2);
        ImVec2 size = {
                renderTexture.getSize().x * m_zoom,
                renderTexture.getSize().y * m_zoom
        };

        if (position.y <= (75.f - 512)) {
            continue;
        }

        ImGui::SetCursorPos(position);


        if (!text.isUpdated()) {
            GenerateTextImage(text);
        }

        if (text.image)
            ImGui::Image(text.image->GetDescriptorSet(), size);
    }
}

void SceneViewer::renderButtons(ImVec2 contentRegion) { 
    for (auto& button : m_sceneContent->GetButtons()) {
        if (button.image) {
            sf::Vector2f scaledPos = sf::Vector2f(button.position.x * m_zoom, button.position.y * m_zoom) + sf::Vector2f(m_offset.x, m_offset.y);
            ImVec2 position(scaledPos.x + contentRegion.x / 2, scaledPos.y + contentRegion.y / 2);
            ImVec2 size = {
                (button.scale.x) * button.image->GetWidth() * m_zoom,
                (button.scale.y) * button.image->GetHeight() * m_zoom
            };

            if (position.y <= 75.f) {
                continue;
            }


            ImGui::SetCursorPos(position);

            ImGui::Image(button.image->GetDescriptorSet(), size);
        }
    }
}

void SceneViewer::GenerateLightImage(LightObject& light)
{
    light.Updating();
    sf::Vector2u size = renderTexture.getSize();
    if (light.type == 0) {
        Micro::RadialLight radialLight(light.name.c_str(), 0);
        radialLight.SetRange(light.radius);
        radialLight.SetColor(sf::Color(static_cast<int>(light.color.x), static_cast<int>(light.color.y), static_cast<int>(light.color.z)));
        radialLight.SetIntensity((light.color.w + 0.3 > 1)? 1.0f : light.color.w + 0.3);
        radialLight.setPosition(size.x / 2.f, size.y / 2.f);
        radialLight.setRotation(light.rotation);
        radialLight.SetFade(light.fade);

        if (light.beamAngle < 359.9f)
            radialLight.SetBeamAngle(light.beamAngle);

        std::vector<ls::Line> vec;
        radialLight.CastLight(vec.begin(), vec.end());

        renderTexture.clear(sf::Color::Transparent);
        renderTexture.draw(radialLight);
        renderTexture.display();
    }
    else if (light.type == 1) {
        Micro::DirectedLight directedLight(light.name.c_str(), 0);
        directedLight.SetBeamWidth(light.width);
        directedLight.SetColor(sf::Color(static_cast<int>(light.color.x 
            ), static_cast<int>(light.color.y), static_cast<int>(light.color.z
                )));
        directedLight.SetIntensity((light.color.w + 0.3 > 1) ? 1.0f : light.color.w + 0.3);
        directedLight.setPosition(size.x / 2.f, size.y / 2.f);
        directedLight.setRotation(light.rotation);
        directedLight.SetRange(light.radius);
        directedLight.SetFade(light.fade);

        std::vector<ls::Line> vec;
        directedLight.CastLight(vec.begin(), vec.end());

        renderTexture.clear(sf::Color::Transparent);
        renderTexture.draw(directedLight);
        renderTexture.display();
    }
    
    sf::Image image = renderTexture.getTexture().copyToImage();
    const sf::Uint8* pixels = image.getPixelsPtr();
    size_t dataSize = image.getSize().x * image.getSize().y * 4; 

    
    light.imageData = std::shared_ptr<uint8_t[]>(new uint8_t[dataSize], std::default_delete<uint8_t[]>());
    std::memcpy(light.imageData.get(), pixels, dataSize);

    light.image = std::make_shared<Walnut::Image>(
        image.getSize().x,
        image.getSize().y,
        Walnut::ImageFormat::RGBA,
        light.imageData.get()
    );

}

void SceneViewer::GenerateTextImage(TextObject& text)
{
    sf::Vector2u size = renderTexture.getSize();
    //create font
    sf::Font font;
    font.loadFromFile(m_mainPath + "\\fonts\\" + text.font + ".ttf");

    //create text and set all the data
    sf::Text sText;
    sText.setFont(font);
    sText.setString(text.value);
    sText.setCharacterSize(text.size);
    sText.setOutlineThickness(text.outlineThickness);
    sText.setRotation(text.rotation);
    sText.setColor(sf::Color(text.color.x, text.color.y, text.color.z));
    sText.setOutlineColor(sf::Color(text.outlineColor.x, text.outlineColor.y, text.outlineColor.z));
    sText.setPosition(size.x / 2, size.y / 2);

    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(sText);
    renderTexture.display();

    sf::Image image = renderTexture.getTexture().copyToImage();
    const sf::Uint8* pixels = image.getPixelsPtr();
    size_t dataSize = image.getSize().x * image.getSize().y * 4;


    text.imageData = std::shared_ptr<uint8_t[]>(new uint8_t[dataSize], std::default_delete<uint8_t[]>());
    std::memcpy(text.imageData.get(), pixels, dataSize);

    text.image = std::make_shared<Walnut::Image>(
        image.getSize().x,
        image.getSize().y,
        Walnut::ImageFormat::RGBA,
        text.imageData.get()
    );
}

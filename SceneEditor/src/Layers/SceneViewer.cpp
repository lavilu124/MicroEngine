#include "SceneViewer.h"
#include <filesystem>
#include <iostream>

#include "../fileManage/FileManage.h"

SceneViewer::SceneViewer(const std::shared_ptr<SceneContent>& sceneContent, const char* mainPath) : sceneContent(sceneContent), m_mainPath(mainPath)
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
    constexpr float buttonWidth = 16.7f;
    constexpr float buttonHeight = 16.7f;

    ImVec2 buttonPos((contentRegion.x - buttonWidth) / 2.0f, 0.0f);

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4 originalBgColor = style.Colors[ImGuiCol_ChildBg];
    ImVec4 originalBorderColor = style.Colors[ImGuiCol_Border];
    float originalBorderSize = style.WindowBorderSize;
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0, 0, 0, 0);
    style.Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);
    style.WindowBorderSize = 0.0f;

    ImGui::BeginChild("##header", ImVec2(contentRegion.x, buttonHeight * 1.75));

    ImGui::SetCursorPos({ 0.0f, 0.0f });
    if (ImGui::ImageButton(m_saveButtonImage->GetDescriptorSet(), ImVec2(buttonWidth, buttonHeight )))
    {
        if (sceneContent->GetCurrentScene().empty()) {
            //open save window
            m_saving = true;
        }
        else {
            FileManage::SaveScene(sceneContent->GetCurrentScene(), sceneContent.get());
        }
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImGui::BeginTooltip();
        ImGui::Text("%s", "Save");
        ImGui::EndTooltip();
    }
    

    ImGui::SetCursorPos(buttonPos);
    if (ImGui::ImageButton(m_playButtonImage->GetDescriptorSet(), ImVec2(buttonWidth, buttonHeight)))
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

void SceneViewer::ExecutePlayCommand()
{
    std::string command = m_mainPath;
    

    if (command.find("\\Resources") != std::string::npos)
        command = command.substr(0, command.find_last_of('\\'));
    std::string command2 = command;

    command += "\\binaries\\windows-x86_64\\Debug\\Game\\Game.exe";
    command2 += "\\binaries\\windows-x86_64\\Release\\Game\\Game.exe";
    
    
    

    auto originalPath = std::filesystem::current_path();
    
    

    if (std::filesystem::exists(command)) {
        std::string workingDir = command.substr(0, command.find_last_of('\\'));
        // Save the current path
        

        // Change to the game's directory
        std::filesystem::current_path(workingDir);

        /*std::string scene = m_sceneContent->GetCurrentScene();
        scene = scene.substr(scene.find_last_of('\\') + 1, scene.size() - scene.find_last_of('\\') - 9);
        command += " ";
        command += scene;*/



        system("Game.exe");

        m_doneWithRun = true;
    }
    else if (std::filesystem::exists(command2)) {
        std::string workingDir = command2.substr(0, command2.find_last_of('\\'));
        // Save the current path


        // Change to the game's directory
        std::filesystem::current_path(workingDir);

        /*std::string scene = m_sceneContent->GetCurrentScene();
        scene = scene.substr(scene.find_last_of('\\') + 1, scene.size() - scene.find_last_of('\\') - 9);
        command2 += " ";
        command2 += scene;*/



        system("Game.exe");
    }

    std::filesystem::current_path(originalPath);


}

bool SceneViewer::DoneRun()
{
    if (m_doneWithRun)
    {
        m_doneWithRun = false;
		return  true;
    }

	return false;
}

void SceneViewer::Window()
{
	if (!m_isOpen) return;
    ImGuiIO& io = ImGui::GetIO();
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S))
    {
        if (sceneContent->GetCurrentScene().empty()) {
            m_saving = true; // open save dialog
        }
        else {
            FileManage::SaveScene(sceneContent->GetCurrentScene(), sceneContent.get());
        }
    }

    ImGui::Begin("Scene Viewer", &m_isOpen, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
    if (m_saving)
        SaveWindow();

    ImVec2 mousePos = io.MousePos;
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    ImVec2 relativeMousePos = { mousePos.x - cursorPos.x, mousePos.y - cursorPos.y };

    ImVec2 contentRegion = ImGui::GetContentRegionAvail();
    // Zoom (scroll wheel)
    if (ImGui::IsWindowHovered() && io.MouseWheel != 0.0f)
    {
        float zoomFactor = 1.1f;
        float oldZoom = m_zoom;
        m_zoom *= (io.MouseWheel > 0) ? zoomFactor : (1.0f / zoomFactor);

        // Mouse position relative to the scene center
        ImVec2 sceneCenter = ImVec2(contentRegion.x / 2.0f, contentRegion.y / 2.0f);
        ImVec2 focusPoint = ImVec2(relativeMousePos.x - sceneCenter.x, relativeMousePos.y - sceneCenter.y);

        m_offset.x -= focusPoint.x * (1.0f - m_zoom / oldZoom);
        m_offset.y -= focusPoint.y * (1.0f - m_zoom / oldZoom);
    }

    // Panning (mouse drag)
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
    {
        ImVec2 delta = io.MouseDelta;
        m_offset.x += delta.x;
        m_offset.y += delta.y;
    }

    
    RenderHeader(contentRegion);
    
    ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + contentRegion.x, ImGui::GetCursorScreenPos().y + contentRegion.y), IM_COL32(0, 0, 0, 255));

    auto pos = ImGui::GetCursorScreenPos();

    RenderGameObjects(contentRegion);
    ImGui::SetCursorScreenPos(pos);

    RenderDarknessOverlay(contentRegion);
    ImGui::SetCursorScreenPos(pos);

    RenderLights(contentRegion);
    ImGui::SetCursorScreenPos(pos);

    renderButtons(contentRegion);
    ImGui::SetCursorScreenPos(pos);

	renderTexts(contentRegion);
    ImGui::SetCursorScreenPos(pos);


    RenderCameraBorder(contentRegion);


    ImGui::End();
}

void SceneViewer::SaveWindow()
{
    ImGui::Begin("Save");
    ImGui::Text("Save Scene");


    static char fileNameBuffer[128];

    ImGui::Indent();

    ImGui::Text("File Name:");
    ImGui::Indent();

    if (ImGui::InputText("##FileName", fileNameBuffer, sizeof(fileNameBuffer)))
    {
        m_saveName = fileNameBuffer;
    }

    ImGui::Unindent();

    if (ImGui::Button("Save"))
    {

        std::string fullPath = m_mainPath + "\\Scenes\\" + m_saveName + ".McScene";
        FileManage::SaveScene(fullPath, sceneContent.get());

        m_doneSaving = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("Close"))
    {
        m_saving = false;
        m_doneSaving = false;
        ImGui::End();
        return;
    }

    if (m_doneSaving)
        ImGui::Text("Scene saved successfully");

    ImGui::End();
}

void SceneViewer::RenderCameraBorder(ImVec2 contentRegion) const
{
    sf::Vector2f size = { 1920.0f / sceneContent->GetCam().zoom, 1080.0f / sceneContent->GetCam().zoom };
    ImVec2 drawOrigin = ImGui::GetCursorScreenPos();

    ImGui::PushClipRect(drawOrigin, ImVec2(drawOrigin.x + contentRegion.x, drawOrigin.y + contentRegion.y), true);
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Convert world space to screen space
    auto ToScreen = [&](sf::Vector2f worldPos) -> ImVec2 {
        sf::Vector2f scaledPos = sf::Vector2f(worldPos.x * m_zoom, worldPos.y * m_zoom) + sf::Vector2f(m_offset.x, m_offset.y);
        return ImVec2(drawOrigin.x + scaledPos.x + contentRegion.x / 2.0f,
            drawOrigin.y + scaledPos.y + contentRegion.y / 2.0f);
        };

    ImVec2 center = sceneContent->GetCam().position;
    float rotationDeg = sceneContent->GetCam().rotation;
    float rotationRad = rotationDeg * 3.14159265f / 180.0f;
    float cosA = std::cos(rotationRad);
    float sinA = std::sin(rotationRad);


    sf::Vector2f halfSize = { size.x / 2.0f - 1, size.y / 2.0f - 1 };

    
    sf::Vector2f corners[4] = {
        sf::Vector2f(-halfSize.x, -halfSize.y), 
        sf::Vector2f(halfSize.x, -halfSize.y), 
        sf::Vector2f(halfSize.x,  halfSize.y), 
        sf::Vector2f(-halfSize.x,  halfSize.y)  
    };

    // Apply rotation
    ImVec2 rotatedCorners[4];
    for (int i = 0; i < 4; ++i)
    {
        float x = corners[i].x;
        float y = corners[i].y;

        float rotatedX = cosA * x - sinA * y;
        float rotatedY = sinA * x + cosA * y;

        rotatedCorners[i] = ToScreen(sf::Vector2f(rotatedX + center.x, rotatedY + center.y));
    }

    const float dashLength = 5.0f;
    const float gapLength = 5.0f;
    const ImU32 color = IM_COL32(255, 255, 255, 255);

    auto DrawDashedLine = [&](ImVec2 p1, ImVec2 p2) {
        ImVec2 dir = ImVec2(p2.x - p1.x, p2.y - p1.y);
        float length = sqrtf(dir.x * dir.x + dir.y * dir.y);
        dir.x /= length;
        dir.y /= length;

        float current = 0.0f;
        while (current < length) {
            ImVec2 start = ImVec2(p1.x + dir.x * current, p1.y + dir.y * current);
            float endOffset = std::min(dashLength, length - current);
            ImVec2 end = ImVec2(start.x + dir.x * endOffset, start.y + dir.y * endOffset);
            drawList->AddLine(start, end, color, 1.0f);
            current += dashLength + gapLength;
        }
        };

    DrawDashedLine(rotatedCorners[0], rotatedCorners[1]);
    DrawDashedLine(rotatedCorners[1], rotatedCorners[2]);
    DrawDashedLine(rotatedCorners[2], rotatedCorners[3]);
    DrawDashedLine(rotatedCorners[3], rotatedCorners[0]);

    ImGui::PopClipRect();
}




void SceneViewer::RenderDarknessOverlay(ImVec2 contentRegion) const
{
    float darknessLevel = sceneContent->GetCam().darknessPrecent;

    // Convert to alpha [0, 255]
    int alpha = static_cast<int>((darknessLevel / 100.0f) * 255.0f);
    ImU32 overlayColor = IM_COL32(0, 0, 0, alpha);

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    drawList->AddRectFilled(pos,
        ImVec2(pos.x + contentRegion.x, pos.y + contentRegion.y),
        overlayColor);
}

void SceneViewer::RenderGameObjects(ImVec2 contentRegion) const
{
    ImVec2 drawOrigin = ImGui::GetCursorScreenPos();

    ImGui::PushClipRect(drawOrigin, ImVec2(drawOrigin.x + contentRegion.x, drawOrigin.y + contentRegion.y), true);

    for (auto& gameObject : sceneContent->GetGameObjects()) {
        if (gameObject.sprite && gameObject.isVisable) {
            sf::Vector2f scaledPos = sf::Vector2f(gameObject.position.x * m_zoom, gameObject.position.y * m_zoom) + sf::Vector2f(m_offset.x, m_offset.y);
            ImVec2 position = ImVec2(drawOrigin.x + scaledPos.x + contentRegion.x / 2, drawOrigin.y + scaledPos.y + contentRegion.y / 2);
            ImVec2 size = {
                gameObject.scale.x * gameObject.sprite->GetWidth() * m_zoom,
                gameObject.scale.y * gameObject.sprite->GetHeight() * m_zoom
            };

            position.x -= size.x / 2.0f;
            position.y -= size.y / 2.0f;

            ImGui::SetCursorScreenPos(position);
            ImGui::Image(gameObject.sprite->GetDescriptorSet(), size);
        }
    }

    ImGui::PopClipRect();
}

void SceneViewer::RenderLights(ImVec2 contentRegion)
{
    ImVec2 drawOrigin = ImGui::GetCursorScreenPos();

    ImGui::PushClipRect(drawOrigin, ImVec2(drawOrigin.x + contentRegion.x, drawOrigin.y + contentRegion.y), true);

    for (auto& light : sceneContent->GetLights()) {
        if (!light.isVisable)
            continue;

        if (!light.isUpdated()) {
            GenerateLightImage(light);
        }

        sf::Vector2f scaledPos = sf::Vector2f(light.position.x * m_zoom, light.position.y * m_zoom) + sf::Vector2f(m_offset.x, m_offset.y);
        ImVec2 position(drawOrigin.x + scaledPos.x + contentRegion.x / 2, drawOrigin.y + scaledPos.y + contentRegion.y / 2);

        ImVec2 size = {
            light.image->GetWidth() * m_zoom,
            light.image->GetHeight() * m_zoom
        };

        position.x -= size.x / 2.0f;
        position.y -= size.y / 2.0f;

        ImGui::SetCursorScreenPos(position);

        if (light.image) {
            ImVec4 brightTint = ImVec4(1.5f, 1.5f, 1.5f, 1.0f);
            ImGui::Image(light.image->GetDescriptorSet(), size, ImVec2(0, 0), ImVec2(1, 1), brightTint);
        }
    }


    ImGui::PopClipRect();
}

void SceneViewer::renderTexts(ImVec2 contentRegion) {
    ImVec2 drawOrigin = ImGui::GetCursorScreenPos();

    ImGui::PushClipRect(drawOrigin, ImVec2(drawOrigin.x + contentRegion.x, drawOrigin.y + contentRegion.y), true);

    for (auto& text : sceneContent->GetTexts()) {
        if (!text.isVisable)
            continue;

        if (!text.isUpdated()) {
            GenerateTextImage(text);
        }

        sf::Vector2f scaledPos = sf::Vector2f(text.position.x * m_zoom, text.position.y * m_zoom) + sf::Vector2f(m_offset.x, m_offset.y);
        ImVec2 position(drawOrigin.x + scaledPos.x + contentRegion.x / 2, drawOrigin.y + scaledPos.y + contentRegion.y / 2);

        ImVec2 size = {
            text.image->GetWidth() * m_zoom,
            text.image->GetHeight() * m_zoom
        };

        position.x -= size.x / 2.0f;
        position.y -= size.y / 2.0f;

        ImGui::SetCursorScreenPos(position);

        if (text.image)
            ImGui::Image(text.image->GetDescriptorSet(), size);
    }


    ImGui::PopClipRect();
}

void SceneViewer::renderButtons(ImVec2 contentRegion) const
{
    ImVec2 drawOrigin = ImGui::GetCursorScreenPos();

    ImGui::PushClipRect(drawOrigin, ImVec2(drawOrigin.x + contentRegion.x, drawOrigin.y + contentRegion.y), true);

    for (auto& button : sceneContent->GetButtons()) {
        if (button.image && button.isVisable) {
            sf::Vector2f scaledPos = sf::Vector2f(button.position.x * m_zoom, button.position.y * m_zoom) + m_offset;
            ImVec2 position = ImVec2(drawOrigin.x + scaledPos.x + contentRegion.x / 2, drawOrigin.y + scaledPos.y + contentRegion.y / 2);

            ImVec2 size = ImVec2(
                button.scale.x * button.image->GetWidth() * m_zoom,
                button.scale.y * button.image->GetHeight() * m_zoom
            );

            position.x -= size.x / 2.0f;
            position.y -= size.y / 2.0f;

            ImGui::SetCursorScreenPos(position);
            ImGui::Image(button.image->GetDescriptorSet(), size);
        }
    }

    ImGui::PopClipRect();
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


    if (light.image) {
    	light.image->~Image();
    }

    light.image = std::make_shared<Walnut::Image>(
        image.getSize().x,
        image.getSize().y,
        Walnut::ImageFormat::RGBA,
        light.imageData.get()
    );

}

void SceneViewer::GenerateTextImage(TextObject& text)
{
    text.Updating();
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

    // Center the text by adjusting the origin
    sf::FloatRect bounds = sText.getLocalBounds();
    sText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);

    // Set the position to center of the render texture
    sText.setPosition(size.x / 2.f, size.y / 2.f);

    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(sText);
    renderTexture.display();

    sf::Image image = renderTexture.getTexture().copyToImage();
    const sf::Uint8* pixels = image.getPixelsPtr();
    size_t dataSize = image.getSize().x * image.getSize().y * 4;


    text.imageData = std::shared_ptr<uint8_t[]>(new uint8_t[dataSize], std::default_delete<uint8_t[]>());
    std::memcpy(text.imageData.get(), pixels, dataSize);

    if (text.image) {
        text.image->~Image();
    }

    text.image = std::make_shared<Walnut::Image>(
        image.getSize().x,
        image.getSize().y,
        Walnut::ImageFormat::RGBA,
        text.imageData.get()
    );
}



#include "SceneViewer.h"
#include <filesystem>

#include <iostream>

SceneViewer::SceneViewer(std::shared_ptr<SceneContent> sceneContent) : m_sceneContent(sceneContent)
{
}

void SceneViewer::OnAttach()
{
	renderTexture.create(512, 512);
	renderTexture.clear(sf::Color::Transparent);

	char playButtonPath[] = "\\play.png";
	std::string playImagePath = std::filesystem::current_path().string() + playButtonPath;
	m_playButtonImage = std::make_shared<Walnut::Image>(playImagePath.c_str());
}

void SceneViewer::OnUIRender()
{
	Window();
}

void SceneViewer::RenderPlayButton(const ImVec2& contentRegion) {
	// Constants
	constexpr float buttonWidth = 25.0f;
	constexpr float buttonHeight = 25.0f;
	
	constexpr char executableRelativePath[] = "\\binaries\\windows-x86_64\\debug\\sendbox\\sendbox.exe Scene1";

	// Calculate button position for horizontal centering
	ImVec2 buttonPos = ImVec2(
		(contentRegion.x - buttonWidth) / 2.0f,
		0.0f // Vertical positioning, adjust as needed for additional content
	);
	
	

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 originalBgColor = style.Colors[ImGuiCol_ChildBg];
	ImVec4 originalBorderColor = style.Colors[ImGuiCol_Border];
	float originalBorderSize = style.WindowBorderSize;
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Transparent Background
	style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Transparent Border
	style.WindowBorderSize = 0.0f; // No Border

	// Begin child window for the button
	ImGui::BeginChild("##header", ImVec2(contentRegion.x, buttonHeight));
	{
		// Position cursor for the button
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + buttonPos.x);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + buttonPos.y);

		// Style adjustments for the button
		ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

		// Render the button (as an image)
		ImGui::Image(m_playButtonImage->GetDescriptorSet(), ImVec2(buttonWidth, buttonHeight));

		// Check for hover and click events
		if (ImGui::IsItemHovered()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}
		if (ImGui::IsItemClicked()) {
			// Construct command to execute, adjusting path if in SceneEditor
			std::string command = std::filesystem::current_path().string();
			if (command.find("SceneEditor") != std::string::npos) {
				command = command.substr(0, command.find_last_of('\\'));
			}
			command += executableRelativePath;

			system(command.c_str());
		}

		// Revert style changes
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}
	ImGui::EndChild(); // Ensure child window is properly ended
}

void SceneViewer::Window()
{
	ImGui::Begin("Scene Viewer");
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	RenderPlayButton(contentRegion);
	contentRegion = ImVec2(contentRegion.x, contentRegion.y);

	ImGui::GetWindowDrawList()->AddRectFilled(
		ImGui::GetCursorScreenPos(),
		ImVec2(ImGui::GetCursorScreenPos().x + contentRegion.x, ImGui::GetCursorScreenPos().y + contentRegion.y),
		IM_COL32(0, 0, 0, 255) // Black
	);

	RenderGameObjects();
	RenderLights();

	ImGui::End();
}

void ImageRotated(ImTextureID user_texture_id, const ImVec2& size, float angle, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
{
	// Convert angle to radians
	angle = angle * (3.14159265359f / 180);

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImDrawList* draw_list = window->DrawList;

	ImVec2 pos = window->DC.CursorPos; 
	ImVec2 center = ImVec2(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f); 

	// Precompute sine and cosine of the angle
	float cos_a = cosf(angle);
	float sin_a = sinf(angle);

	// Calculate rotated corners (relative to center)
	ImVec2 corners[4] = {
		ImVec2(-size.x * 0.5f, -size.y * 0.5f),
		ImVec2(+size.x * 0.5f, -size.y * 0.5f),
		ImVec2(+size.x * 0.5f, +size.y * 0.5f),
		ImVec2(-size.x * 0.5f, +size.y * 0.5f),
	};

	ImVec2 rotated[4];
	for (int i = 0; i < 4; ++i)
	{
		rotated[i].x = center.x + (corners[i].x * cos_a - corners[i].y * sin_a);
		rotated[i].y = center.y + (corners[i].x * sin_a + corners[i].y * cos_a);
	}

	// Now, calculate the UV coordinates for each corner
	ImVec2 uvCorners[4] = {
		ImVec2(uv0.x, uv0.y),
		ImVec2(uv1.x, uv0.y),
		ImVec2(uv1.x, uv1.y),
		ImVec2(uv0.x, uv1.y),
	};

	// Draw the image with rotation (using the rotated corners)
	draw_list->AddImageQuad(
		user_texture_id,
		rotated[0], rotated[1], rotated[2], rotated[3],
		uvCorners[0], uvCorners[1], uvCorners[2], uvCorners[3],
		ImGui::GetColorU32(tint_col)
	);

	// Optionally draw the border if the color is non-transparent
	if (border_col.w > 0.0f)
	{
		draw_list->AddQuad(
			rotated[0], rotated[1], rotated[2], rotated[3],
			ImGui::GetColorU32(border_col),
			1.0f
		);
	}

	// Advance the cursor position
	ImGui::Dummy(size); // Adds spacing so ImGui layout doesn't overlap
}

void SceneViewer::RenderGameObjects() const
{
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	for (auto& gameObject : m_sceneContent->GetGameObjects()) {
		if (gameObject.sprite) {
			ImVec2 position(
				gameObject.position.x + contentRegion.x / 2 - (float)gameObject.sprite->GetWidth() / 2,
				gameObject.position.y + contentRegion.y / 2 - (float)gameObject.sprite->GetHeight() / 2 + 25
			);

			
			ImGui::SetCursorPos(position);
			ImageRotated(
				gameObject.sprite->GetDescriptorSet(),
				ImVec2(gameObject.scale.x * (float)gameObject.sprite->GetWidth(), gameObject.scale.y * (float)gameObject.sprite->GetHeight()),
				gameObject.rotation,
				ImVec2(0, 0), 
				ImVec2(1, 1), 
				ImVec4(1, 1, 1, 1), 
				ImVec4(0, 0, 0, 0) 
			);
		}
	}
}

void SceneViewer::RenderLights()
{

	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	for (auto& light : m_sceneContent->GetLights()) {
		ImGui::SetCursorPos(ImVec2(
			light.position.x + contentRegion.x / 2 , 
			light.position.y + contentRegion.y / 2
		));

		if (light.image == nullptr) {
			Micro::RadialLight* lightVal = new Micro::RadialLight("", 0);
			lightVal->setRange(light.radius);
			lightVal->setColor(sf::Color::White);
			lightVal->setPosition(520 / 2, 520 / 2);

			renderTexture.draw(*lightVal);
			renderTexture.display();

			const sf::Texture& lightTexture = renderTexture.getTexture();

			lightTexture.copyToImage().saveToFile("temp.png");
			light.image = std::make_shared<Walnut::Image>("C:\\github\\MicroEngine\\SceneEditor\\temp.png");
			std::remove("temp.png");

			renderTexture.clear(sf::Color::Transparent);

			delete lightVal;
		}

		if (!light.isUpdated()) {
			Micro::RadialLight* lightVal = new Micro::RadialLight("", 0);
			lightVal->setRange(light.radius);

			int r = static_cast<int>(light.color.Value.x * 255.0f);
			int g = static_cast<int>(light.color.Value.y * 255.0f);
			int b = static_cast<int>(light.color.Value.z * 255.0f);
			lightVal->setColor(sf::Color(r,g,b));
			lightVal->setIntensity(light.color.Value.w);
			lightVal->setPosition(520 / 2, 520 / 2);

			renderTexture.draw(*lightVal);
			renderTexture.display();

			const sf::Texture& lightTexture = renderTexture.getTexture();

			lightTexture.copyToImage().saveToFile("temp.png");
			light.image = std::make_shared<Walnut::Image>("C:\\github\\MicroEngine\\SceneEditor\\temp.png");
			std::remove("temp.png");
			
			renderTexture.clear(sf::Color::Transparent);
			delete lightVal;
			light.Updating();
		}

		ImGui::Image(
			light.image->GetDescriptorSet(),
			ImVec2(512, 512)
		);
	}
}

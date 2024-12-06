#include "SceneViewer.h"
#include <filesystem>


#define HEADERHEIGHT 25
SceneViewer::SceneViewer(std::shared_ptr<SceneContent> sceneContent) : m_sceneContent(sceneContent)
{
	ls.setView(view);
}

void SceneViewer::OnUIRender()
{
	Window();
}

ImageData SceneViewer::ExtractImageData(const sf::Sprite& sprite) const {

	if (!sprite.getTexture()) {
		return {0, 0, nullptr};
	}

	sf::Image image = sprite.getTexture()->copyToImage();

	return { image.getSize().x ,image.getSize().y, image.getPixelsPtr()};
}
void SceneViewer::SetImage(LightObject& light)
{
	if (light.counter == 1)
	{
		sf::Color color = sf::Color(light.color.Value.x * 255, light.color.Value.y * 255, light.color.Value.z * 255, light.color.Value.w * 255);
		Micro::ls::SpotLight spotLight = Micro::ls::SpotLight(sf::Vector2f(0, 0), light.radius, color, light.name, 0);
		spotLight.setActive(true);
		spotLight.setSpreadAngle(35);
		spotLight.setDirectionAngle(90);
		ls.addLight(&spotLight);
		ImageData imageData = ExtractImageData(spotLight.getSprite());
		light.image = std::make_shared<Walnut::Image>(imageData.width, imageData.height, Walnut::ImageFormat::RGBA, imageData.data);
		return;
	}

	light.counter++;
}

void SceneViewer::RenderPlayButton(const ImVec2& contentRegion) {
	// Constants
	constexpr float buttonWidth = 25.0f;
	constexpr float buttonHeight = 25.0f;
	constexpr char playButtonPath[] = "\\play.png";
	constexpr char executableRelativePath[] = "\\binaries\\windows-x86_64\\debug\\sendbox\\sendbox.exe";

	// Calculate button position for horizontal centering
	ImVec2 buttonPos = ImVec2(
		(contentRegion.x - buttonWidth) / 2.0f,
		0.0f // Vertical positioning, adjust as needed for additional content
	);

	// Path to play button image
	std::string playImagePath = std::filesystem::current_path().string() + playButtonPath;
	std::shared_ptr<Walnut::Image> playButtonImage = std::make_shared<Walnut::Image>(playImagePath.c_str());

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 originalBgColor = style.Colors[ImGuiCol_ChildBg];
	ImVec4 originalBorderColor = style.Colors[ImGuiCol_Border];
	float originalBorderSize = style.WindowBorderSize;
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Transparent Background
	style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Transparent Border
	style.WindowBorderSize = 0.0f; // No Border

	// Begin child window for the button
	ImGui::BeginChild("##header", ImVec2(contentRegion.x, HEADERHEIGHT));
	{
		// Position cursor for the button
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + buttonPos.x);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + buttonPos.y);

		// Style adjustments for the button
		ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

		// Render the button (as an image)
		ImGui::Image(playButtonImage->GetDescriptorSet(), ImVec2(buttonWidth, buttonHeight));

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


	ImGui::BeginChild("#scene", ImVec2(contentRegion.x,contentRegion.y-HEADERHEIGHT));
	{
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImGui::GetCursorScreenPos(),
			ImVec2(ImGui::GetCursorScreenPos().x + contentRegion.x, ImGui::GetCursorScreenPos().y + contentRegion.y),
			IM_COL32(0, 0, 0, 255) // Black
		);

		RenderGameObjects();
		//RenderLights();
	}
	ImGui::EndChild();

	ImGui::End();
}

void SceneViewer::ImageRotated(ImTextureID user_texture_id, const ImVec2& size, float angle, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col) const
{
	// Convert angle to radians for trigonometric functions
	float angleRad = angle * (3.14159f / 180.0f);

	// Calculate the new bounding box to account for rotation
	ImVec2 rotatedSize = size;
	if (angle != 0.0f && fmod(angle, 90.0f) != 0.0f) // Use fmod for floating-point modulo
	{
		float newSizeX = size.x * std::abs(std::cos(angleRad)) + size.y * std::abs(std::sin(angleRad));
		float newSizeY = size.x * std::abs(std::sin(angleRad)) + size.y * std::abs(std::cos(angleRad));
		rotatedSize = ImVec2(std::max(newSizeX, newSizeY), std::max(newSizeX, newSizeY)); // Ensure bounding box can fully contain the rotated image
	}

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImRect bb(window->DC.CursorPos, ImVec2(window->DC.CursorPos.x + rotatedSize.x, window->DC.CursorPos.y + rotatedSize.y));
	if (border_col.w > 0.0f)
		bb.Max = ImVec2(bb.Max.x + 2, bb.Max.y + 2);
	ImGui::ItemSize(bb);
	if (!ImGui::ItemAdd(bb, 0))
		return;

	// Define UV coordinates for the original image (no need to rotate these)
	ImVec2 _uv0 = uv0;
	ImVec2 _uv1 = ImVec2(uv1.x, uv0.y);
	ImVec2 _uv2 = uv1;
	ImVec2 _uv3 = ImVec2(uv0.x, uv1.y);

	// Calculate vertex positions for the rotated image
	ImVec2 center = ImVec2(size.x / 2, size.y / 2);
	ImVec2 x0, x1, x2, x3;

	// Rotate points around the center (using original UVs as if they were positions for simplicity)
	x0 = ImVec2(center.x + (uv0.x - 0.5f) * size.x * std::cos(angleRad) - (uv0.y - 0.5f) * size.y * std::sin(angleRad),
		center.y + (uv0.x - 0.5f) * size.x * std::sin(angleRad) + (uv0.y - 0.5f) * size.y * std::cos(angleRad));
	x1 = ImVec2(center.x + (uv1.x - 0.5f) * size.x * std::cos(angleRad) - (uv1.y - 0.5f) * size.y * std::sin(angleRad),
		center.y + (uv1.x - 0.5f) * size.x * std::sin(angleRad) + (uv1.y - 0.5f) * size.y * std::cos(angleRad));
	x2 = ImVec2(center.x + (_uv2.x - 0.5f) * size.x * std::cos(angleRad) - (_uv2.y - 0.5f) * size.y * std::sin(angleRad),
		center.y + (_uv2.x - 0.5f) * size.x * std::sin(angleRad) + (_uv2.y - 0.5f) * size.y * std::cos(angleRad));
	x3 = ImVec2(center.x + (_uv3.x - 0.5f) * size.x * std::cos(angleRad) - (_uv3.y - 0.5f) * size.y * std::sin(angleRad),
		center.y + (_uv3.x - 0.5f) * size.x * std::sin(angleRad) + (_uv3.y - 0.5f) * size.y * std::cos(angleRad));

	// Adjust vertex positions based on the window's cursor position and potential border
	ImVec2 offset = (border_col.w > 0.0f) ? ImVec2(1, 1) : ImVec2(0, 0);
	x0 = ImVec2(bb.Min.x + (x0.x - center.x) + center.x * (rotatedSize.x / size.x) + offset.x, bb.Min.y + (x0.y - center.y) + center.y * (rotatedSize.y / size.y) + offset.y);
	x1 = ImVec2(bb.Min.x + (x1.x - center.x) + center.x * (rotatedSize.x / size.x) + offset.x, bb.Min.y + (x1.y - center.y) + center.y * (rotatedSize.y / size.y) + offset.y);
	x2 = ImVec2(bb.Min.x + (x2.x - center.x) + center.x * (rotatedSize.x / size.x) + offset.x, bb.Min.y + (x2.y - center.y) + center.y * (rotatedSize.y / size.y) + offset.y);
	x3 = ImVec2(bb.Min.x + (x3.x - center.x) + center.x * (rotatedSize.x / size.x) + offset.x, bb.Min.y + (x3.y - center.y) + center.y * (rotatedSize.y / size.y) + offset.y);

	if (border_col.w > 0.0f)
	{
		window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(border_col), 0.0f);
		window->DrawList->AddImageQuad(user_texture_id, x0, x1, x2, x3, _uv0, _uv1, _uv2, _uv3, ImGui::GetColorU32(tint_col));
	}
	else
	{
		window->DrawList->AddImageQuad(user_texture_id, x0, x1, x2, x3, _uv0, _uv1, _uv2, _uv3, ImGui::GetColorU32(tint_col));
	}
}
void SceneViewer::RenderGameObjects() const
{
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	for (auto& gameObject : m_sceneContent->GetGameObjects()) {
		if (gameObject.sprite) {
			ImVec2 position(
				gameObject.position.x + contentRegion.x / 2 - (float)gameObject.sprite->GetWidth() / 2,
				gameObject.position.y + contentRegion.y / 2 - (float)gameObject.sprite->GetHeight() / 2 - HEADERHEIGHT
			);

			
			int rotation = round(gameObject.rotation / 90.0f) * 90;

			
			ImGui::SetCursorPos(position);
			ImageRotated(
				gameObject.sprite->GetDescriptorSet(),
				ImVec2(gameObject.scale.x * (float)gameObject.sprite->GetWidth(), gameObject.scale.y * (float)gameObject.sprite->GetHeight()),
				rotation,
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
			light.position.x + contentRegion.x / 2 - (float)light.radius, 
			light.position.y + contentRegion.y / 2 - (float)light.radius
		));
		if (light.type == 0)
		{
			if (light.image != nullptr)
			{
				ImGui::Image(light.image->GetDescriptorSet(), { (float)light.image->GetWidth(), (float)light.image->GetHeight() });
				break;
			}
			SetImage(light);
		}
	}
}

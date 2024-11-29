#include "SceneViewer.h"


SceneViewer::SceneViewer(std::shared_ptr<SceneContent> sceneContent) : m_sceneContent(sceneContent)
{
	ls.setView(view);
}

void SceneViewer::OnUIRender()
{
	Window();
}

ImageData extractImageData(const sf::Sprite& sprite) {

	if (!sprite.getTexture()) {
		return {0, 0, nullptr};
	}

	sf::Image image = sprite.getTexture()->copyToImage();

	return { image.getSize().x ,image.getSize().y, image.getPixelsPtr()};
}
void SceneViewer::setImage(LightObject& light)
{
	if (light.counter == 1)
	{
		sf::Color color = sf::Color(light.color.Value.x * 255, light.color.Value.y * 255, light.color.Value.z * 255, light.color.Value.w * 255);
		Micro::ls::SpotLight spotLight = Micro::ls::SpotLight(sf::Vector2f(0, 0), light.radius, color, light.name, 0);
		spotLight.setActive(true);
		spotLight.setSpreadAngle(35);
		spotLight.setDirectionAngle(90);
		ls.addLight(&spotLight);
		ImageData imageData = extractImageData(spotLight.getSprite());
		light.image = std::make_shared<Walnut::Image>(imageData.width, imageData.height, Walnut::ImageFormat::RGBA, imageData.data);
		return;
	}

	light.counter++;
}



void SceneViewer::Window()
{
	ImGui::Begin("Scene Viewer");
	//if (ImGui::Button("run game")) {
	//	std::string command = "c:\\github\\microengine\\binaries\\windows-x86_64\\debug\\sendbox\\sendbox.exe"; // assuming the exe is in the same directory
	//	system(command.c_str());
	//}

	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	ImGui::GetWindowDrawList()->AddRectFilled(
		ImGui::GetCursorScreenPos(),
		ImVec2(ImGui::GetCursorScreenPos().x + contentRegion.x, ImGui::GetCursorScreenPos().y + contentRegion.y),
		IM_COL32(0, 0, 0, 255) // Black
	);

	RenderGameObjects();
	RenderLights();

	ImGui::End();
}

void SceneViewer::RenderGameObjects() const
{
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	for (auto& gameObject : m_sceneContent->GetGameObjects()) {
		if (gameObject.sprite) {
			ImGui::SetCursorPos(ImVec2(
				gameObject.position.x + contentRegion.x / 2 - (float)gameObject.sprite->GetWidth() / 2, 
				gameObject.position.y + contentRegion.y / 2 - (float)gameObject.sprite->GetHeight() / 2
			));
			ImGui::Image(gameObject.sprite->GetDescriptorSet(), { gameObject.scale.x * (float)gameObject.sprite->GetWidth(), gameObject.scale.y * (float)gameObject.sprite->GetHeight() });
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
			setImage(light);
		}
	}
}

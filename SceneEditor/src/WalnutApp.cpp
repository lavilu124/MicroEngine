#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/UI/UI.h"

#include "Layers/SceneViewer.h"
#include "Layers/Saves.h"

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Micro Engine";
	spec.CustomTitlebar = true;
	

	Walnut::Application* app = new Walnut::Application(spec);

	std::shared_ptr<ProjectDirectory> dir =  std::make_shared<ProjectDirectory>(R"(C:\github\MicroEngine\Resources)");
	app->PushLayer(dir);

	std::shared_ptr<ObjectViewer> viewer =  std::make_shared<ObjectViewer>(dir);
	app->PushLayer(viewer);

	std::shared_ptr<SceneContent> sceneContent = std::make_shared<SceneContent>(viewer);
	app->PushLayer(sceneContent);

	std::shared_ptr<SceneViewer> sceneViewer = std::make_shared<SceneViewer>(sceneContent);
	app->PushLayer(sceneViewer);

	std::shared_ptr<Saves> saves = std::make_shared<Saves>(Saves(sceneContent));
	app->PushLayer(saves);


	app->SetMenubarCallback([app, saves]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save")) {
				saves->Save();
			}

			if (ImGui::MenuItem("Load")) {
				saves->Load();
			}

			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}
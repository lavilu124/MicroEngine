#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/UI/UI.h"

#include "Layers/SceneViewer.h"
#include "Layers/Saves.h"

#include <filesystem>
#include <GLFW/glfw3.h>

void OnFileDrop(GLFWwindow* window, int count, const char** paths)
{
	for (int i = 0; i < count; i++)
	{
		ProjectDirectory::UploadFile(paths[i]);
	}
}

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	//spec if the app
	Walnut::ApplicationSpecification spec;
	spec.Name = "Micro Engine";
	spec.CustomTitlebar = true;
	
	//the app
	Walnut::Application* app = new Walnut::Application(spec);
	

	std::string path = std::filesystem::current_path().string();
	if (path.find("SceneEditor") != std::string::npos)
		path = path.substr(0, path.find_last_of('\\'));
	path += "\\Resources";

	//add the layers (sub-windows) 
	std::shared_ptr<ProjectDirectory> dir =  std::make_shared<ProjectDirectory>(path);
	app->PushLayer(dir);
	glfwSetDropCallback(static_cast<GLFWwindow*>(app->GetWindowHandle()), OnFileDrop);

	std::shared_ptr<ObjectViewer> viewer =  std::make_shared<ObjectViewer>(dir);
	app->PushLayer(viewer);

	std::shared_ptr<SceneContent> sceneContent = std::make_shared<SceneContent>(viewer, dir);
	app->PushLayer(sceneContent);

	std::shared_ptr<SceneViewer> sceneViewer = std::make_shared<SceneViewer>(sceneContent, path.c_str());
	app->PushLayer(sceneViewer);

	/*std::shared_ptr<Saves> saves = std::make_shared<Saves>(Saves(sceneContent));
	app->PushLayer(saves);

	//menus
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
	});*/


	return app;
}
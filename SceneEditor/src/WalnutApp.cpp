#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/UI/UI.h"
#include "Layers/ProjectSelector.h"

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
    ApplicationSpecification spec;
    spec.Name = "Micro Engine";
    spec.CustomTitlebar = true;
	spec.Height = 600;
	spec.Width = 800;


	//set to icon path
    spec.IconPath = "appGUI\\playIcon.png";

    Application* app = new Application(spec);
    app->PushLayer(std::make_shared<ProjectSelector>(app));


    return app;
}

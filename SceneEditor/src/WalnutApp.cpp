#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/UI/UI.h"
#include "Layers/ProjectSelector.h"

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
    Walnut::ApplicationSpecification spec;
    spec.Name = "Micro Engine";
    spec.CustomTitlebar = true;

    Walnut::Application* app = new Walnut::Application(spec);
    app->PushLayer(std::make_shared<ProjectSelector>(app));

    return app;
}

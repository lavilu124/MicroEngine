#include "ObjectViewer.h"

template <class T>
T min(T x, T y)
{
	return (x < y)? x : y;
}

ObjectViewer::ObjectViewer(std::shared_ptr<ProjectDirectory> projectDirectory) : m_ProjectDirectory(projectDirectory)
{
}

void ObjectViewer::OnUIRender()
{
	Window();
}

void ObjectViewer::Window()
{
	ImGui::Begin("Object Viewer");
	
	if (currentObject == nullptr) {
		ImGui::Text("No Object Was Selected");
	}
	else {
		switch (m_CurrentObjectType) {
		case currentObjectType::game:
			DisplayGameObject();
			break;
		case currentObjectType::light:
			DisplayLightObject();
		}
	}



	ImGui::End();
}

void ObjectViewer::DisplayGameObject() {
	ImGui::Text("Object Details:");
	ImGui::Separator();

	GameObject* GameOj = (GameObject*)currentObject;

	ImGui::Text("Name:");
	ImGui::Indent();

	static char nameBuffer[128];
	strcpy_s(nameBuffer, sizeof(nameBuffer), currentObject->name.c_str());
	if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer)))
	{
		currentObject->name = nameBuffer;
	}

	ImGui::Text("Position:");
	ImGui::Indent();

	float positionValues[2] = { currentObject->position.x, currentObject->position.y };
	if (ImGui::InputFloat2("##Position", positionValues))
	{
		currentObject->position.x = positionValues[0];
		currentObject->position.y = positionValues[1];
	}
	ImGui::Unindent();

	ImGui::Text("Rotation (Degrees):");
	ImGui::Indent();
	if (ImGui::InputFloat("##Rotation", &currentObject->rotation))
	{
		currentObject->rotation = min<float>(currentObject->rotation, 360.0f);
	}
	ImGui::Unindent();

	ImGui::Text("Scale:");
	ImGui::Indent();

	float scaleValues[2] = { GameOj->scale.x, GameOj->scale.y };
	if (ImGui::InputFloat2("##Scale", scaleValues))
	{
		GameOj->scale.x = scaleValues[0];
		GameOj->scale.y = scaleValues[1];
	}
	ImGui::Unindent();
	

	ImGui::Text("Sprite:");

	if (ImGui::IsItemHovered() && m_ProjectDirectory->GetSelectedPath() != "" && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
		// If user drops the path by clicking
		GameOj->SetPath(m_ProjectDirectory->GetSelectedPath());
	}
	m_ProjectDirectory->ClearSelectedPath();
	// Visual indicator for drag-and-drop
	if (ImGui::IsItemHovered()) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}
	
	ImGui::Indent();
	try {
		if (GameOj->sprite)
			ImGui::Image(GameOj->sprite->GetDescriptorSet(), { (float)GameOj->sprite->GetWidth(), (float)GameOj->sprite->GetHeight() });
	}
	catch (std::exception) {

	}
	
	ImGui::Unindent();
}

void ObjectViewer::DisplayLightObject() {
	ImGui::Text("Object Details:");
	ImGui::Separator();

	LightObject* Light = (LightObject*)currentObject;

	ImGui::Text("Name:");
	ImGui::Indent();

	static char nameBuffer[128];
	strcpy_s(nameBuffer, sizeof(nameBuffer), currentObject->name.c_str());
	if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer)))
	{
		currentObject->name = nameBuffer;
	}

	ImGui::Text("Position:");
	ImGui::Indent();

	float positionValues[2] = { currentObject->position.x, currentObject->position.y };
	if (ImGui::InputFloat2("##Position", positionValues))
	{
		currentObject->position.x = positionValues[0];
		currentObject->position.y = positionValues[1];
	}
	ImGui::Unindent();

	ImGui::Text("Rotation (Degrees):");
	ImGui::Indent();
	if (ImGui::InputFloat("##Rotation", &currentObject->rotation))
	{
		currentObject->rotation = min<float>(currentObject->rotation, 360.0f);
	}
	ImGui::Unindent();

	ImGui::Text("Color:");
	ImGui::Indent();
	float colorValues[3] = { Light->color.Value.x, Light->color.Value.y, Light->color.Value.z };
	if (ImGui::ColorEdit3("##Color", colorValues))
	{
		Light->color.Value.x = colorValues[0];
		Light->color.Value.y = colorValues[1];
		Light->color.Value.z = colorValues[2];
		Light->UpdateVal();
	}
	ImGui::Unindent();

	ImGui::Text("Intensity:");
	ImGui::Indent();
	float dispaledInt = Light->color.Value.w * 255.0f;
	if (ImGui::InputFloat("##Intensity", &Light->color.Value.w))
	{
		Light->color.Value.w = min<float>(Light->color.Value.w, 255.0f) / 255.0f;
		Light->UpdateVal();
	}
	ImGui::Unindent();

	//ImGui::Text("Type:");
	/*ImGui::Indent();
	if (ImGui::RadioButton("Spot", Light->type == 0)) {
		Light->type = 0;
		Light->UpdateVal();
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Flash", Light->type == 1)) {
		Light->type = 1;
		Light->UpdateVal();
	}
	ImGui::Unindent();*/

	/*if (Light->type == 0)
	{
		ImGui::Text("Angle");
		ImGui::Indent();
		if (ImGui::InputFloat("##Angle"))
		{
			
		}
		ImGui::Unindent();
	}*/

	ImGui::Text("Radius");
	ImGui::Indent();
	float displayRad = Light->radius * 1.5;
	if (ImGui::InputFloat("##Radius", &displayRad))
	{
		Light->radius = min<float>(Light->radius, 512.0f) / 1.5;
		Light->UpdateVal();
	}
	ImGui::Unindent();

	ImGui::Text("BeamAngle: ");
	ImGui::Indent();
	if (ImGui::InputFloat("##BeamAngle", &Light->beamAngle))
	{
		Light->beamAngle = min<float>(Light->beamAngle, 360);
		Light->UpdateVal();
	}
	ImGui::Unindent();
}

void ObjectViewer::SetObject(Object* newObject, currentObjectType type)
{
	currentObject = newObject;
	m_CurrentObjectType = type;
}

Object* ObjectViewer::GetObject() const
{
	return currentObject;
}

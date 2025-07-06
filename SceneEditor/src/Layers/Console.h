#pragma once

#include "SceneViewer.h"

class Console : public Walnut::Layer
{
	public:
		Console(const std::shared_ptr<SceneViewer>& sceneViewer);


		void OnUIRender() override;

		bool* Open() { return &m_isOpen; }

private:
	void ReadConsole();


	bool m_isOpen = true;

	std::shared_ptr<SceneViewer> m_sceneViewer;

	bool m_logLoaded = false;

	std::vector<std::string> m_logLines;

};


#pragma once
#include <string>
#include "../Layers/SceneContent.h"

class FileManage
{
public:
	static void SaveScene(std::string path, SceneContent* content);
};


#pragma once

#include "Ogre.h"

#include <string>

class ResourceManager
{
public:
	ResourceManager(const std::string& strResourceRoot);
	~ResourceManager();

	void LoadOgreResources(std::string strConfigPath);

private:
	void LoadOgreHlms(Ogre::ConfigFile& cf);

	std::string m_strResourceRoot;
};


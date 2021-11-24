#pragma once

#include <string>
#include "Ogre.h"

class EntityManager;

class LoadingSystem
{
public:
	LoadingSystem(EntityManager* pEntityManager, const std::string& scriptsRoot);

	~LoadingSystem();
	void LoadFromXML(const std::string fileName);
	void SaveToXML(const std::string fileName);
private:
	EntityManager* m_pEntityManager;
	std::string m_strSavesRootPath;

	Ogre::Vector3 ParsePosition(const char* strPosition);
};


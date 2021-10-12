#include "LoadingSystem.h"
#include "../../../SDKs/Ogre/include/OGRE/OgreSceneManager.h"
#include "../../tinyxml/tinyxml.h"

LoadingSystem::LoadingSystem()
{

}

void LoadingSystem::LoadFromXML(const std::string fileName)
{
    TiXmlDocument doc(fileName.c_str());

    if (doc.LoadFile())
    {
        TiXmlElement* element = doc.FirstChildElement("meshName");
        std::string mesh = element->GetText();
    }
}

void LoadingSystem::SaveToXML(const std::string fileName)
{

}
LoadingSystem::~LoadingSystem()
{

};
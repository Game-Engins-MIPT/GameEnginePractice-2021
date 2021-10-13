#include "LoadingSystem.h"
#include "../EntityManager.h"
#include "../../../SDKs/Ogre/include/OGRE/OgreSceneManager.h"
#include "../../tinyxml/tinyxml.h"

LoadingSystem::LoadingSystem(EntityManager *pEntityManager, const std::string &scriptsRoot)
    :m_pEntityManager(pEntityManager),
    m_strSavesRootPath(scriptsRoot)
{
    
}

void LoadingSystem::LoadFromXML(const std::string fileName)
{
    const auto pathName = m_strSavesRootPath + fileName;

    TiXmlDocument doc(pathName.c_str());

    if (doc.LoadFile())
    {
        const auto elem = doc.FirstChildElement("scene");
        for (TiXmlElement* e = elem->FirstChildElement("character"); e != NULL; e = e->NextSiblingElement("character"))
        {
            Spawn currentCharacter;
            TiXmlElement* meshElement = e->FirstChildElement("meshName");
            currentCharacter.meshName = meshElement->GetText();

            TiXmlElement* scriptElement = e->FirstChildElement("scriptName");
            currentCharacter.scriptName = scriptElement->GetText();

            m_pEntityManager->CreateEntity(currentCharacter);
        }
    }
}


void LoadingSystem::SaveToXML(const std::string fileName)
{
    const auto pathName = m_strSavesRootPath + fileName;
    TiXmlDocument doc(pathName.c_str());
    const auto elem = doc.FirstChildElement("scene");
    auto entityQueue = m_pEntityManager->GetEntityQueue();

    for (auto& entity : entityQueue)
    {
        for (TiXmlElement* e = elem->FirstChildElement("character"); e != NULL; e = e->NextSiblingElement("character"))
        {
            TiXmlElement* meshElement = e->FirstChildElement("meshName");
            meshElement->SetAttribute("meshName", entity.second.pScriptNode->GetMeshName().c_str());
        }
    }

    doc.SaveFile();
}
LoadingSystem::~LoadingSystem()
{
};
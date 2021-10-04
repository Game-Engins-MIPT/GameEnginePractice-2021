#include "FileSystem.h"

FileSystem::FileSystem()
{
	constexpr char cNativeSlash = static_cast<char>(e_cNativeSlash);

	m_strMediaRoot = "..";
	m_strMediaRoot.push_back(cNativeSlash);
	m_strMediaRoot.append("Media");
	m_strMediaRoot.push_back(cNativeSlash);

	m_strScriptsRoot = m_strMediaRoot;
	m_strScriptsRoot.append("Scripts");
	m_strScriptsRoot.push_back(cNativeSlash);
}

FileSystem::~FileSystem()
{
}

const std::string& FileSystem::GetMediaRoot()
{
	return m_strMediaRoot;
}

const std::string& FileSystem::GetScriptsRoot()
{
	return m_strScriptsRoot;
}
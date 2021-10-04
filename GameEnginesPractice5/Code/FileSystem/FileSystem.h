#pragma once

#include <string>

#include <shared_mutex>

#include "crc32.h"

typedef std::shared_mutex Lock;
typedef std::unique_lock<Lock>  WriteLock;
typedef std::shared_lock<Lock>  ReadLock;

class FileSystem
{
public:
#if defined(_WINDOWS)
	enum : char { e_cNativeSlash = '\\', e_cNonNativeSlash = '/' };
#else
	enum : char { e_cNativeSlash = '/', e_cNonNativeSlash = '\\' };
#endif

	FileSystem();
	~FileSystem();

	const std::string& GetMediaRoot();
	const std::string& GetScriptsRoot();

private:
	std::string m_strMediaRoot;
	std::string m_strScriptsRoot;

	Lock m_RWLock;
};
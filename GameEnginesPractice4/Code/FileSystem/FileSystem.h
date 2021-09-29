#pragma once

#include <string>

#include <shared_mutex>

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

private:
	std::string m_strMediaRoot;

	Lock m_RWLock;
};
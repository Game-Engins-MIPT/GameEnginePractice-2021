#include "IOWrapper.h"

size_t IOWrapper::Fread(void* pData, size_t nSize, size_t nCount, FILE* hFile)
{
	return ::fread(pData, nSize, nCount, hFile);
}

size_t IOWrapper::Fwrite(const void* pData, size_t size, size_t count, FILE* file)
{
	return ::fwrite(pData, size, count, file);
}

int IOWrapper::Fseek(FILE* hFile, _int64 seek, int mode)
{
	return ::fseek(hFile, (long)seek, mode);
}

_int64 IOWrapper::FTell(FILE* hFile)
{
#if defined(_WINDOWS)
	return ::_ftelli64(hFile);
#else
	return (int64)::ftell(hFile);
#endif
}

int IOWrapper::Fclose(FILE* hFile)
{
	return ::fclose(hFile);
}

FILE* IOWrapper::Fopen(const char* file, const char* mode)
{
	return ::fopen(file, mode);
}
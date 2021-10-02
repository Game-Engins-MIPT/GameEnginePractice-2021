#pragma once
#include <cstdio>

#include "../ProjectDefines.h"

class IOWrapper final
{
public:
	static size_t Fread(void* pData, size_t nSize, size_t nCount, FILE* hFile);
	static size_t Fwrite(const void* pData, size_t size, size_t count, FILE* file);
	static int Fclose(FILE* hFile);
	static FILE* Fopen(const char* file, const char* mode);
	static _int64 FTell(FILE* hFile);
	static int Fseek(FILE* hFile, _int64 seek, int mode);
};
#pragma once
#include <string>
class LoadingSystem
{
	LoadingSystem();
	~LoadingSystem();
	void LoadFromXML(const std::string fileName);
	void SaveToXML(const std::string fileName); 

};


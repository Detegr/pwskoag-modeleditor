#pragma once
#include <fstream>
#include <vector>

struct C_FileReader
{
	static std::string M_ReadToStr(const std::string& path);
	static std::vector<std::string> M_ReadToArray(const std::string& path);
};

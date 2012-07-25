#include "filereader.h"
#include <string>

std::string C_FileReader::M_ReadToStr(const std::string& path)
{
	std::ifstream i(path.c_str(), std::ifstream::in);
	std::string out;
	if(i.is_open())
	{
		std::string line;
		while(i.good())
		{
			while(getline(i, line))
			{
				out += "\n" + line;
			}
		}
	}
	i.close();
	return out;
}
#include <iostream>
std::vector<std::string> C_FileReader::M_ReadToArray(const std::string& path)
{
	std::ifstream i(path.c_str(), std::ifstream::in);
	std::vector<std::string> out;
	if(i.is_open())
	{
		std::string line;
		while(i.good())
		{
			while(getline(i, line))
			{
				if(line.length() && line[0]!='#') out.push_back(line);
			}
		}
	}
	i.close();
	return out;
}

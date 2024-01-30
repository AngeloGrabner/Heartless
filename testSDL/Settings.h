#pragma once
#include <unordered_map>
#include <string>

struct Settings
{
	static std::unordered_map<std::string, int> Table;
	
	static void Init(const std::string path); // to be Tested
	
	
};
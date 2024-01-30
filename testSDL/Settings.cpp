#include "Settings.h"
#include "CSV.h"
std::unordered_map<std::string, int> Settings::Table;

void Settings::Init(const std::string path)
{
	CSV csv(path, '=');
	for (int i = 0; i < csv.GetRowCount(); i++)
	{
		Table[csv.GetCell(i, 0)] = std::stoi(csv.GetCell(i, 1));
	}
}

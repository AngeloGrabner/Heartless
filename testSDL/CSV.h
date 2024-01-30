#pragma once
#include <vector>
#include <string>
#include <SDL.h>

#define CSV_BUFFER_SIZE 256
class CSV
{
	std::vector<std::vector<std::string>> mData;
	char mSeperator = ',';
	char mBuffer[CSV_BUFFER_SIZE];
public:
	CSV() = default;
	CSV(const std::string& path, char seperator = ',');
	CSV(const CSV&) = delete; // todo
	CSV(CSV&&) = delete;
	CSV& operator=(const CSV&) = delete;
	CSV& operator=(CSV&&) = delete;
	~CSV();
	void Load(const std::string& path);
	//void Safe() {}
	const std::vector<std::string>& GetRow(size_t index);
	const std::string& GetCell(size_t row, size_t column);
	size_t GetRowCount();
	//retuns the amount of elements of a given collumn
	size_t GetColumnCount(size_t row);
private:
	void ToCell(int i, int offset, char* data);
};
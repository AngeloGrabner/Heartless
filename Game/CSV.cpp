#include "CSV.h"
#include <iostream>
#include "Utility.h"

CSV::CSV(const std::string& path, char seperator)
	:mSeperator(seperator)
{
	try
	{
		SDL_memset(mBuffer, 0, CSV_BUFFER_SIZE);
		Load(path);
		//prevent commen user error
		//trim first and last row is they are empty 
		if (mData.size() > 0)
		{
			if (mData[0].empty() || mData[0][0].empty())
			{
				mData.erase(mData.begin());
			}
			if (mData[mData.size() - 1].empty() || mData[mData.size() - 1][0].empty())
			{
				mData.erase(std::prev(mData.end()));
			}
		}
	}
	catch (std::exception& e)
	{
		DB_OUT(e.what());
	}
}
void CSV::Load(const std::string& path)
{
	mData.clear();
	mData.push_back(std::vector<std::string>());

	size_t size;
	char* data = (char*)SDL_LoadFile(path.c_str(), &size);
	int offset = 0;
	bool rFlag = false, rnFlag = false, quotated = false, quotatedPrev = false;


	for (int i = 0; i <= size; i++)
	{
		if (rnFlag)
		{
			offset++;
			rnFlag = false;
		}
		if (data[i] == mSeperator)
		{
			if (!quotated)
			{
				ToCell(i, offset, data);	
				offset = i + 1;
			}
		}
		else if (data[i] == '\r')
		{
			if (i+1 <= size)
			if (data[i + 1] == '\n')
			{
				rnFlag = true;
				rFlag = true;
			}
			ToCell(i, offset, data);
			offset = i + 1;
			mData.push_back(std::vector<std::string>());
		}
		else if (data[i] == '\n' && !rFlag)
		{
			ToCell(i, offset, data);
			offset = i + 1;
			mData.push_back(std::vector<std::string>());
		}
		else if (data[i] == '"')
		{
			if (!quotated)
			{
				quotated = true;
			}
			else if (data[i + 1] != '"')
			{
				quotated = false;
			}
		}


	}
	ToCell(size, offset, data);

	SDL_free(data);
}
const std::vector<std::string>& CSV::GetRow(size_t index)
{
	return mData.at(index);
}
const std::string& CSV::GetCell(size_t row, size_t column)
{
	return mData.at(row).at(column);
}
size_t CSV::GetRowCount()
{
	return mData.size();
}
size_t CSV::GetColumnCount(size_t row)
{
	return mData.at(row).size();
}
void CSV::ToCell(int i, int offset, char* data) // offset is start, i is end
{
	// trim front spaces 
	for (int j = offset; j < i; j++)
	{
		if (data[j] == ' ')
		{
			offset++;
			
		}
		else break;
	}

	if (data[offset] == '"')
	{
		offset++;
	}
	if (data[i] == '"')
	{
		i--;
	}

	bool quotatedPrev = false;
	char* buf = nullptr;

	if (i - offset < CSV_BUFFER_SIZE - 1) // prevent buffer overflow
	{
		buf = mBuffer;
	}
	else
	{
		buf = (char*)SDL_malloc(sizeof(char) * (i - offset));
	}

	int index = 0;
	for (char* it = (char*)(data + offset); it < (char*)(data + i); it++)
	{
		if ((*it) == '"' && !quotatedPrev)
		{
			quotatedPrev = true;
			continue;
		}
		else
			quotatedPrev = false;
		buf[index] = *it;
		index++;
	}
	//SDL_memcpy(mBuffer, data + offset, i - offset);
	buf[i - offset] = 0;
	mData[mData.size() - 1].push_back(buf);

	if (!(i - offset < CSV_BUFFER_SIZE - 1))
	{
		SDL_free(buf);
	}
}
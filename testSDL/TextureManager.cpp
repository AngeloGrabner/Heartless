#include "TextureManager.h"
#include "Renderer.h"
#include "Utility.h"

std::vector<Texture> TextureManager::sCache;
std::vector<TextureManager::InternBundle> TextureManager::sData;

void TextureManager::Init(const std::string& pathToCSV)
{
	CSV csv(pathToCSV);
	//std::cout << csv.GetCell(csv.GetColumnCount(csv.GetRowCount() - 1) - 1, csv.GetRowCount() - 1);
	auto rows = csv.GetRowCount();
	InternBundle ib;
	for (int i = 0; i < rows; i++)
	{
		try
		{
			ib.Clear();
			auto row = csv.GetRow(i);
			
			ib.id = std::stoi(row[0]);
			if (csv.GetColumnCount(i) <= 2)
			{
				sData.push_back(ib);
				continue;
			}
			ib.path = row[1];
			ib.rect = { stoi(row[2]),stoi(row[3]),stoi(row[4]),stoi(row[5]) };
			sData.push_back(ib);
		}
		catch (std::exception& e)
		{
			LOG_PUSH(e.what());
		}
	}
	std::sort(sData.begin(), sData.end(), [](InternBundle a, InternBundle b) -> bool {return a.id < b.id; });
}
Texture TextureManager::Get(int id)
{

	SDL_assert(id >= 0);

	if (sData.at(id).texId >= 0)
	{
		return Texture(sCache[sData[id].texId].tex, sData[id].rect);
	}
	else
	{
		SDL_assert(!sData[id].path.empty());

		sCache.push_back(LoadBmp(sData[id].path));
		for (int i = 0; i < sData.size(); i++)
		{
			if (sData[id].path == sData[i].path)
			{
				sData[i].texId = sCache.size() - 1;
			}
		}
		return std::move(TextureManager::Get(id));
	}
}
void TextureManager::FlushTextures()
{
	for (int i = 0; i < sData.size(); i++)
	{
		sData[i].texId = -1;
	}
	for (int i = 0; i < sCache.size(); i++)
	{
		if (sCache[i].tex != nullptr)
		{
			SDL_DestroyTexture(sCache[i].tex);
			sCache[i].tex = nullptr;
		}
	}
}
void TextureManager::Free() // destructor
{
	FlushTextures();
	sData.clear();
	sCache.clear();
}
Texture TextureManager::LoadBmp(const std::string& path)
{
	SDL_Surface* sur = SDL_LoadBMP(path.c_str());
	if (!sur)
	{
		std::cout << "SDL error: " << SDL_GetError();
	}
	SDL_Rect r = { 0,0,sur->w,sur->h };

	SDL_Texture* tex = SDL_CreateTextureFromSurface(Renderer::Get(), sur);
	if (!tex)
	{
		std::cout << "SDL error: " << SDL_GetError();
	}

	SDL_FreeSurface(sur);

	return Texture(tex, r);
}
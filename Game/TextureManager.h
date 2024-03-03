#pragma once
#include "CSV.h"
#include "Texture.h"
#include <algorithm>

class TextureManager
{
	struct InternBundle
	{
		std::string path = "";
		SDL_Rect rect = { 0,0,0,0 };
		int id = 0; 
		int texId = -1;
		void Clear()
		{
			path = "";
			rect = { 0,0,0,0 };
			id = 0;
			texId = -1;
		}
	};
	static std::vector<Texture> sCache;
	static std::vector<InternBundle> sData;

public:
	//CSV format: id[,path,x,y,w,h]
	// only using id as a placeholder is also possible
	static void Init(const std::string& pathToCSV);
	static Texture Get(int id);
	static void FlushTextures();
	static void Free(); // destructor
	static Texture LoadBmp(const std::string& path);
};
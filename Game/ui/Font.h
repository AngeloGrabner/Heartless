#pragma once
#include "../Renderer.h"
#include "../Color.h"
#include <unordered_map>

namespace ui
{
	constexpr int CHAR_COUNT = 128;
	struct Font
	{
		int fontType = -1;
		SDL_Point charSize{ 0,0 };
		int customWidth[CHAR_COUNT];
		int xOffset = -1, yOffset = -1;
		int texID = -1; 
		Font() = default;
		inline Font(int Type, SDL_Point CharSize, int texId)
			: fontType(Type), charSize(CharSize), texID(texId) 
		{
			SDL_memset(customWidth, -1, CHAR_COUNT);
		}
	};
	class FontWriter
	{
	public:
		struct Drawprogress
		{
			bool finish = false;
			int failedAtIndex = 0;
			SDL_Rect endRect;
			Drawprogress() = default;
			Drawprogress(bool done, int fail, SDL_Rect rect)
				: finish(done), failedAtIndex(fail), endRect(rect) {};
		};
	private:
		static std::vector<Font> sFonts;
		static int sSelected;
		static bool sMonspace;
		static std::unordered_map<char, int> sLookupTable;
	public:
		// row 0: list of supported charaters 
		// row 1: textureID,fontId,width,height,xOffset,yOffset,followed by custom charater width
		static void Init(const std::string& pathToCSV);
		// returns the position of the last drawn charater 
		static Drawprogress DrawText(SDL_Rect area, 
			const std::string& text, 
			SDL_FPoint scale = { 1.0f,1.0f },
			Color tint = {255,255,255,255},
			bool breakOnnewLine = false);
		static bool SetFont(int id);
		static int GetFont();
		//returns monospace size if char is null, else custom char size
		static SDL_Point GetSize(char c = 0);	
		static SDL_Point GetCharSpacing();

		//retuns the length, ignores new lines;
		static SDL_FPoint GetStrSize(const std::string& text, SDL_FPoint scale = { 1.0f,1.0f });

	};
}
#include "Font.h"
#include "../CSV.h"
#include "../TextureManager.h"

std::vector<ui::Font> ui::FontWriter::sFonts;
int ui::FontWriter::sSelected = -1;
bool ui::FontWriter::sMonspace = false;
std::unordered_map<char, int> ui::FontWriter::sLookupTable;

void ui::FontWriter::Init(const std::string& pathToCSV)
{
	sSelected = 0;
	CSV csv(pathToCSV);
	const auto& row0 = csv.GetRow(0);
	for (int i = 0; i < row0.size(); i++)
		sLookupTable[row0[i][0]] = i;
	for (int i = 1; i < csv.GetRowCount(); i++)
	{
		Font f;
		auto t = csv.GetCell(i, 0);
		f.texID = std::stoi(t);
		f.fontType = std::stoi(csv.GetCell(i, 1));
		f.charSize = { std::stoi(csv.GetCell(i, 2)),std::stoi(csv.GetCell(i, 3)) };
		f.xOffset = std::stoi(csv.GetCell(i, 4));
		f.yOffset = std::stoi(csv.GetCell(i, 5));
		for (int j = 6; j < csv.GetColumnCount(i); j++)
		{
			auto cell = csv.GetCell(i, j);
			if (!cell.empty())
			{
				f.customWidth[j - 6] = std::stoi(cell);
			}
		}
		sFonts.push_back(f);
	}
}

ui::FontWriter::Drawprogress ui::FontWriter::DrawText(SDL_Rect area, const std::string& text, SDL_FPoint scale, Color tint, bool breakOnnewLine)
{
	const Font& font = sFonts.at(sSelected);
	Texture tex =  TextureManager::Get(font.texID);
	tex.SetTint(tint);

	SDL_Rect textureRect = tex.rect;
	SDL_Rect drawPos = { area.x,area.y, (int)(font.charSize.x*scale.x),(int)(font.charSize.y* scale.y)};
	int offset = font.charSize.x;
	tex.rect.w = font.charSize.x;
	for (int i = 0; i < text.size(); i++)
	{
		tex.rect.x = textureRect.x + sLookupTable[text[i]]* font.charSize.x;
		
		if (text[i] == '\n')
		{
			if (breakOnnewLine)
			{
				return { false,i,drawPos };
			}
			drawPos.x = area.x;
			drawPos.y += drawPos.h + (int)(font.yOffset * scale.y);
			if (area.y + area.h < drawPos.y + drawPos.h)
			{
				return { false,i,drawPos };
			}
			continue;
		}
		else if (text[i] == '\t')
		{
			drawPos.x += drawPos.w * 4;
		}
		else if (text[i] == ' ')
		{
			drawPos.x += drawPos.w;
			continue;
		}
		if (!sMonspace)
		{
			drawPos.w = (int)(font.customWidth[sLookupTable[text[i]]] * scale.x);
			tex.rect.w = font.customWidth[sLookupTable[text[i]]];
		}
		if (area.x + area.w < drawPos.x + drawPos.w)
		{
			if (breakOnnewLine)
			{
				return { false,i,drawPos };
			}
			drawPos.x = area.x;
			drawPos.y += drawPos.h + (int)(font.yOffset * scale.y);
			if (area.y + area.h < drawPos.y + drawPos.h)
			{
				return { false,i,drawPos};
			}
		}

		Renderer::DrawTexture(tex, drawPos);

		drawPos.x += drawPos.w + (int)(font.xOffset * scale.x);

	}

	tex.SetTint(WHITE);
	return {true,(int)(text.size()-1),drawPos};
}

SDL_Point ui::FontWriter::GetCharSpacing()
{
	auto font = sFonts.at(sSelected);
	return { font.xOffset,font.yOffset };
}

bool ui::FontWriter::SetFont(int id)
{
	if (id < sFonts.size() && id >= 0)
	{
		sSelected = id;
		return true;
	}
	return false;
}

int ui::FontWriter::GetFont()
{
	return sSelected;
}

SDL_Point ui::FontWriter::GetSize(char c)
{
	if (sMonspace || c == 0 || c == ' ')
		return sFonts[sSelected].charSize; 
	return { sFonts[sSelected].customWidth[sLookupTable.at(c)], sFonts[sSelected].charSize.y };
}

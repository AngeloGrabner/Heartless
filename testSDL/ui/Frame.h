#pragma once
#include "Ui.h"
#include "../Color.h"
namespace ui
{
	class Frame : public Widget
	{
	protected:
		int mTexId = -1; 
		float mScale = 1.0f;
		SDL_Rect mTexAreaChache = { 0,0,0,0 };
		Color mBackgroundTint = { 255,255,255 };
	public:
		Frame(Widget* parent, SDL_FRect area, const std::string& name, int textureID,WidgetPostion x = WIDGET_LEFT, WidgetPostion y = WIDGET_TOP, float scale = 1.0f);
		void Update() override;
		void Draw() override;
		void DebugDraw() override;

		Frame* SetScale(float scale);
		Frame* SetTexture(int textureId);

		SDL_FRect GetInnerRect() const;
		float GetScale();

	};
}
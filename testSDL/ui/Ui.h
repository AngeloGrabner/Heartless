#pragma once
#include "../Renderer.h"
#include "../Event.h"
#include <stdint.h>
#include <memory>

namespace ui
{
	enum WidgetPostion// relative to parent
	{
		WIDGET_CENTER,
		WIDGET_TOP,
		WIDGET_LEFT = WIDGET_TOP,
		WIDGET_BOTTOM,
		WIDGET_RIGHT = WIDGET_BOTTOM
	};
	class Widget
	{ 

	protected:  
		SDL_FRect mArea, mBorder = { 0,0,0,0 };
		std::vector<std::unique_ptr<Widget>> mChilds;
		bool mActive = true;
		std::string mName = "";
		//used to prevent multible calls into DebugDrawChildrin/DrawChildrin 
		bool mChildrinDrawFlag = false;
		bool mChildrinDebugDrawFlag = false;
	public:
		Widget() = delete;
		Widget(Widget* parent, SDL_FRect area, const std::string& name, WidgetPostion x = WIDGET_LEFT, WidgetPostion y = WIDGET_TOP); // hier
		Widget(const Widget& other) = delete;
		Widget(Widget&& other) noexcept;
		Widget& operator=(const Widget & other) = delete;
		Widget& operator=(Widget&& other) noexcept;

		void Move(int xOffset, int yOffset);
		const std::string& GetName() const;
		const SDL_FRect& GetArea() const;

		Widget* SetBorder(SDL_FRect size);

		Widget* Activate(bool active);
		void Activate(const std::string& name, bool active);

		virtual void Update();
		virtual void Draw() = 0;
		virtual void DebugDraw();

	protected:
		void DrawChildrin();
		void UpdateChildrin();
		void DebugDrawChildrin();
	};
}
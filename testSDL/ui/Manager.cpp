#include "Manager.h"
#include "../Renderer.h"
#include "../Window.h"

ui::Manager::Manager(Manager&& other) noexcept
	: mWidgets(std::move(other.mWidgets))
{
}

ui::Manager& ui::Manager::operator=(Manager&& other) noexcept
{
	if (this != &other)
	{
		mWidgets = std::move(other.mWidgets);
	}
	return *this;
}

void ui::Manager::Handle(const SDL_Event* e)
{
	std::string editorCheckbox;
	bool checked;
	if (EventReceiver::CheckBox(e, editorCheckbox, checked))
	{
		if (editorCheckbox == "editorCheckBox")
		{
			Activate("editorFrame", checked);
		}
	}
}

void ui::Manager::Update()
{
	for (int i = mWidgets.size() -1 ; i >= 0; i--)
	{
		mWidgets[i]->Update();
	}
}

void ui::Manager::Draw()
{
	Renderer::SetScreenMode(false);
	for (int i = 0; i < mWidgets.size(); i++)
	{
		mWidgets[i]->Draw();
	}
	Renderer::SetScreenMode(true);
}

void ui::Manager::DebugDraw()
{
	Renderer::SetScreenMode(false);
	for (int i = 0; i < mWidgets.size(); i++)
	{
		mWidgets[i]->DebugDraw();
	}
	Renderer::SetScreenMode(true);
}



void ui::Manager::Activate(const std::string& name, bool active)
{
	for (int i = 0; i < mWidgets.size(); i++)
	{
		if (mWidgets[i])
			mWidgets[i]->Activate(name, active);
	}
}

void ui::Manager::Init()
{
	SDL_Point winSize = Window::GetSize();
	float aspectRatio = winSize.x / (float)winSize.y;

	{ // editor checkbox
		SDL_FRect boxArea = SDL_FRect(0, 0, winSize.x / 15.0f , winSize.y* aspectRatio / 15.0f);
		ui::Widget* editorBox =
			(new ui::CheckBox(nullptr, boxArea, "editorCheckBox", WIDGET_BOTTOM, WIDGET_RIGHT))
			->SetTextures(4, 5); // see data.csv 
		mWidgets.push_back(std::unique_ptr<ui::Widget>(editorBox));
	}

	{ // editor pannel 
		SDL_FRect frameArea = SDL_FRect(0, 0, winSize.x / 6.0f, winSize.y / 10.0f * 4.5 * aspectRatio);
		
		ui::Widget* editorFrame =
			(new ui::Frame(nullptr, frameArea, "editorFrame", 1, WIDGET_RIGHT, WIDGET_TOP))
			->Activate(false);
		
		mWidgets.push_back(std::unique_ptr<ui::Widget>(editorFrame));
		
		Widget* layerDiv = new ui::Div(editorFrame,SDL_FRect(0, 5, editorFrame->GetArea().w, editorFrame->GetArea().h / 15.0f),WIDGET_CENTER,WIDGET_TOP);
		
		{

			SDL_FRect sliderArea = SDL_FRect(0, 0,layerDiv->GetArea().w/2.0f, layerDiv->GetArea().h);

			ui::Widget* layerSlider =
				(new ui::Slider(layerDiv, sliderArea, "editorLayerSlider", false, WIDGET_RIGHT, WIDGET_CENTER, 4, { layerDiv->GetArea().h/2.0f,layerDiv->GetArea().h-2 }))
				->SetSnappiness(true, 3);

			SDL_FRect textboxArea = SDL_FRect(0, 0, layerDiv->GetArea().w / 2.0f, layerDiv->GetArea().h);

			ui::Widget* layerSliderLable =
				(new ui::Label(layerDiv, textboxArea, "editorLayerLable", WIDGET_LEFT, WIDGET_CENTER))
				->SetText("Layer");
		}
	}
}

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
	for (int i = 0; i < mWidgets.size(); i++)
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
	float aspectRatio = winSize.x / (float)winSize.x;

	{ // editor checkbox
		SDL_FRect boxArea = SDL_FRect(0, 0, winSize.x / 10.0f, winSize.y / 10.0f);
		ui::Widget* editorBox =
			(new ui::CheckBox(nullptr, boxArea, "editorCheckBox", WIDGET_BOTTOM, WIDGET_RIGHT))
			->SetTextures(4, 5); // see data.csv 
		mWidgets.push_back(std::unique_ptr<ui::Widget>(editorBox));
	}

	{ // editor pannel 
		SDL_FRect frameArea = SDL_FRect(0, 0, winSize.x / 6.0f, winSize.y / 10.0f * 8.0f);
		
		ui::Widget* editorFrame =
			(new ui::Frame(nullptr, frameArea, "editorFrame", 1, WIDGET_RIGHT, WIDGET_TOP))
			->Activate(false);

		SDL_FRect sliderArea = SDL_FRect(0, 0, editorFrame->GetArea().w / 3.0f, editorFrame->GetArea().h / 15.0f);

		ui::Widget* layerSlider =
			(new ui::Slider(editorFrame, sliderArea, "editorLayerSlider",false, WIDGET_CENTER, WIDGET_TOP,4,{16,32}));

		mWidgets.push_back(std::unique_ptr<ui::Widget>(editorFrame));
	}
}

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
	std::string name;
	bool checked;
	float val;
	if (EventReceiver::CheckBox(e, name, checked))
	{
		if (name == "editorCheckBox")
		{
			Activate("editorFrame", checked);
		}
	}
	if (EventReceiver::Slider(e, name, val))
	{
		if (name == "editorLayerSlider")
		{
			if (val == 0.0f)
			{
				Activate("editorTileDiv",true);
				Activate("editorEntityDiv",false);
			}
			else if (val == .5f)
			{
				Activate("editorTileDiv", false);
				Activate("editorEntityDiv", true);
			}
			else if (val == 1.0f)
			{
				Activate("editorTileDiv", true);
				Activate("editorEntityDiv", false);
			}
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

//warning the following code is CURSED
//btw it might look like a mem leak, but it isnt, see ui::widget for details
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
		
		Widget* layerDiv = new ui::Div(editorFrame,SDL_FRect(0, 5, editorFrame->GetInnerArea().w, editorFrame->GetInnerArea().h / 15.0f),WIDGET_CENTER,WIDGET_TOP);
		
		{ //slider div

			SDL_FRect sliderArea = SDL_FRect(0, 0,layerDiv->GetInnerArea().w/2.0f, layerDiv->GetInnerArea().h);

			(new ui::Slider(layerDiv, sliderArea, "editorLayerSlider", false, WIDGET_RIGHT, WIDGET_CENTER, 4, { layerDiv->GetInnerArea().h/2.0f,layerDiv->GetInnerArea().h-2 }))
				->SetSnappiness(true, 3);

			SDL_FRect textboxArea = SDL_FRect(0, 0, layerDiv->GetInnerArea().w / 2.0f, layerDiv->GetInnerArea().h);

			(new ui::Label(layerDiv, textboxArea, "editorLayerLable", WIDGET_LEFT, WIDGET_CENTER))
				->SetText("Layer");
		}

		Widget* tileDiv = (new ui::Div(editorFrame, SDL_FRect(0, editorFrame->GetInnerArea().h / 15.0f, editorFrame->GetInnerArea().w, editorFrame->GetInnerArea().h - editorFrame->GetInnerArea().h / 15.0f)))
			->SetName("editorTileDiv")
			->SetBorder({ 5,5,5,5 });

		{ // tile div
			SDL_FRect selectTextureArea = SDL_FRect(0, 0, tileDiv->GetInnerArea().w / 2.0f, tileDiv->GetInnerArea().h / 15.0f);
		
			(new ui::Select(tileDiv, selectTextureArea, "editorTileTextureSelect", 6))
				->AddOption("gras")
				->AddOption("sand")
				->AddOption("dirt")
				->AddOption("snow")
				->AddOption("stone brick w")
				->AddOption("dirt w")
				->AddOption("stone w")

				->SetBorder({ 2,2,2,2 });

			(new ui::Select(tileDiv, selectTextureArea, "editorTileSubTextureSelect", 6, WIDGET_RIGHT))
				->AddOption("full")
				->AddOption("left")
				->AddOption("right")
				->AddOption("bottom")
				->AddOption("top")
				->AddOption("inner t l")
				->AddOption("inner t r")
				->AddOption("inner b l")
				->AddOption("inner b r")
				->AddOption("outer t l")
				->AddOption("outer t r")
				->AddOption("outer b l")
				->AddOption("outer b r")
				
				->SetBorder({ 2,2,2,2 });

			{
				SDL_FRect checkboxArea = SDL_FRect(0, tileDiv->GetInnerArea().h / 14.0f, tileDiv->GetInnerArea().w/3.0f,20 );

				ui::Widget* solidDiv = new ui::Div(tileDiv, checkboxArea);

				(new ui::Label(solidDiv, SDL_FRect(0,0,100,20), "editorTileSolidLabel"))
					->SetAlignment(WIDGET_LEFT)
					->SetText("Solid");

				(new ui::CheckBox(solidDiv, SDL_FRect(0, 0,20,20), "editorTileSolidCheckbox", WIDGET_RIGHT))
					->SetTextures(4, 5);
			
			}

			{
				SDL_FRect tileTopArea = SDL_FRect(0, tileDiv->GetInnerArea().h / 14.0f * 2, tileDiv->GetInnerArea().w, tileDiv->GetInnerArea().h / 20.0f);

				(new ui::Select(tileDiv,tileTopArea,"editorTileTopSelect",6))
					->AddOption("No Top")
					->AddOption("Top")
					->AddOption("Top on Top")

					->SetBorder({ 2,2,2,2 });
			}
			
		}

		Widget* entityDiv = (new ui::Div(editorFrame, SDL_FRect(0, editorFrame->GetInnerArea().h / 15.0f, editorFrame->GetInnerArea().w, editorFrame->GetInnerArea().h - editorFrame->GetInnerArea().h / 15.0f)))
			->SetName("editorEntityDiv")
			->SetBorder({ 5, 5, 5, 5 })
			->Activate(false);;

		{ // entity div

			/* plan
			
				input x
				input y
				select stats
				input stats
			div item
				input slot
				select itme
				button insert
				button delete
			div createNdelete
				select type 
				button delete
				button insert
			*/
			Widget* statsDiv = new ui::Div(entityDiv, SDL_FRect(0, 0, entityDiv->GetInnerArea().w, entityDiv->GetInnerArea().h / 4.0f));
				
			
			{ // div stats
				auto Area = SDL_FRect(0, 0, statsDiv->GetInnerArea().w, statsDiv->GetInnerArea().h / 4.1f);
				(new ui::InputField(statsDiv, Area, "editorEntityXInputField"))
					->SetTexture(7)
					->SetText("X")
					->SetBorder({ 5,5,5,5 });

				Area.y += Area.h;

				(new ui::InputField(statsDiv, Area, "editorEntityYInputField"))
					->SetTexture(7)
					->SetText("Y")
					->SetBorder({ 5,5,5,5 });

				Area.y += Area.h;

				(new ui::Select(statsDiv, Area, "editorEntityStatsSelect", 6))
					->AddOption("atk")
					->AddOption("hp")
					->AddOption("spd")

					->SetBorder({ 2, 2, 2, 2 });

				Area.y += Area.h;

				(new ui::InputField(statsDiv, Area, "editorEntityStatsInputField"))
					->SetTexture(7)
					->SetText("Value")
					->SetBorder({ 5,5,5,5 });
			}

			Widget* itemDiv = new ui::Div(entityDiv, SDL_FRect(0, 0, entityDiv->GetInnerArea().w, entityDiv->GetInnerArea().h / 4.0f), WIDGET_LEFT, WIDGET_CENTER);
		
			{
				auto Area = SDL_FRect(0, 0, itemDiv->GetInnerArea().w, itemDiv->GetInnerArea().h / 4.1);
				
				(new ui::InputField(itemDiv, Area, "editorEntityItemSoltInputField"))
					->SetText("Slot number")
					->SetTexture(7)
					->SetBorder({ 5,5,5,5 });

				Area.y += Area.h;

				(new ui::Select(itemDiv, Area, "editorEntityItemSelect", 6))
					->AddOption("todo")
					
					->SetBorder({2,2,2,2}); // hier

				Area.y += Area.h;

				(new ui::Button(itemDiv, Area, "editorEntityItemInsertButton", 3))
					->SetCooldown(50)
					->SetText("Insert")
					->SetBorder({2,2,2,2 });

				Area.y += Area.h;

				(new ui::Button(itemDiv, Area, "editorEntityItemDeleteButton", 3))
					->SetCooldown(50)
					->SetText("Delete")
					->SetBorder({ 2,2,2,2 });
			}
		}
	}
}

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
	for (auto& ptr : mWidgets)
	{
		ptr->Handle(e);
	}

	std::string name;
	bool checked;
	float val;
	if (EventReceiver::CheckBox(e, name, checked))
	{
		if (name == "editorCheckBox")
		{
			Activate("editorFrame", checked);
			Activate("sceneFrame", checked);
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
	if (EventReceiver::Button(e, name))
	{
		if (name == "sceneSwapButton")
		{
			Activate("editorFrame", false);
			Activate("sceneFrame", false);
			EventBuilder::ReCheckBox("editorCheckBox", false);
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

#define TEX_ID_INPUTFIELD 7
#define TEX_ID_BUTTON 3
#define TEX_ID_FRAME 1
#define TEX_ID_SLIDER 4
#define TEX_ID_SELECT 6
#define TEX_ID_CHECKBOX_FRAME 4 
#define TEX_ID_CHECKBOX_CHECK 5

//warning the following code is CURSED
//btw it might look like a mem leak, but it isnt, see ui::widget for details
void ui::Manager::Init()
{
	using namespace ui;

	SDL_Point winSize = Window::GetSize();
	float aspectRatio = winSize.x / (float)winSize.y;

	{ // editor checkbox
		SDL_FRect boxArea = SDL_FRect(0, 0, winSize.x / 15.0f, winSize.y * aspectRatio / 15.0f);
		
		Widget* editorBox =
			(new ui::CheckBox(nullptr, boxArea, "editorCheckBox", WIDGET_BOTTOM, WIDGET_RIGHT))
			->SetTextures(TEX_ID_CHECKBOX_FRAME, TEX_ID_CHECKBOX_CHECK); // see data.csv 
		mWidgets.push_back(std::unique_ptr<ui::Widget>(editorBox));
	}

	{ // editor pannel 
		SDL_FRect frameArea = SDL_FRect(0, 0, winSize.x / 6.0f, winSize.y / 10.0f * 4.5 * aspectRatio);

		ui::Widget* editorFrame =
			(new ui::Frame(nullptr, frameArea, "editorFrame", TEX_ID_FRAME, WIDGET_RIGHT, WIDGET_TOP))
			->SetBorder({2,2,2,2})
			->Activate(false);

		mWidgets.push_back(std::unique_ptr<ui::Widget>(editorFrame));
		
		SDL_FRect headerArea = SDL_FRect(0, 0, editorFrame->GetInnerArea().w, editorFrame->GetInnerArea().h/10);
		// editor pannel header 
		{
			Div* header = (new Div(editorFrame, headerArea));


			SDL_FRect rowArea = { 0,0,header->GetInnerArea().w/2, header->GetInnerArea().h /2};
			
			(new Label(header, rowArea, "editorFrameHeaderLabel1"))
				->SetText("Draw");
			
			{ // row 0 check div and checks
				auto checkDivArea = rowArea;
				checkDivArea.x = checkDivArea.w;

				Div* checkDiv = (new Div(header, checkDivArea));

				SDL_FRect checkArea(0, 0, 0, 0);
				checkArea.w = checkDiv->GetInnerArea().w;
				checkArea.h = checkDiv->GetInnerArea().h;


				checkArea.w /= 3.0f;

				checkArea.w < checkArea.h ? checkArea.h = checkArea.w : checkArea.w = checkArea.h;	//min()

				(new CheckBox(checkDiv, checkArea, "editorFrameHeaderCheck1"))
					->SetTextures(TEX_ID_CHECKBOX_FRAME, TEX_ID_CHECKBOX_CHECK);
				(new CheckBox(checkDiv, checkArea, "editorFrameHeaderCheck2", WIDGET_CENTER))
					->SetTextures(TEX_ID_CHECKBOX_FRAME, TEX_ID_CHECKBOX_CHECK);
				(new CheckBox(checkDiv, checkArea, "editorFrameHeaderCheck3", WIDGET_RIGHT))
					->SetTextures(TEX_ID_CHECKBOX_FRAME, TEX_ID_CHECKBOX_CHECK);
			}

			rowArea.y += rowArea.h;

			(new Label(header, rowArea, "editorFrameHeaderLabel2"))
				->SetText("Selecting");

			rowArea.x += rowArea.w;

			(new Select(header, rowArea, "editorFrameHeaderSelectingSelect", TEX_ID_SELECT))
				->AddOption("Tile")
				->AddOption("Entity");
		}
		
		SDL_FRect bodyDivArea = SDL_FRect(0, headerArea.h, editorFrame->GetInnerArea().w, editorFrame->GetInnerArea().h - headerArea.h);

		// editor pannel Tile
		{
			Widget* tileDiv = (new Div(editorFrame, bodyDivArea))
				->SetName("editorFrameTileDiv")
				->SetBorder({2,2,2,2});

			SDL_FRect area = { 0,0,tileDiv->GetInnerArea().w/2,tileDiv->GetInnerArea().h/ 16};


			(new Label(tileDiv, area, "editorFrameTileLabel1"))
				->SetText("Type")
				->SetAlignment(WIDGET_LEFT);
			(new Select(tileDiv, area, "editorFrameTileTypeSelect", TEX_ID_SELECT,WIDGET_RIGHT))
				->AddOption("Tile")
				->AddOption("Swap");

			area.y += area.h;

			{ // double checkbox row 
				SDL_FRect tempArea = area;
				tempArea.w *= 0.8f;

				(new Label(tileDiv, tempArea, "editorFrameTileLabel2"))
					->SetText("Solid")
					->SetAlignment(WIDGET_LEFT);

				tempArea.x += tempArea.w;
				tempArea.w = area.w * .2f;

				tempArea.w < tempArea.h ? tempArea.h = tempArea.w : tempArea.w = tempArea.h;

				(new CheckBox(tileDiv, tempArea, "editorFrameTileSolidCheckBox"))
					->SetTextures(TEX_ID_CHECKBOX_FRAME, TEX_ID_CHECKBOX_CHECK);

				tempArea.h = area.h;
				tempArea.x += tempArea.w;
				tempArea.w = area.w * .8f;

				(new Label(tileDiv,tempArea, "editorFrameTileLabel3"))
					->SetText("Light")
					->SetAlignment(WIDGET_LEFT);

				tempArea.x += tempArea.w;
				tempArea.w = area.w * .2f;

				tempArea.w < tempArea.h ? tempArea.h = tempArea.w : tempArea.w = tempArea.h;

				(new CheckBox(tileDiv, tempArea, "editorFrameTileLightCheckBox"))
					->SetTextures(TEX_ID_CHECKBOX_FRAME, TEX_ID_CHECKBOX_CHECK);
			}

			area.y += area.h;


			(new Label(tileDiv, area, "editorFrameTileLabel4"))
				->SetText("Top")
				->SetAlignment(WIDGET_LEFT);

			(new Select(tileDiv, area, "editorFrameTileTopSelect", TEX_ID_SELECT, WIDGET_RIGHT))
				->AddOption("No Top")
				->AddOption("Top")
				->AddOption("Top on Top");

			area.y += area.h;

			(new Label(tileDiv, area, "editorFrameTileLabel5"))
				->SetText("Tex Top")
				->SetAlignment(WIDGET_LEFT);

			(new Select(tileDiv, area, "editorFrameTileTexTopSelect", TEX_ID_SELECT, WIDGET_RIGHT))
				->AddOption("Bottom")
				->AddOption("Top");

			area.y += area.h;

			(new Label(tileDiv, area, "editorFrameTileLabel6"))
				->SetText("Tex")
				->SetAlignment(WIDGET_LEFT);

			(new Select(tileDiv, area, "editorFrameTileTexSelect", TEX_ID_SELECT, WIDGET_RIGHT))
				->AddOption("todo grass");

			area.y += area.h;

			//todo label & select for tex rotation
		}
	}

	{ //scene pnnel
		SDL_FRect frameArea = SDL_FRect(0, 0, winSize.x / 8.0f, winSize.y / 6.0f * aspectRatio);

		ui::Widget* sceneFrame = (new ui::Frame(nullptr, frameArea, "sceneFrame", TEX_ID_FRAME, WIDGET_LEFT, WIDGET_CENTER))
			->Activate(false)
			->SetBorder({ 4,4,4,4 });
		mWidgets.push_back(std::unique_ptr<ui::Widget>(sceneFrame));

		SDL_FRect inputArea = SDL_FRect(0, 0, sceneFrame->GetInnerArea().w, sceneFrame->GetInnerArea().h/5.0f);
		
		{ // name field
			(new ui::InputField(sceneFrame, inputArea, "sceneNameInputField"))
				->SetTexture(TEX_ID_INPUTFIELD)
				->SetText("Name")
				->SetBorder({5,5,5,5});
		}

		inputArea.y += inputArea.h;

		{ // width field
			(new ui::InputField(sceneFrame, inputArea, "sceneWidthInputField"))
				->SetTexture(TEX_ID_INPUTFIELD)
				->SetText("Width")
				->SetBorder({ 5,5,5,5 });
		}

		inputArea.y += inputArea.h;

		{ // heightfield
			(new ui::InputField(sceneFrame, inputArea, "sceneHeightInputField"))
				->SetTexture(TEX_ID_INPUTFIELD)
				->SetText("Height")
				->SetBorder({ 5,5,5,5 });
		}

		inputArea.y += inputArea.h;

		{ // create button
			(new ui::Button(sceneFrame, inputArea, "sceneCreateButton", TEX_ID_BUTTON))
				->SetCooldown(2000)
				->SetText("Create")
				->SetBorder({ 5,5,5,5 });
		}

		inputArea.y += inputArea.h;

		{ // switch button
			(new ui::Button(sceneFrame, inputArea, "sceneSwapButton", TEX_ID_BUTTON))
				->SetCooldown(2000)
				->SetText("Swap")
				->SetBorder({ 5,5,5,5 });
		}
	}
}

#undef TEX_ID_INPUTFIELD
#undef TEX_ID_BUTTON
#undef TEX_ID_FRAME
#undef TEX_ID_SLIDER
#undef TEX_ID_SELECT 
#undef TEX_ID_CHECKBOX_FRAME
#undef TEX_ID_CHECKBOX_CHECK
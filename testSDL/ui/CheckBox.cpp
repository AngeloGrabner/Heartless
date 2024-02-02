#include "CheckBox.h"
#include "../Input.h"
#include "../TextureManager.h"

ui::CheckBox::CheckBox(Widget* parent, SDL_FRect area, const std::string& name, WidgetPostion x, WidgetPostion y, int boxTextureId, int checkTextureId, bool checked)
	: Widget(parent,area,name,x,y), mCheckTexId(checkTextureId), mBoxTexId(boxTextureId),
	mChecked(checked)
{

}

ui::CheckBox* ui::CheckBox::SetTextures(int boxTextureId, int checkTextureId)
{
	mCheckTexId = checkTextureId;
	mBoxTexId = boxTextureId;
	return this;
}

ui::CheckBox* ui::CheckBox::SetChecked(bool checked)
{
	mChecked = checked;
	return this;
}

void ui::CheckBox::Update()
{
	if (!mActive)
		return;
	Widget::Update();
	if (contain(mArea, Input::GetMousePos()) && 
		Input::GetMouse(Input::LMB, LAYER_UI).Down())
	{
		Input::Handled(Input::LMB, LAYER_UI);
		mChecked = !mChecked;
		EventBuilder::CheckBox(mName, mChecked);
	}
}

void ui::CheckBox::Draw()
{
	if (!mActive)
		return;
	auto box = TextureManager::Get(mBoxTexId);
	auto check = TextureManager::Get(mCheckTexId);
	bool hoverOverBox = contain(mArea, Input::GetMousePos());
	if (hoverOverBox)
	{
		box.SetTint(mTint);
		check.SetTint(mTint);
	}
	Renderer::DrawTexture(box, mArea);
	if (mChecked)
		Renderer::DrawTexture(check, mArea);
	if (hoverOverBox)
	{
		box.SetTint(WHITE);
		check.SetTint(WHITE);
	}
	DrawChildrin();
}


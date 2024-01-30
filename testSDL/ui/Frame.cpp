#include "Frame.h"
#include "../TextureManager.h"

ui::Frame::Frame(Widget* parent, SDL_FRect area, const std::string& name, int textureID, WidgetPostion x, WidgetPostion y, float scale)
	: Widget(parent,area,name,x,y), mScale(scale), mTexId(textureID)
{
}

void ui::Frame::Update()
{
	Widget::Update();
}

void ui::Frame::Draw() 
{
	if (!mActive) 
		return;
	auto texture = TextureManager::Get(mTexId);
	mTexAreaChache = texture.rect;
	const float thirdX = texture.rect.w / 3.0, thirdY = texture.rect.h / 3.0;
	SDL_Rect parts[9]; 
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			parts[j + 3 * i] = { texture.rect.x + j * (int)thirdX,texture.rect.y + i * (int)thirdY, (int)thirdX, (int)thirdY };
		}
	}
	for (float y = 0; y < mArea.h; y+=thirdY*mScale)
	{
		for (float x = 0; x < mArea.w; x+= thirdX*mScale)
		{
			if (y == 0) // top
			{
				if (x == 0) //left
				{
					texture.rect = parts[0];
				}
				else if (x >= mArea.w - thirdX * mScale) //right
				{
					texture.rect = parts[2];
				}
				else // mid
				{
					texture.rect = parts[1];
				}
			}
			else if (y >= mArea.h - thirdY * mScale) // bottom
			{
				if (x == 0)
				{
					texture.rect = parts[6];
				}
				else if (x >= mArea.w - thirdX * mScale)
				{
					texture.rect = parts[8];
				}
				else
				{
					texture.rect = parts[7];
				}
			}
			else //mid
			{
				if (x == 0)
				{
					texture.rect = parts[3];
				}
				else if (x >= mArea.w - thirdX * mScale)
				{
					texture.rect = parts[5];
				}
				else
				{
					texture.rect = parts[4];
				}
			}
			texture.SetTint(mBackgroundTint);
			Renderer::DrawTexture(texture, SDL_FRect( mArea.x + x,mArea.y + y,thirdX*mScale,thirdY * mScale));
			texture.SetTint(WHITE);
		}
	}
	DrawChildrin();
}

void ui::Frame::DebugDraw()
{
	bool flag = mChildrinDebugDrawFlag;
	mChildrinDebugDrawFlag = true;
	Widget::DebugDraw();
	if (flag)
		mChildrinDebugDrawFlag = false;
	DebugDrawChildrin();
}

SDL_FRect ui::Frame::GetInnerRect() const
{
	return { mArea.x + mTexAreaChache.w / 3.0f * mScale, 
		mArea.y + mTexAreaChache.h / 3.0f * mScale,
		mArea.w - 2*(mTexAreaChache.w / 3.0f * mScale),
		mArea.h - 2*(mTexAreaChache.h / 3.0f * mScale)};
}

ui::Frame* ui::Frame::SetScale(float scale)
{
	mScale = scale;
	return this;
}

ui::Frame* ui::Frame::SetTexture(int textureId)
{
	mTexId = textureId;
	return this;
}

float ui::Frame::GetScale()
{
	return mScale;
}

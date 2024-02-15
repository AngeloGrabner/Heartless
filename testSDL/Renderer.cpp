#include "Renderer.h"

SDL_Renderer* Renderer::sRen = nullptr;
SDL_Rect Renderer::sViewPort = { 0,0,0,0 };
SDL_FPoint Renderer::sScale = { 1.0f,1.0f };
SDL_FPoint Renderer::sOffset = { 0.0f,0.0f };
bool Renderer::sScreenToWorld = false;
std::vector<std::function<void()>> Renderer::sDelayedCalls;

bool Renderer::Init(SDL_Window* window)
{
	sRen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!sRen)
	{
		std::cout << "SDL error: " << SDL_GetError();
		return false;
	}
	SDL_RenderGetViewport(sRen, &sViewPort);
	return true;
}
void Renderer::Free()
{
	SDL_DestroyRenderer(sRen);
}

SDL_Renderer* Renderer::Get()
{
	return sRen;
}
void Renderer::SetViewport(int x, int y, int w, int h)
{
	if (w == -1)
		w = sViewPort.w;
	if (h == -1)
		h = sViewPort.h;
	SDL_Rect r = { x,y,w,h };
	SDLCHECK(SDL_RenderSetViewport(sRen, &r));
	sViewPort = r;
}
SDL_Rect Renderer::GetViewport()
{
	return sViewPort;
}
void Renderer::DrawTexture(Texture texture, const SDL_FRect& pos)
{
	auto p = Transform(pos);
	SDLCHECK(SDL_RenderCopyF(sRen, texture.tex, &texture.rect, &p));
}
void Renderer::DrawTexture(Texture texture, const SDL_FRect& pos, double angle, const SDL_FPoint& center, SDL_RendererFlip flip)
{
	auto p = Transform(pos);
	if (center.x == FLT_MAX && center.y == FLT_MAX)
	{
		SDLCHECK(SDL_RenderCopyExF(sRen, texture.tex, &texture.rect, &p, angle, NULL, flip));
	}
	else
	{
		SDLCHECK(SDL_RenderCopyExF(sRen, texture.tex, &texture.rect, &p, angle, &center, flip));
	}
}
void Renderer::DrawTexture(Texture texture, const SDL_Rect& pos)
{
	auto p = Transform(pos);
	SDLCHECK(SDL_RenderCopy(sRen, texture.tex, &texture.rect, &p));
}
void Renderer::DrawTexture(Texture texture, const SDL_Rect& pos, double angle, const SDL_Point& center, SDL_RendererFlip flip)
{
	auto p = Transform(pos);
	if (center.x == INT_MAX && center.y == INT_MAX)
	{
		SDLCHECK(SDL_RenderCopyEx(sRen, texture.tex, &texture.rect, &p, angle, NULL, flip));
	}
	else
	{
		SDLCHECK(SDL_RenderCopyEx(sRen, texture.tex, &texture.rect, &p, angle, &center, flip));
	}
}
void Renderer::DelayedDraw(std::function<void(void)> func)
{
	sDelayedCalls.push_back(func);
}
void Renderer::Clear()
{
	sDelayedCalls.clear();
	SDLCHECK(SDL_RenderClear(sRen));
}
void Renderer::SetBlendMode(SDL_BlendMode mode)
{
	SDLCHECK(SDL_SetRenderDrawBlendMode(sRen, mode));
}
SDL_BlendMode Renderer::GetBlendMode()
{
	SDL_BlendMode b;
	SDLCHECK(SDL_GetRenderDrawBlendMode(sRen, &b));
	return b;
}
void Renderer::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	SDLCHECK(SDL_SetRenderDrawColor(sRen, r, g, b, a));
}
void Renderer::SetColor(Color c)
{
	SetColor(c.r, c.g, c.b, c.a);
}
Color Renderer::GetColor()
{
	Color c;
	SDLCHECK(SDL_GetRenderDrawColor(sRen, &c.r, &c.g, &c.b, &c.a));
	return c;
}
void Renderer::UpdateScreen()
{
	for (int i = 0; i < sDelayedCalls.size(); i++)
	{
		sDelayedCalls[i]();
	}
	SDL_RenderPresent(sRen);
}
void Renderer::SetTarget(SDL_Texture* texture)
{
	SDLCHECK(SDL_SetRenderTarget(sRen, texture));
}
void Renderer::SetVSync(VSync v)
{
	SDLCHECK(SDL_RenderSetVSync(sRen, (int)v));
}

void Renderer::SetZoom(SDL_FPoint zoom)
{
	sScale = zoom;
}

void Renderer::SetOffset(SDL_FPoint offset)
{
	sOffset = offset;
}

void Renderer::SetScreenMode(bool drawWorld)
{
	sScreenToWorld = drawWorld;
}

bool Renderer::GetScreenMode()
{
	return sScreenToWorld;
}

void Renderer::DrawRect(const SDL_Rect& rect)
{
	auto p = Transform(rect);
	SDLCHECK(SDL_RenderDrawRect(sRen, &p));
}
void Renderer::DrawRects(const SDL_Rect* rects, int count)
{
	SDL_Rect* r = new SDL_Rect[count];
	for (int i = 0; i < count; i++)
		r[i] = Transform(rects[i]);
	SDLCHECK(SDL_RenderDrawRects(sRen, r, count));
	delete[] r;
}
void Renderer::DrawLine(SDL_Point p1, SDL_Point p2)
{
	p1 = Transform(p1);
	p2 = Transform(p2);
	SDLCHECK(SDL_RenderDrawLine(sRen, p1.x, p1.y, p2.x, p2.y));
}
void Renderer::DrawLines(const SDL_Point* points, int count)
{
	SDL_Point* p = new SDL_Point[count];
	for (int i = 0; i < count; i++)
		p[i] = Transform(points[i]);
	SDLCHECK(SDL_RenderDrawLines(sRen, p, count));
	delete[] p;
}
void Renderer::DrawPoint(SDL_Point p)
{
	p = Transform(p);
	SDLCHECK(SDL_RenderDrawPoint(sRen, p.x, p.y));
}
void Renderer::DrawPoints(SDL_Point* points, int count)
{
	SDL_Point* p = new SDL_Point[count];
	for (int i = 0; i < count; i++)
		p[i] = Transform(points[i]);
	SDLCHECK(SDL_RenderDrawPoints(sRen, p, count));
	delete[] p;
}
void Renderer::FillRect(const SDL_Rect& rect)
{
	auto r = Transform(rect);
	SDLCHECK(SDL_RenderFillRect(sRen, &r));
}
void Renderer::FillRects(const SDL_Rect* rects, int count)
{
	SDL_Rect* r = new SDL_Rect[count];
	for (int i = 0; i < count; i++)
		r[i] = Transform(rects[i]);
	SDLCHECK(SDL_RenderFillRects(sRen, r, count));
	delete[] r;
}
void Renderer::DrawRect(const SDL_FRect& rect)
{
	auto r = Transform(rect);
	SDLCHECK(SDL_RenderDrawRectF(sRen, &r));
}
void Renderer::DrawRects(const SDL_FRect* rects, int count)
{
	SDL_FRect* r = new SDL_FRect[count];
	for (int i = 0; i < count; i++)
		r[i] = Transform(rects[i]);
	SDLCHECK(SDL_RenderDrawRectsF(sRen, r, count));
	delete[] r;
}
void Renderer::DrawLine(SDL_FPoint p1, SDL_FPoint p2)
{
	p1 = Transform(p1);
	p2 = Transform(p2);
	SDLCHECK(SDL_RenderDrawLineF(sRen, p1.x, p1.y,p2.x,p2.y));
}
void Renderer::DrawLines(const SDL_FPoint* points, int count)
{
	SDL_FPoint* p = new SDL_FPoint[count];
	for (int i = 0; i < count; i++)
		p[i] = Transform(points[i]);
	SDLCHECK(SDL_RenderDrawLinesF(sRen, p, count));
	delete[] p;
}
void Renderer::DrawPoint(SDL_FPoint p)
{
	p = Transform(p);
	SDLCHECK(SDL_RenderDrawPointF(sRen, p.x,p.y));
}
void Renderer::DrawPoints(const SDL_FPoint* points, int count)
{
	SDL_FPoint* p = new SDL_FPoint[count];
	for (int i = 0; i < count; i++)
		p[i] = Transform(points[i]);
	SDLCHECK(SDL_RenderDrawPointsF(sRen, p, count));
	delete[] p;
}
void Renderer::FillRect(const SDL_FRect& rect)
{
	auto r = Transform(rect);
	SDLCHECK(SDL_RenderFillRectF(sRen, &r));
}
void Renderer::FillRects(const SDL_FRect* rects, int count)
{
	SDL_FRect* r = new SDL_FRect[count];
	for (int i = 0; i < count; i++)
		r[i] = Transform(rects[i]);
	SDLCHECK(SDL_RenderFillRectsF(sRen, rects, count));
	delete[] r;
}

SDL_Rect Renderer::Transform(SDL_Rect rect)
{
	if (sScreenToWorld)
	{
		rect.w = (int)std::round(rect.w * sScale.x);
		rect.h = (int)std::round(rect.h * sScale.y);

		rect.x -= (int)std::round(sOffset.x);
		rect.y -= (int)std::round(sOffset.y);

		rect.x = (int)std::round(rect.x * sScale.x);
		rect.y = (int)std::round(rect.y * sScale.y);
	}
	return rect;
}

SDL_FRect Renderer::Transform(SDL_FRect rect)
{
	if (sScreenToWorld)
	{
		rect.w *= sScale.x;
		rect.h *= sScale.y;

		rect.x -= sOffset.x;
		rect.y -= sOffset.y;

		rect.x *= sScale.x;
		rect.y *= sScale.y;
	}
	return rect;
}

SDL_Point Renderer::Transform(SDL_Point point)
{
	if (sScreenToWorld)
	{
		point.x -= (int)std::round(sOffset.x);
		point.y -= (int)std::round(sOffset.y);

		point.x = (int)std::round(point.x * sScale.x);
		point.y = (int)std::round(point.y * sScale.y);
	}
	return point;
}

SDL_FPoint Renderer::Transform(SDL_FPoint point)
{
	if (sScreenToWorld)
	{
		point.x -= sOffset.x;
		point.y -= sOffset.y;

		point.x *= sScale.x;
		point.y *= sScale.y;
	}
	return point;
}

#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <mutex>
#include <algorithm>

inline std::vector<std::string> loger;
inline std::mutex logerMut;

#ifdef _DEBUG
	#define DEBUG
#endif // _DEBUG

#if _DEBUG
	#define DB_OUT(msg) std::cout << msg << std::endl
#else
	#define DB_OUT(msg)
#endif 


#define LOG_STR(msg) (std::string((msg)) + " file: " + __FILE__ + " line: " + std::to_string(__LINE__))
#define LOG_PUSH(msg) logerMut.lock();\
		 loger.push_back(LOG_STR(msg)); \
			logerMut.unlock()

#define LOG_PRINT logerMut.lock();\
	for (int i= 0; i < loger.size(); i++) \
	{ \
		std::cout << loger[i] << '\n'; \
	} \
	logerMut.unlock()

#ifdef _DEBUG
	#define SDLCHECK(returnVal) if ((returnVal) < 0) {std::cout << "SDL error: " << SDL_GetError() << LOG_STR(" at") <<'\n'; SDL_assert(false);} //else {std::cout << "Success " << LOG(" at") << '\n';}
#else
	#define SDLCHECK(returnVal) (returnVal)
#endif

// just there to specefy  that a no op is intended
#define NO_OP do {} while (0)

template<typename T = float>
constexpr T lerp(T start, T stop, T scale)
{
	return ((stop-start) * scale + start);
}
template<typename T = float>
constexpr T invlerp(T start, T stop, T step)
{
	return ((step - start) / (stop-start));
}

template<typename T>
concept RECT = requires(T t)
{
	t.x;
	t.y;
	t.w;
	t.h;
};

template<class Archive, RECT Rect>
void serialize(Archive& ar, Rect& rect)
{
	ar(rect.x, rect.y, rect.w, rect.h);
}

template<typename T>
concept POINT = requires(T t)
{
	t.x;
	t.y;
};

template<RECT Rect>
constexpr bool overlape(Rect a, Rect b)
{
	return (a.x + a.w >= b.x &&
		a.x <= b.x + b.w &&
		a.y + a.h >= b.y &&
		a.y <= b.y + b.h);
}

template<RECT Rect>
constexpr bool contain(Rect outer, Rect inner)
{
	return (outer.x <= inner.x &&
		outer.x + outer.w >= inner.x + inner.w &&
		outer.y <= inner.y &&
		outer.y + outer.h >= inner.y + inner.h);
}

template<RECT Rect, POINT Point>
constexpr bool contain(Rect outer, Point inner)
{
	return (outer.x <= inner.x && outer.x + outer.w >= inner.x&&
		outer.y <= inner.y && outer.y + outer.h >= inner.y);
}

template<RECT Rect>
inline std::string to_string(Rect r)
{
	return ("x: " + std::to_string(r.x) + " y: " + std::to_string(r.y) + " w: " + std::to_string(r.w) + " h: " + std::to_string(r.h));
}

template<RECT RectIn, RECT RectOut>
inline RectOut cast(RectIn rect)
{
	return RectOut(std::round(rect.x), std::round(rect.y), std::round(rect.w), std::round(rect.h));
}

template<POINT Point,RECT Rect>
constexpr Point mid(Rect rect)
{
	return Point(rect.x + rect.w / 2.0f, rect.y + rect.h / 2.0f);
}

template<POINT Ponit>
constexpr Ponit norm(Ponit p)
{
	float len = std::sqrtf(p.x * p.x + p.y * p.y);
	p.x /= len;
	p.y /= len;
	return p;
}

inline int random(int max, int min = 0)
{
	return rand() % (max - min) + min;
}

inline float randomF(float max, float min = 0.0f)
{
	return rand() / (float)RAND_MAX * (max - min) + min;
}

inline long long randomLL(long long max, long long min = 0)
{
	long long ret = rand();
	ret << sizeof(int) * 8;
	ret |= rand();
	return ret % (max - min) + min;
}

std::string& lower(std::string& str);

std::string& upper(std::string& str);
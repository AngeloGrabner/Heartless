#pragma once
#include "SDL.h"
#include "Utility.h"
#include <unordered_map>
#include <memory>

class Event
{
public:
	enum Type : uint8_t
	{
		UI_EVENT,
		CAMERA_EVENT,

		//...
		LAST_EVENT // dont use this. it is for init 
	};
private:
	static std::unordered_map<Type, Uint32> sMap;
	static Uint32 AddEvent(Event::Type Id); // registers a event
public:
	static void Init();
	static Uint32 GetEventType(Type Id); // event lookup	
};
//buildas and pushes events 
namespace EventBuilder
{
	void Button(const std::string& name);
	void CheckBox(const std::string& name, bool checked);
	void InputField(const std::string& name,const std::string& text);
	void Slider(const std::string& name, float val);

	void CameraTargetRect(SDL_FRect rect);
	void CameraAbsPos(SDL_FPoint pos);
	void CameraRelPos(SDL_FPoint pos);
	void CameraScaleAbs(SDL_FPoint scale);
	void CameraScaleRel(SDL_FPoint scale);

	void PlayerDied(size_t id); //todo
}
namespace EventReceiver
{
	bool Button(const SDL_Event* e, std::string& name);
	bool CheckBox(const SDL_Event* e, std::string& name, bool& checked);
	bool InputField(const SDL_Event* e, std::string& name, std::string& text);
	bool Slider(const SDL_Event* e, std::string& name, float& value);

	bool CameraTargetRect(const SDL_Event* e, SDL_FRect& rect);
	bool CameraAbsPos(const SDL_Event* e, SDL_FPoint& pos);
	bool CameraRelPos(const SDL_Event* e, SDL_FPoint& pos);
	bool CameraScaleAbs(const SDL_Event* e, SDL_FPoint& scale);
	bool CameraScaleRel(const SDL_Event* e, SDL_FPoint& scale);
}
void UserEventDeallocator(SDL_Event* e);
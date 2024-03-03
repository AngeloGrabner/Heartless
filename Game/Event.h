#pragma once
#include "SDL.h"
#include "Utility.h"
#include <unordered_map>
#include <memory>

class Entity;

class Event
{
public:
	enum Type : uint8_t
	{
		UI_EVENT,
		RE_UI_EVENT,
		CAMERA_EVENT,
		EDITOR_EVENT,
		SETTINGS_EVENT,
		SCENE_EVENT,
		//... game evetns
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
	void Select(const std::string& name, const std::string& selected);
	
	void ReInputField(const std::string& name, const std::string& text);
	void ReLabel(const std::string& name, const std::string& text);
	void ReTextBox(const std::string& name, const std::string& text);
	void ReCheckBox(const std::string& name, bool val);

	void SettingsChanged();

	//todo add carryover list 
	void StartSceneChange(const std::string& sceneName, const SDL_FPoint& destPos, const std::list<std::shared_ptr<Entity>>& carryOver);
	void FinishedSceneChange();

	void PlayerDied(size_t id); //todo
}

namespace EventReceiver
{
	bool Button(const SDL_Event* e, std::string& name);
	bool CheckBox(const SDL_Event* e, std::string& name, bool& checked);
	bool InputField(const SDL_Event* e, std::string& name, std::string& text);
	bool Slider(const SDL_Event* e, std::string& name, float& value);
	bool Select(const SDL_Event* e, std::string& name, std::string& selected);

	bool ReIputField(const SDL_Event* e,std::string& name, std::string& text);
	bool ReLabel(const SDL_Event* e,std::string& name, std::string& text);
	bool ReTextBox(const SDL_Event* e,std::string& name, std::string& text);
	bool ReCheckBox(const SDL_Event* e, std::string& name, bool& val);

	bool SettingsChanged(const SDL_Event* e);

	//todo add carryover list 
	bool StartSceneChange(const SDL_Event* e, std::string& sceneName, SDL_FPoint& destPos, std::list<std::shared_ptr<Entity>>& carryOver);
	bool FinishedSceneChange(const SDL_Event* e);
}
void UserEventDeallocator(SDL_Event* e);
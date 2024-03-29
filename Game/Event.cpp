#include "Event.h"
#include "Utility.h"

std::unordered_map<Event::Type, Uint32> Event::sMap;

Uint32 Event::AddEvent(Event::Type Id)
{
    auto eventType = SDL_RegisterEvents(1);
    
    SDL_assert(eventType != (Uint32)-1); //SDL Can't add Event Type

    sMap[Id] = eventType;
    return eventType;
}

void Event::Init()
{
    for (unsigned int i = 0; i != (unsigned int)Type::LAST_EVENT; i++)
    {
        AddEvent((Type)i);
    }
}

Uint32 Event::GetEventType(Event::Type Id)
{
    return sMap.at(Id);
}

#define DO_EVENT(x) SDL_Event e;\
                SDL_zero(e); \
                e.type = Event::GetEventType((x))
#define PUSH SDLCHECK(SDL_PushEvent(&e))

void EventBuilder::Button(const std::string& name)
{
    DO_EVENT(Event::UI_EVENT);
    e.user.code = 0;
    e.user.data1 = new std::string(name);
    PUSH;
}

void EventBuilder::CheckBox(const std::string& name, bool checked)
{
    DO_EVENT(Event::UI_EVENT);
    e.user.code = 1;
    e.user.data1 = new std::string(name);
    e.user.data2 = new bool(checked);
    PUSH;
}

void EventBuilder::InputField(const std::string& name,const std::string& text)
{
    DO_EVENT(Event::UI_EVENT);
    e.user.code = 2;
    e.user.data1 = new std::string(name);
    e.user.data2 = new std::string(text);
    PUSH;
}

void EventBuilder::Slider(const std::string& name, float val)
{
    DO_EVENT(Event::UI_EVENT);
    e.user.code = 3;
    e.user.data1 = new std::string(name);
    e.user.data2 = new float(val);
    PUSH;
}

void EventBuilder::Select(const std::string& name, const std::string& selected)
{
    DO_EVENT(Event::UI_EVENT);
    e.user.code = 4;
    e.user.data1 = new std::string(name);
    e.user.data2 = new std::string(selected);
    PUSH;
}

void EventBuilder::ReInputField(const std::string& name, const std::string& text)
{
    DO_EVENT(Event::RE_UI_EVENT);
    e.user.code = 0;
    e.user.data1 = new std::string(name);
    e.user.data2 = new std::string(text);
    PUSH;
}

void EventBuilder::ReLabel(const std::string& name, const std::string& text)
{
    DO_EVENT(Event::RE_UI_EVENT);
    e.user.code = 1;
    e.user.data1 = new std::string(name);
    e.user.data2 = new std::string(text);
    PUSH;
}

void EventBuilder::ReTextBox(const std::string& name, const std::string& text)
{
    DO_EVENT(Event::RE_UI_EVENT);
    e.user.code = 2;
    e.user.data1 = new std::string(name);
    e.user.data2 = new std::string(text);
    PUSH;
}

void EventBuilder::ReCheckBox(const std::string& name, bool val)
{
    DO_EVENT(Event::RE_UI_EVENT);
    e.user.code = 3;
    e.user.data1 = new std::string(name);
    e.user.data2 = new bool(val);
    PUSH;
}

void EventBuilder::SettingsChanged()
{
    DO_EVENT(Event::SETTINGS_EVENT);
    e.user.code = 0;
    PUSH;
}

void EventBuilder::StartSceneChange(const std::string& sceneName, const SDL_FPoint& destPos, const std::list<std::shared_ptr<Entity>>& carryOver)
{
    DO_EVENT(Event::SCENE_EVENT);
    e.user.code = 0;
    e.user.data1 = new std::pair(std::string(sceneName),SDL_FPoint(destPos));
    e.user.data2 = new std::list<std::shared_ptr<Entity>>(carryOver);
    PUSH;
}

void EventBuilder::FinishedSceneChange()
{
    DO_EVENT(Event::SCENE_EVENT);
    e.user.code = 1;
    PUSH;
}


void EventBuilder::PlayerDied(size_t id)
{
   
}



#undef DO_EVENT
#undef PUSH
#define IF(x,y) if (e->type == Event::GetEventType((x)) && e->user.code == (y)) 
#define ELIF(x,y) else if (e->type == Event::GetEventType((x)) && e->user.code == (y)) 

bool EventReceiver::Button(const SDL_Event* e, std::string& name)
{
    IF(Event::UI_EVENT, 0)
    {
        name = *(std::string*)(e->user.data1);
        return true;
    }
    return false;
}

bool EventReceiver::CheckBox(const SDL_Event* e, std::string& name, bool& checked)
{
    IF(Event::UI_EVENT, 1)
    {
        name = *(std::string*)(e->user.data1);
        checked = *(bool*)(e->user.data2);
        return true;
    }
    return false;
}

bool EventReceiver::InputField(const SDL_Event* e, std::string& name, std::string& text)
{
    IF(Event::UI_EVENT, 2)
    {
        name = *(std::string*)(e->user.data1);
        text = *(std::string*)(e->user.data2);
        return true;
    }
    return false;
}

bool EventReceiver::Slider(const SDL_Event* e, std::string& name, float& value)
{
    IF(Event::UI_EVENT, 3)
    {
        name = *(std::string*)(e->user.data1);
        value = *(float*)(e->user.data2);
        return true;
    }
    return false;
}

bool EventReceiver::Select(const SDL_Event* e, std::string& name, std::string& selected)
{
    IF(Event::UI_EVENT, 4)
    {
        name = *(std::string*)(e->user.data1);
        selected =  *(std::string*)(e->user.data2);
        return true;
    }
    return false;
}

bool EventReceiver::ReIputField(const SDL_Event* e, std::string& name, std::string& text)
{
    IF(Event::RE_UI_EVENT, 0)
    {
        name = *(std::string*)(e->user.data1);
        text = *(std::string*)(e->user.data2);
        return true;
    }
    return false;
}

bool EventReceiver::ReLabel(const SDL_Event* e, std::string& name,  std::string& text)
{
    IF(Event::RE_UI_EVENT, 1)
    {
        name = *(std::string*)(e->user.data1);
        text = *(std::string*)(e->user.data2);
        return true;
    }
    return false;
}

bool EventReceiver::ReTextBox(const SDL_Event* e,  std::string& name,  std::string& text)
{
    IF(Event::RE_UI_EVENT, 2)
    {
        name = *(std::string*)(e->user.data1);
        text = *(std::string*)(e->user.data2);
        return true;
    }
    name = *(std::string*)(e->user.data1);
    text = *(std::string*)(e->user.data2);
    return false;
}

bool EventReceiver::ReCheckBox(const SDL_Event* e, std::string& name, bool& val)
{
    IF(Event::RE_UI_EVENT, 3)
    {
        name = *(std::string*)(e->user.data1);
        val = *(bool*)(e->user.data2);
        return true;
    }
    return false;
}

bool EventReceiver::SettingsChanged(const SDL_Event* e)
{
    IF(Event::SETTINGS_EVENT, 0)
    {
        return true;
    }
    return false;
}

bool EventReceiver::StartSceneChange(const SDL_Event* e, std::string& sceneName, SDL_FPoint& destPos, std::list<std::shared_ptr<Entity>>& carryOver)
{
    IF(Event::SCENE_EVENT, 0)
    {
        auto temp = *(std::pair<std::string,SDL_FPoint>*)(e->user.data1);
        
        sceneName = temp.first;
        destPos = temp.second;
        
        carryOver.clear();
        carryOver = *(std::list<std::shared_ptr<Entity>>*)(e->user.data2);

        return true;
    }
    return false;
}

bool EventReceiver::FinishedSceneChange(const SDL_Event* e)
{
    IF(Event::SCENE_EVENT, 1)
    {
        return true;
    }
    return false;
}



void UserEventDeallocator(SDL_Event* e)
{
    bool flag = true;
    IF(Event::UI_EVENT, 0)
    {
        delete (std::string*)e->user.data1;
    }
    ELIF(Event::UI_EVENT, 1)
    {
        delete (std::string*)e->user.data1;
        delete (bool*)e->user.data2;
    }
    ELIF(Event::UI_EVENT, 2)
    {
        delete (std::string*)e->user.data1;
        delete (std::string*)e->user.data2;
    }
    ELIF(Event::UI_EVENT, 3)
    {
        delete (std::string*)e->user.data1;
        delete (float*)e->user.data2;
    }
    ELIF(Event::UI_EVENT, 4)
    {
        delete (std::string*)(e->user.data1);
        delete (std::string*)(e->user.data2);
    }
    ELIF(Event::RE_UI_EVENT, 0)
    {
        delete (std::string*)(e->user.data1);
        delete (std::string*)(e->user.data2);
    }
    ELIF(Event::RE_UI_EVENT, 1)
    {
        delete (std::string*)(e->user.data1);
        delete (std::string*)(e->user.data2);
    }
    ELIF(Event::RE_UI_EVENT, 2)
    {
        delete (std::string*)(e->user.data1);
        delete (std::string*)(e->user.data2);
    }
    ELIF(Event::RE_UI_EVENT, 3)
    {
        delete (std::pair<std::string, SDL_FPoint>*)(e->user.data1);
        delete (bool*)(e->user.data2);
    }
    ELIF(Event::SETTINGS_EVENT, 0)
    {
        NO_OP;
    }
    ELIF(Event::SCENE_EVENT, 0)
    {
        delete (std::string*)(e->user.data1);
    }
    ELIF(Event::SCENE_EVENT, 1)
    {
        NO_OP;
    }
    else 
        flag = false;

    SDL_assert(!(e->type >= SDL_USEREVENT && e->type < SDL_LASTEVENT && !flag));
}
#undef IF
#undef ELIF
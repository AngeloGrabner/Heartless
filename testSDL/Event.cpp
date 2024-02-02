#include "Event.h"

std::unordered_map<Event::Type, Uint32> Event::sMap;

Uint32 Event::AddEvent(Event::Type Id)
{
    auto eventType = SDL_RegisterEvents(1);
    if (eventType == (Uint32)-1)
        throw std::exception(LOG_STR("SDL Can't add Event Type").c_str());
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
#define PUSH SDL_PushEvent(&e)

void EventBuilder::Button(const std::string& name)
{
    DO_EVENT(Event::UI_EVENT);
    e.user.code = 0;
    e.user.data1 = new std::string(name);
    SDL_PushEvent(&e);
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

void EventBuilder::CameraTargetRect(SDL_FRect rect)
{
    DO_EVENT(Event::CAMERA_EVENT);
    e.user.code = 0;
    e.user.data1 = new SDL_FRect(rect);
    PUSH;
}

void EventBuilder::CameraAbsPos(SDL_FPoint pos)
{
    DO_EVENT(Event::CAMERA_EVENT);
    e.user.code = 1;
    e.user.data1 = new SDL_FPoint(pos);
    PUSH;
}

void EventBuilder::CameraRelPos(SDL_FPoint pos)
{
    DO_EVENT(Event::CAMERA_EVENT);
    e.user.code = 2;
    e.user.data1 = new SDL_FPoint(pos);
    PUSH;
}

void EventBuilder::CameraScaleAbs(SDL_FPoint scale)
{
    DO_EVENT(Event::CAMERA_EVENT);
    e.user.code = 3;
    e.user.data1 = new SDL_FPoint(scale);
    PUSH;
}

void EventBuilder::CameraScaleRel(SDL_FPoint scale)
{
    DO_EVENT(Event::CAMERA_EVENT);
    e.user.code = 4;
    e.user.data1 = new SDL_FPoint(scale);
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

bool EventReceiver::CameraTargetRect(const SDL_Event* e,SDL_FRect& rect)
{
    IF(Event::CAMERA_EVENT, 0)
    {
        rect = *(SDL_FRect*)e->user.data1;
        return true;
    }
    return false;
}

bool EventReceiver::CameraAbsPos(const SDL_Event* e,SDL_FPoint& pos)
{
    IF(Event::CAMERA_EVENT, 1)
    {
        pos = *(SDL_FPoint*)e->user.data1;
        return true;
    }
    return false;
}

bool EventReceiver::CameraRelPos(const SDL_Event* e,SDL_FPoint& pos)
{
    IF(Event::CAMERA_EVENT, 2)
    {
        pos = *(SDL_FPoint*)e->user.data1;
        return true;
    }
    return false;
}

bool EventReceiver::CameraScaleAbs(const SDL_Event* e,SDL_FPoint& scale)
{
    IF(Event::CAMERA_EVENT, 3)
    {
        auto s = (SDL_FPoint*)e->user.data1;
        scale = *s;
        return true;
    }
    return false;
}

bool EventReceiver::CameraScaleRel(const SDL_Event* e,SDL_FPoint& scale)
{
    IF(Event::CAMERA_EVENT, 4)
    {
        scale = *(SDL_FPoint*)e->user.data1;
        return true;
    }
    return false;
}




void UserEventDeallocator(SDL_Event* e)
{
    bool flag = false;
    IF(Event::UI_EVENT, 0)
    {
        delete (std::string*)e->user.data1;
        flag = true;
    }
    ELIF(Event::UI_EVENT, 1)
    {
        delete (std::string*)e->user.data1;
        delete (bool*)e->user.data2;
        flag = true;
    }
    ELIF(Event::UI_EVENT, 2)
    {
        delete (std::string*)e->user.data1;
        delete (std::string*)e->user.data2;
        flag = true;
    }
    ELIF(Event::UI_EVENT, 3)
    {
        delete (std::string*)e->user.data1;
        delete (float*)e->user.data2;
        flag = true;
    }
    ELIF(Event::CAMERA_EVENT, 0)
    {
        delete (SDL_FRect*)e->user.data1; // crash
        flag = true;
    }
    else if (e->type == Event::GetEventType(Event::CAMERA_EVENT) && 
        (e->user.code == 1 || e->user.code == 2 ||
         e->user.code == 3 || e->user.code == 4))
    {
        delete (SDL_FPoint*)e->user.data1;
        flag = true;
    }


#if _DEBUG
    if (e->type >= SDL_USEREVENT && e->type < SDL_LASTEVENT && !flag)
    {
        //mem leak 
        throw;
    }
#endif
}
#undef IF
#undef ELIF
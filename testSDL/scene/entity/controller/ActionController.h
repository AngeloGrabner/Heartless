#pragma once
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp> // meight be needed for some smart ptrs 
#include <cereal/types/base_class.hpp>
#include "../../../sdl_type_serializer.h"

//todo: make to thread safe
class ActionController
{
protected:
	SDL_FPoint mDir = { 0,0 };
	bool mUseItem = false;
	int mIndex = 0; 
public:
	ActionController() = default;

	virtual void Update() = 0;
	SDL_FPoint GetDir();
	bool UseItem(int& index);

	//to do for creature to work, also do all derived classes
	template<class Archive>
	void save(Archive& ar) const;
	template<class Archive>
	void load(Archive& ar);
};

template<class Archive>
inline void ActionController::save(Archive& ar) const
{
	ar(mDir, mUseItem, mIndex);
}

template<class Archive>
inline void ActionController::load(Archive& ar)
{
	ar(mDir, mUseItem, mIndex);
}

#include "ActionController.h"

SDL_FPoint ActionController::GetDir()
{
    return mDir;
}

bool ActionController::UseItem(int& index)
{
    index = mIndex;
    return mUseItem;
}

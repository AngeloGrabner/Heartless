#include "Item.h"

Item::Item(int texId, const std::string& name, int maxCount)
    : mTexId(texId), mMaxCount(maxCount), mName(name)
{
}

Item::Item(std::unique_ptr<Animation>&& ani, const std::string& name, int maxCount)
    : mAni(std::move(ani)), mMaxCount(maxCount), mName(name)
{
}

bool Item::Comp(const Item& other)
{
    if (this->Type == other.Type &&
        mTexId == other.mTexId && 
        mAni == other.mAni &&
        mName == other.mName &&
        mDiscription == other.mDiscription)
    {
        return true;
    }
    return false;
}

bool Item::operator=(const Item& other)
{
    return Comp(other);
}

int Item::GetCount() const
{
    return mCount;
}

Item* Item::SetCount(int count)
{
    mCount = count;
    return this;
}

const std::string& Item::GetName() const
{
    return mName;
}

Item* Item::SetName(const std::string& name)
{
    mName = name;
    return this;
}

const std::string& Item::GetDiscription() const
{
    return mDiscription;
}

Item* Item::SetDiscription(const std::string& text)
{
    mDiscription = text;
    return this;
}

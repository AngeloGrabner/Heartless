#pragma once
#include <string>
#include "../../../Animation.h"
#include <memory>

#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>

#define ITEM_TYPE static constexpr size_t Type = __COUNTER__

class Entity;

class Item
{
public:
	ITEM_TYPE;
protected:
	int mTexId = -1;
	int mCount = -1;
	int mMaxCount = -1;
	std::unique_ptr<Animation> mAni;
	std::string mName, mDiscription;
public:
	Item() = default;
	Item(int texId, const std::string& name ,int maxCount = 16);
	Item(std::unique_ptr<Animation>&& ani, const std::string& name, int maxCount = 16);

	virtual bool Use(Entity*) = 0;
	virtual bool Comp(const Item& other);


	bool operator =(const Item& other);


	int GetCount() const;
	Item* SetCount(int count);

	const std::string& GetName() const;
	Item* SetName(const std::string& name);

	const std::string& GetDiscription() const;
	Item* SetDiscription(const std::string& text);

	template<class Archive>
	void save(Archive& ar) const;

	template<class Archive>
	void load(Archive& ar);
};

template<class Archive>
inline void Item::save(Archive& ar) const
{
	ar(mTexId, mCount, mMaxCount,mAni, mName, mDiscription);
}

template<class Archive>
inline void Item::load(Archive& ar)
{
	ar(mTexId, mCount, mMaxCount, mAni, mName, mDiscription);
}

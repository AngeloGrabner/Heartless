#pragma once
#include "Item.h"
#include <cereal/types/vector.hpp>

class Inventory
{
	std::vector<Item*> mInv;
public:
	Inventory(size_t size);
	Inventory(const Inventory&) = delete;
	Inventory(Inventory&& other) noexcept;
	Inventory& operator=(const Inventory&) = delete;
	Inventory& operator=(Inventory&& other) noexcept;
	~Inventory();

	bool Empty() const;
	bool Full() const;

	void Clear();

	bool TryPickup(std::unique_ptr<Item> item);

	size_t GetSize() const;

	std::unique_ptr<Item> Drop(size_t idx);

	//use Drop() if you want to store the ptr
	Item* At(size_t idx);

	template<class Archive>
	void save(Archive& ar) const;
	template<class Archive>
	void load(Archive& ar) const;
};

template<class Archive>
inline void Inventory::save(Archive& ar) const
{
	ar(cereal::make_nvp("count", mInv.size()));
	for (auto ptr : mInv)
		ar(cereal::make_nvp("item", std::unique_ptr<Item>(ptr)));
}

template<class Archive>
inline void Inventory::load(Archive& ar) const
{
	int count = 0;
	ar(count);
	std::unique_ptr<Item> ptr;
	for (int i = 0; i < count; i++)
	{
		ar(ptr);
		mInv.push_back(ptr.release());
	}
}

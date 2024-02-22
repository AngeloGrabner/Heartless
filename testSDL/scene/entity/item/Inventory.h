#pragma once
#include "Item.h"

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

	bool TryPickup(std::unique_ptr<Item> item);

	size_t GetSize() const;

	std::unique_ptr<Item> Drop(size_t idx);

	//use Drop() if you want to store the ptr
	Item* At(size_t idx);

};
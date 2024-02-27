#include "Inventory.h"

Inventory::Inventory(size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		mInv.push_back(nullptr);
	}
}

Inventory::Inventory(Inventory&& other) noexcept
{
	mInv.swap(other.mInv);
}

Inventory& Inventory::operator=(Inventory&& other) noexcept
{
	if (this != &other)
		mInv.swap(other.mInv);
	return *this;
}

Inventory::~Inventory()
{
	for (auto ptr : mInv)
	{
		delete ptr;
	}
}

bool Inventory::Empty() const
{
	for (auto ptr : mInv)
	{
		if (ptr)
		{
			return false;
		}
	}
	return true;
}

bool Inventory::Full() const
{
	for (auto ptr : mInv)
	{
		if (!ptr)
		{
			return false;
		}
	}
	return true;
}

void Inventory::Clear()
{
	for (int i = 0; i < mInv.size(); i++)
	{
		delete mInv[i];
	}
}

bool Inventory::TryPickup(std::unique_ptr<Item> item)
{
	for (auto ptr : mInv)
	{
		if (!ptr)
		{
			ptr = item.release();
			return true;
		}
	}
	return false;
}

size_t Inventory::GetSize() const
{
	return mInv.size();
}

std::unique_ptr<Item> Inventory::Drop(size_t idx)
{
	auto unique = std::unique_ptr<Item>(mInv[idx]);
	mInv[idx] = nullptr;
	return unique;
}

Item* Inventory::At(size_t idx)
{
	return mInv.at(idx);
}

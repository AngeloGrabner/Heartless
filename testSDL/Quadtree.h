#pragma once
#include <list>
#include <memory>
#include <concepts>
#include <array>
#include "Utility.h"
#include <type_traits>
#include <cereal/cereal.hpp>

template<typename T, RECT Rect>
struct Interbundle
{
	T data;
	Rect rect;
};

template<typename T, RECT Rect>
struct QuadTreeItemLocation
{
	std::list<Interbundle<T,Rect>>* leafList = nullptr;
	std::list<Interbundle<T,Rect>>::iterator itemIter;
};
template<typename T, RECT Rect>
class Quadtree
{
public:

	using Ib = Interbundle<T,Rect>;

private:
	int mMaxDepth = -1;
	int mDepth = 0;
	std::list<Ib> mData;
	Rect mChildsArea[4] = {Rect(0,0,0,0),Rect(0,0,0,0),Rect(0,0,0,0),Rect(0,0,0,0) };
	std::unique_ptr<Quadtree> mChilds[4];

public:

	Quadtree()
	{

	}
	Quadtree(Rect size, int maxDepth,int depth = 0)
	{
		float hh = size.h / 2.0f, hw = size.w/2.0f;
		mChildsArea[0] = { size.x, size.y, hw, hh};
		mChildsArea[1] = { size.x + hw, size.y, hw, hh };
		mChildsArea[2] = { size.x, size.y + hh,hw,hh };
		mChildsArea[3] = { size.x + hw, size.y + hh, hw, hh };

		mDepth = depth;
		mMaxDepth = maxDepth;
	}
	Quadtree(const Quadtree& other)
	{
		mMaxDepth = other.mMaxDepth;
		mDepth = other.mDepth;
		mData = other.mData;
		for (int i = 0; i < 4; i++)
		{
			mChildsArea[i] = other.mChildsArea[i];
			if (other.mChilds[i])
			{
				if (!mChilds[i])
				{
					mChilds[i] = std::make_unique<Quadtree<T, Rect>>();
				}
				mChilds[i]->Quadtree(other.mChilds[i]);
			}
		}
	}
	Quadtree(Quadtree&& other) noexcept
	{
		mMaxDepth = other.mMaxDepth;
		mDepth = other.mDepth;
		mData = std::move(other.mData);
		for (int i = 0; i < 4; i++)
		{
			mChildsArea[i] = other.mChildsArea[i];
			if (other.mChilds[i])
			{
				mChilds[i] = other.mChilds[i];
				other.mChilds[i] = nullptr;
			}
		}

	}
	Quadtree& operator=(const Quadtree& other)
	{
		if (this != &other)
		{
			mMaxDepth = other.mMaxDepth;
			mDepth = other.mDepth;
			mData = other.mData;
			for (int i = 0; i < 4; i++)
			{
				mChildsArea[i] = other.mChildsArea[i];
				if (other.mChilds[i])
				{
					if (!mChilds[i])
					{
						mChilds[i] = std::make_unique<Quadtree<T, Rect>>();
					}
					mChilds[i]->Quadtree(other.mChilds[i]);
				}
			}
		}
		return *this;
	}
	Quadtree& operator=(Quadtree&& other) noexcept
	{
		if (this != &other)
		{
			mMaxDepth = other.mMaxDepth;
			mDepth = other.mDepth;
			mData = std::move(other.mData);
			for (int i = 0; i < 4; i++)
			{
				mChildsArea[i] = other.mChildsArea[i];
				if (other.mChilds[i])
				{
					mChilds[i] = std::move(other.mChilds[i]);
					other.mChilds[i] = nullptr;
				}
			}
		}
		return *this;
	}
	~Quadtree() = default;
private:
	void GetAll(std::list<Ib>* data)
	{
		data->insert(data->begin(), mData.begin(), mData.end());
		for (int i = 0; i < 4; i++)
		{
			if (mChilds[i])
			{
				mChilds[i]->GetAll(data);
			}
		}
	}
	void RemoveAll()
	{
		mData.clear();
		for (int i = 0; i < 4; i++)
		{
			if (mChilds[i])
			{
				mChilds[i]->RemoveAll();
			}
		}
	}
public:
	QuadTreeItemLocation<T,Rect> Insert(T item, Rect area)
	{
		bool flag = false;
		for (int i = 0; i < 4; i++)
		{
			if (contain<Rect>(mChildsArea[i], area))
			{
				if (mDepth + 1 < mMaxDepth)
				{
					if (!mChilds[i])
					{
						mChilds[i] = std::make_unique<Quadtree<T, Rect>>(mChildsArea[i], mMaxDepth,mDepth + 1);
					}
					return mChilds[i]->Insert(item, area);
				}
			}
		}
		mData.push_front({ item,area });
		return { &mData,mData.begin()};
	}
	std::list<Ib> Search(Rect area)
	{
		std::list<Ib> data;
		Search(area, &data);
		return data;
	}
	void Search(Rect area, std::list<Ib>* data)
	{
		for (const auto& d : mData)
		{
			if (overlape<Rect>(area, d.rect))
			{
				data->push_front(d);
			}
		}
		for (int i = 0; i < 4; i++)
		{
			if (mChilds[i])
			{
				if (contain<Rect>(area, mChildsArea[i]))
				{
					mChilds[i]->GetAll(data);
				}
				else if (overlape<Rect>(mChildsArea[i], area))
				{
					mChilds[i]->Search(area, data);
				}
			}
		}
	}
	// removes the first element that is equal to item 
	bool Remove(const T& item) 
	{
		for (auto it = mData.begin(); it != mData.end(); it++)
		{
			if ((*it).data == item)
			{
				mData.erase(it);
				return true;
			}
		}
		bool flag = false;
		for (int i = 0; i < 4; i++)
		{
			if (mChilds[i] && ! flag)
			{
				flag |= mChilds[i]->Remove(item);
			}
		}
		return flag;
	}
	void Remove(Rect area)
	{
		for (auto it = mData.begin(); it != mData.end(); it++)
		{
			if (overlape<Rect>(area, (*it).rect))
			{
				mData.erase(it);
			}
		}
		for (int i = 0; i < 4; i++)
		{				
			if (mChilds[i])
			{
				if (contain<Rect>(area, mChildsArea[i]))
				{
					mChilds[i]->RemoveAll();
				}
				else if (overlape<Rect>(mChildsArea[i], area))
				{
					mChilds[i]->Remove(area);
				}
			}
		}
	}

	Rect GetSize() const
	{
		Rect r = mChildsArea[0];
		r.w *= 2;
		r.h *= 2;
		return r;
	}
	int GetMaxDepth() const
	{
		return mMaxDepth;
	}
};



template<typename T, RECT Rect, size_t MAX_SIZE = 10000>
class DynamicQuadTree
{
public:
	template<typename T, RECT RRect, size_t MAX_SIZE>
	struct QuadTreeItem
	{
		bool empty = true;
		typename T item;
		using ITER = std::array<QuadTreeItem<T, Rect, MAX_SIZE>, MAX_SIZE>::iterator;
		typename QuadTreeItemLocation<ITER, Rect> ptr;
	};
	using QTI = QuadTreeItem<T, Rect,MAX_SIZE>;
	using ITER = std::array<QTI, MAX_SIZE>::iterator;
	
private:
	std::array<QTI, MAX_SIZE> mData;
	ITER mFreeSpace = mData.begin();
	Quadtree<ITER, Rect> mQt;
public:
	DynamicQuadTree() = default;
	DynamicQuadTree(Rect size, int maxDepth, int depth = 0)
	{
		mQt = Quadtree<ITER, Rect>(size, maxDepth, depth);
	}
	bool Insert(T item, Rect area)
	{
		QTI qtItem;
		qtItem.item = item;
		qtItem.empty = false;
		bool flag = false;
		auto it = mFreeSpace; 
		while (it != mData.end() && !(*it).empty)
		{
			it++;
		}
		if (it == mData.end())
		{
			auto itr = mData.begin();
			while ( itr != mFreeSpace && !(*itr).empty)
			{ 
				itr++;
			}
			if (itr == mFreeSpace)
				return false;
			it = itr;
		}
		mFreeSpace = std::next(it);
		(*it) = qtItem;
		(*it).ptr = mQt.Insert(it, area);
		return true;
	}
	std::list<Interbundle<ITER,Rect>> Search(Rect area)
	{
		return mQt.Search(area);
	}
	void Remove(ITER item)
	{
		(*item).ptr.leafList->erase((*item).ptr.itemIter);
		if constexpr (std::is_destructible<T>() || std::is_trivially_destructible<T>() || std::is_nothrow_destructible<T>())
		{
			if constexpr (std::is_pointer<T>())
				delete (*item).item;
			else
				(*item).item.~T();
		}
		(*item).empty = true;
	}
	void Remove(Rect area)
	{
		auto list = mQt.Search(area);
		for (auto l : list)
		{
			Remove(l);
		}
	}
	void Relocate(ITER item, Rect area)
	{
		(*item).ptr.leafList->erase((*item).ptr.itemIter);
		(*item).ptr = mQt.Insert(item, area);
	}
	std::array<QTI, MAX_SIZE>::iterator begin()
	{
		return mData.begin();
	}
	std::array<QTI, MAX_SIZE>::iterator end()
	{
		return mData.end();
	}
	std::array<QTI, MAX_SIZE>::const_iterator cbegin()
	{
		return mData.cbegin();
	}
	std::array<QTI, MAX_SIZE>::const_iterator cend()
	{
		return mData.cend();
	}
	
	template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::make_nvp("size",mQt.GetSize()));
		ar(cereal::make_nvp("maxDepth",mQt.GetMaxDepth()));
		int itemCounter = 0;
		for (auto it : mData)
			if (!it.empty)
			{
				itemCounter++;
			}
		ar(cereal::make_nvp("itemCounter",itemCounter));
		for (auto it : mData)
			if (!it.empty)
			{
				ar(it.item);
			}
		
	}
	template<class Archive>
	void load(Archive& ar)
	{
		Rect r;
		ar(r);
		int maxDepth = 0;
		ar(maxDepth);
		int itemCounter = 0;
		ar(itemCounter);

		mQt = Quadtree<ITER, Rect>(r, maxDepth);
		T item;
		for (int i = 0; i < itemCounter; i++)
		{
			ar(item);
			Insert(item, item->GetHitBox());
		}
	}
};
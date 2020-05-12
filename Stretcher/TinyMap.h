#pragma once

#include <vector>
#include <algorithm>
#include <memory>
using std::pair;
using std::vector;
using std::unique_ptr;
using std::make_pair;
using std::make_unique;
using std::swap;

template <class TKey, class TValue>
class TinyMapUnique
{
	vector<pair<TKey, unique_ptr<TValue>>> vec;
	pair<TKey, unique_ptr<TValue>> firstPair;
public:
	TinyMapUnique()
	{

	}
	bool AddNew(TKey key)
	{
		if (Get(key) != NULL) return false;
		if (firstPair.second == nullptr)
		{
			firstPair = make_pair(key, make_unique<TValue>());
		}
		else
		{
			vec.push_back(make_pair(key, make_unique<TValue>()));
		}
		return true;
	}
	TValue* Get(TKey key)
	{
		if (firstPair.first == key) return firstPair.second.get();
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i].first == key)
			{
				swap(vec[i], firstPair);
				if (vec[i].second == nullptr)
				{
					vec.erase(vec.begin() + i, vec.begin() + 1 + i);
				}
				return firstPair.second.get();
			}
		}
		return nullptr;
	}
	bool ContainsKey(TKey key)
	{
		if (firstPair.first == key) return true;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i].first == key) return true;
		}
		return false;
	}
	bool Remove(TKey key)
	{
		if (firstPair.first == key && firstPair.second != nullptr)
		{
			auto dummyPair = make_pair<TKey, unique_ptr<TValue>>(0, nullptr);
			swap(firstPair, dummyPair);
			return true;
		}
		else
		{
			for (size_t i = 0; i < vec.size(); i++)
			{
				if (vec[i].first == key)
				{
					vec.erase(vec.begin() + i, vec.begin() + i + 1);
					return true;
				}
			}
		}
		return false;
	}
	TValue* GetFirstValue()
	{
		return firstPair.second.get();
	}
	TKey GetFirstKey()
	{
		return firstPair.first;
	}

};

template <class TKey, class TValue>
class TinyMapUnique;

static void* __zero__ = 0;
template <class TKey, class TValue>
class TinyMap
{
	friend class TinyMapUnique<TKey, TValue>;
	vector<pair<TKey, TValue>> vec;
	pair<TKey, TValue> firstPair;
	//static const TValue _DummyValue();
	//static const TKey _DummyKey();
public:
	bool Add(const TKey &key, const TValue &value)
	{
		if (Get(key) != NULL) return false;
		if (firstPair.second == nullptr)
		{
			firstPair = make_pair(key, value);
		}
		else
		{
			vec.push_back(make_pair(key, value));
		}
		return true;
	}
	bool Emplace(const TKey& key, TValue&& value)
	{
		if (Get(key) != NULL) return false;
		if (firstPair.second == nullptr)
		{
			firstPair = make_pair(key, value);
		}
		else
		{
			vec.emplace_back(make_pair(key, value));
		}
		return true;
	}

	TValue &Get(const TKey& key)
	{
		if (firstPair.first == key) return firstPair.second;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i].first == key)
			{
				swap(vec[i], firstPair);
				return firstPair.second;
			}
		}
		return *reinterpret_cast<TValue*>(&__zero__);
	}
	TValue* GetNullable(const TKey& key)
	{
		if (firstPair.first == key) return &(firstPair.second);
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i].first == key)
			{
				swap(vec[i], firstPair);
				return &(firstPair.second);
			}
		}
		return nullptr;
	}

	const TValue* GetNullable(const TKey& key) const
	{
		if (firstPair.first == key) return &firstPair.second;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i].first == key)
			{
				return &vec[i].second;
			}
		}
		return nullptr;
	}

	const TValue& Get(const TKey& key) const
	{
		if (firstPair.first == key) return firstPair.second;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i].first == key)
			{
				return vec[i].second;
			}
		}
		return *reinterpret_cast<TValue*>(&__zero__);
	}
	TKey &GetKey(const TValue& value)
	{
		if (firstPair.second == value) return firstPair.first;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i].second == value)
			{
				swap(vec[i], firstPair);
				return firstPair.first;
			}
		}
		return *reinterpret_cast<TKey*>(&__zero__);
	}
	const TKey &GetKey(const TValue& value) const
	{
		if (firstPair.second == value) return firstPair.first;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i].second == value)
			{
				return vec[i].first;
			}
		}
		return *reinterpret_cast<TKey*>(&__zero__);
	}
	bool ContainsKey(const TKey &key) const
	{
		if (firstPair.first == key) return true;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i].first == key) return true;
		}
		return false;
	}
	bool ContainsValue(const TValue &value) const
	{
		if (firstPair.second == value) return true;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i].second == value) return true;
		}
		return false;
	}
	bool Remove(TKey key)
	{
		if (firstPair.first == key && firstPair.second != nullptr)
		{
			auto dummyPair = make_pair<TKey, TValue>(0, 0);
			swap(firstPair, dummyPair);
			return true;
		}
		else
		{
			for (size_t i = 0; i < vec.size(); i++)
			{
				if (vec[i].first == key)
				{
					vec.erase(vec.begin() + i, vec.begin() + i + 1);
					return true;
				}
			}
		}
		return false;
	}
	TValue& GetFirstValue()
	{
		return firstPair.second;
	}
	TKey& GetFirstKey()
	{
		return firstPair.first;
	}
	const TValue& GetFirstValue() const
	{
		return firstPair.second;
	}
	const TKey& GetFirstKey() const
	{
		return firstPair.first;
	}
};

/*
template <class TKey, class TValue>
class TinyMapUnique
{
	TinyMap<TKey, unique_ptr<TValue>> map;
public:
	bool AddNew(const TKey& key)
	{
		return map.Emplace(key, make_unique<TValue>(TValue()));
	}
	TValue* Get(const TKey& key)
	{
		auto result = map.GetNullable(key);
		if (result == nullptr) return nullptr;
		return result->get();
	}
	bool ContainsKey(const TKey& key) const
	{
		return map.ContainsKey(key);
	}
	bool Remove(TKey key)
	{
		return map.Remove(key);
	}
	TValue* GetFirstValue()
	{
		return Get(GetFirstKey());
	}
	TKey& GetFirstKey()
	{
		return map.GetFirstKey();
	}
	TKey& GetFirstKey() const
	{
		return map.GetFirstKey();
	}

};
*/
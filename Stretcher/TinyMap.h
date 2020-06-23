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

#include <unordered_map>
using std::unordered_map;

template <class TKey, class TValue>
class HashMap
{
	unordered_map<TKey, TValue> map;
private:
	auto FindValue(const TValue& value)
	{
		for (auto iterator = map.begin(); iterator != map.end(); iterator++)
		{
			if (iterator->second == value) return iterator;
		}
		return map.end();
	}
	auto FindValue(const TValue& value) const
	{
		for (auto iterator = map.cbegin(); iterator != map.cend(); iterator++)
		{
			if (iterator->second == value) return iterator;
		}
		return map.cend();
	}
public:
	using KeyType = TKey;
	using ValueType = TValue;
	bool ContainsKey(const TKey& key) const
	{
		return map.find(key) != map.end();
	}
	bool ContainsValue(const TValue& value) const
	{
		return FindValue(value) != map.cend();
	}
	TValue& Set(const TKey& key, TValue&& value) noexcept
	{
		auto iteratorAndBool = map.emplace(key, std::forward<TValue>(value));
		bool wasInserted = iteratorAndBool.second;
		auto iterator = iteratorAndBool.first;
		auto& pair = *iterator;
		if (!wasInserted)
		{
			pair.second = std::forward<TValue>(value);
		}
		return pair.second;
	}
	TValue& Set(const TKey& key, const TValue& value)
	{
		auto iteratorAndBool = map.emplace(key, value);
		bool wasInserted = iteratorAndBool.second;
		auto iterator = iteratorAndBool.first;
		auto& pair = *iterator;
		if (!wasInserted)
		{
			pair.second = value;
		}
		return pair.second;
	}
	bool Remove(const TKey& key)
	{
		return 0 != map.erase(key);
	}
	bool RemoveValue(const TValue& value)
	{
		bool anythingRemoved = false;
		auto iterator = map.begin();
		while (iterator != map.end())
		{
			if (iterator->second == value)
			{
				anythingRemoved = true;
				iterator = map.erase(iterator);
			}
			else
			{
				iterator++;
			}
		}
		return anythingRemoved;
	}
	TValue* GetReference(const TKey& key)
	{
		auto found = map.find(key);
		if (found == map.end()) return NULL;
		return &found->second;
	}
	const TValue* GetReference(const TKey& key) const
	{
		auto found = map.find(key);
		if (found == map.end()) return NULL;
		return &found->second;
	}
	TValue Get(const TKey& key)
	{
		auto found = map.find(key);
		if (found == map.end()) return TValue();
		return found->second;
	}
	TKey GetKey(const TValue& value)
	{
		auto found = FindValue(value);
		if (found == map.end()) return TKey();
		return found->first;
	}
};

template <class TKey, class TValue>
class VectorMap
{
	vector<pair<TKey, TValue>> vec;
private:
	int Find(const TKey& key) const
	{
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i].first == key) return (int)i;
		}
		return -1;
	}
	int FindValue(const TValue& value) const
	{
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i].second == value) return (int)i;
		}
		return -1;
	}
public:
	using KeyType = TKey;
	using ValueType = TValue;
	bool ContainsKey(const TKey& key) const
	{
		return Find(key) != -1;
	}
	bool ContainsValue(const TValue& value) const
	{
		return FindValue(value) != -1;
	}
	TValue& Set(const TKey& key, TValue&& value) noexcept
	{
		int index = Find(key);
		if (index != -1)
		{
			vec[index].second = std::forward<TValue>(value);
			return vec[index].second;
		}
		else
		{
			vec.emplace_back(make_pair(key, std::forward<TValue>(value)));
			return vec[vec.size() - 1].second;
		}
	}
	TValue& Set(const TKey& key, const TValue& value)
	{
		int index = Find(key);
		if (index != -1)
		{
			vec[index] = make_pair(key, value);
			return vec[index].second;
		}
		else
		{
			vec.emplace_back(make_pair(key, value));
			return vec[vec.size() - 1].second;
		}
	}

	bool Remove(const TKey& key)
	{
		int index = Find(key);
		if (index == -1) return false;
		vec.erase(vec.begin() + index, vec.begin() + index + 1);
		return true;
	}

	bool RemoveValue(const TValue& value)
	{
		bool anythingRemoved = false;
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i].second == value)
			{
				vec.erase(vec.begin() + i, vec.begin() + i + 1);
				i--;
			}
		}
		return anythingRemoved;
	}

	TValue Get(const TKey& key) const
	{
		int index = Find(key);
		if (index == -1) return TValue();
		return vec[index].second;
	}

	TValue* GetReference(const TKey& key)
	{
		int index = Find(key);
		if (index == -1) return NULL;
		return &vec[index].second;
	}

	const TValue* GetReference(const TKey& key) const
	{
		int index = Find(key);
		if (index == -1) return NULL;
		return &vec[index].second;
	}

	TKey GetKey(const TValue& value) const
	{
		int index = FindValue(value);
		if (index == -1) return TKey();
		return vec[index].first;
	}
};

template <class TKey, class TValue, class TMap>
class CachedMap
{
	TMap map;
	mutable TKey mostRecentKey = TKey();
	mutable TValue* pMostRecentValue = nullptr;
public:
	using KeyType = TKey;
	using ValueType = TValue;

	~CachedMap()
	{
		mostRecentKey = TKey();
		pMostRecentValue = nullptr;
	}

	bool ContainsKey(const TKey& key) const
	{
		if (pMostRecentValue != NULL && mostRecentKey == key) return true;
		pMostRecentValue = const_cast<TValue*>(map.GetReference(key));
		mostRecentKey = key;
		return pMostRecentValue != NULL;
	}

	bool ContainsValue(const TValue& value) const
	{
		if (pMostRecentValue != NULL && *pMostRecentValue == value) return true;
		TKey key = map.GetKey(value);
		if (ContainsKey(key))
		{
			return *pMostRecentValue == value;
		}
		return false;
	}

	TValue& Set(const TKey& key, TValue&& value) noexcept
	{
		TValue& result = map.Set(key, std::forward<TValue>(value));
		mostRecentKey = key;
		pMostRecentValue = &result;
		return result;
	}

	TValue& Set(const TKey& key, const TValue& value)
	{
		TValue& result = map.Set(key, value);
		mostRecentKey = key;
		pMostRecentValue = &result;
		return result;
	}

	bool Remove(const TKey& key)
	{
		if (mostRecentKey == key)
		{
			mostRecentKey = TKey();
			pMostRecentValue = NULL;
		}
		return map.Remove(key);
	}

	bool RemoveValue(const TValue& value)
	{
		if (pMostRecentValue != NULL && *pMostRecentValue == value)
		{
			pMostRecentValue = NULL;
		}
		return map.RemoveValue(value);
	}

	TValue Get(const TKey& key)
	{
		if (ContainsKey(key)) return *pMostRecentValue;
		return TValue();
	}

	TValue* GetReference(const TKey& key)
	{
		ContainsKey(key);
		return pMostRecentValue;
	}

	TValue* GetMostRecentValue()
	{
		if (pMostRecentValue != NULL) return pMostRecentValue;
		return NULL;
	}

	TKey GetKey(const TValue& value) const
	{
		if (ContainsValue(value))
		{
			return mostRecentKey;
		}
		return TKey();
	}
};

template <class TKey, class TValue>
class CachedVectorMap : public CachedMap<TKey, TValue, VectorMap<TKey, TValue>> { };
template <class TKey, class TValue>
class CachedHashMap : public CachedMap<TKey, TValue, HashMap<TKey, TValue>> { };

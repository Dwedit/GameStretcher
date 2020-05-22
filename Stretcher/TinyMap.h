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
public:
	bool ContainsKey(const TKey& key) const
	{
		return map.find(key) != map.end();
	}
	bool ContainsValue(const TValue& value) const
	{
		for (auto iterator = map.begin(); iterator != map.end(); iterator++)
		{
			if (iterator->second == value) return true;
		}
		return false;
	}
	TValue& Set(const TKey& key, TValue&& value)
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
	bool ContainsKey(const TKey& key) const
	{
		return Find(key) != -1;
	}
	bool ContainsValue(const TValue& value) const
	{
		return FindValue(value) != -1;
	}
	TValue& Set(const TKey& key, TValue&& value)
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
};

template <class TKey, class TValue, class TMap>
class CachedMap
{
	TMap map;
	mutable TKey mostRecentKey;
	mutable TValue* pMostRecentValue;
public:
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
		return map.ContainsValue(value);
	}

	TValue& Set(const TKey& key, TValue&& value)
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
};

template <class TKey, class TValue>
class CachedVectorMap : public CachedMap<TKey, TValue, VectorMap<TKey, TValue>> {};
template <class TKey, class TValue>
class CachedHashMap : public CachedMap<TKey, TValue, HashMap<TKey, TValue>> {};


//template <class TKey, class TValue>
//typedef CachedMap<TKey, TValue, VectorMap<TKey, TValue>> CachedVectorMap;

//template <class TKey, class TValue>
//typedef CachedMap<TKey, TValue, VectorMap<TKey, TValue>> CachedHashMap;


/*
template <class TKey, class TValue, class TMap>
class CachedMap
{
	TMap map;
	mutable TKey mostRecentKey;
	mutable TValue* pMostRecentValue;
public:
	bool ContainsKey(const TKey& key)
	{
		if (pMostRecentValue != NULL && mostRecentKey == key) return true;
		return map.ContainsKey(key);
	}

	bool ContainsValue(const TValue& value)
	{
		if (pMostRecentValue != NULL && *pMostRecentValue == value) return true;
		return map.ContainsValue(value);
	}

	bool Add(const TKey& key, TValue value)
	{
		if (ContainsKey(key)) return false;
		Set(key, value);
		return true;
	}

	bool Emplace(const TKey& key, TValue&& value)
	{
		if (ContainsKey(key)) return false;
		auto &newItem = map.Emplace(key, std::forward<TValue>(value));
		mostRecentKey = newItem.first;
		pMostRecentValue = &newItem.second;
		return true;
	}

	bool AddDefault(const TKey& key)
	{
		return Emplace(key, TValue());
	}

	void Set(const TKey& key, TValue value)
	{
		std::pair<unordered_map<TKey, TValue>::iterator, bool> newItem = map.emplace(key, std::forward<TValue>(value));
		mostRecentKey = newItem.first->first;
		pMostRecentValue = &newItem.first->second;
	}

	bool Remove(const TKey& key)
	{
		if (mostRecentKey == key)
		{
			map.erase(key);
			mostRecentKey = TKey();
			pMostRecentValue = NULL;
			return true;
		}
		return map.erase(key) > 0;
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
		if (anythingRemoved)
		{
			mostRecentKey = TKey();
			pMostRecentValue = NULL;
		}
		return anythingRemoved;
	}

	TValue Get(const TKey& key)
	{
		if (ContainsKey(key)) return *pMostRecentValue;
		return TValue();
	}

	TValue* GetReference(const TKey& key)
	{
		if (ContainsKey(key)) return pMostRecentValue;
		return NULL;
	}

	TValue* GetMostRecentValue()
	{
		if (pMostRecentValue != NULL) return pMostRecentValue;
		auto iterator = map.begin();
		if (iterator != map.end())
		{
			pMostRecentValue = &iterator->second;
			return pMostRecentValue;
		}
		return NULL;
	}
};

template <class TKey, class TValue>
class TinyMap
{
	unordered_map<TKey, TValue> map;
	mutable TKey mostRecentKey;
	mutable TValue *pMostRecentValue;
public:
	//TinyMap()
	//{
	//
	//}
	//~TinyMap()
	//{
	//
	//}

	bool ContainsKey(const TKey &key)
	{
		if (pMostRecentValue != NULL && mostRecentKey == key) return true;
		auto found = map.find(key);
		if (found != map.end())
		{
			mostRecentKey = key;
			pMostRecentValue = &found->second;
			return true;
		}
		return false;
	}

	bool ContainsValue(const TValue& value)
	{
		if (pMostRecentValue != NULL && *pMostRecentValue == value) return true;
		for (auto iterator = map.begin(); iterator != map.end(); iterator++)
		{
			if (iterator->second == value)
			{
				mostRecentKey = iterator->first;
				pMostRecentValue = &iterator->second;
				return true;
			}
		}
		return false;
	}

	bool Add(const TKey &key, TValue value)
	{
		if (ContainsKey(key)) return false;
		Set(key, value);
		return true;
	}

	bool Emplace(const TKey& key, TValue&& value)
	{
		if (ContainsKey(key)) return false;
		std::pair<unordered_map<TKey, TValue>::iterator, bool> newItem = map.emplace(key, std::forward<TValue>(value));
		mostRecentKey = newItem.first->first;
		pMostRecentValue = &newItem.first->second;
		return true;
	}

	bool AddDefault(const TKey& key)
	{
		return Emplace(key, TValue());
	}

	void Set(const TKey &key, TValue value)
	{
		std::pair<unordered_map<TKey, TValue>::iterator, bool> newItem = map.emplace(key, std::forward<TValue>(value));
		mostRecentKey = newItem.first->first;
		pMostRecentValue = &newItem.first->second;
	}

	bool Remove(const TKey& key)
	{
		if (mostRecentKey == key)
		{
			map.erase(key);
			mostRecentKey = TKey();
			pMostRecentValue = NULL;
			return true;
		}
		return map.erase(key) > 0;
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
		if (anythingRemoved)
		{
			mostRecentKey = TKey();
			pMostRecentValue = NULL;
		}
		return anythingRemoved;
	}

	TValue Get(const TKey& key)
	{
		if (ContainsKey(key)) return *pMostRecentValue;
		return TValue();
	}

	TValue* GetReference(const TKey& key)
	{
		if (ContainsKey(key)) return pMostRecentValue;
		return NULL;
	}

	TValue* GetMostRecentValue()
	{
		if (pMostRecentValue != NULL) return pMostRecentValue;
		auto iterator = map.begin();
		if (iterator != map.end())
		{
			pMostRecentValue = &iterator->second;
			return pMostRecentValue;
		}
		return NULL;
	}

	/*
	unordered_map<TKey, TValue>& UnsafeGetInnerMap()
	{
		return this->map;
	}
	*/
//};

/*

















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

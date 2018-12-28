#pragma once

#include "Platform.h"
#include "HashBasicTypes.h"
#include "Containers/Array.h"
#include "Containers/Pair.h"

template<typename Key, typename Value>
class Map
{
	using KeyType = Key;
	using ValueType = Value;
	using MapType = Pair<KeyType, ValueType>;
	using BucketType = Array<MapType>;
public:
	Map()
		: buckets(DefaultAmountOfBuckets)
	{
	}

	explicit Map(uint32 bucketAmount)
		: buckets(bucketAmount)
	{
	}

	Map(std::initializer_list<MapType> list)
		: buckets(DefaultAmountOfBuckets),
		maxBucketSize(0)
	{
		const MapType* beginning = list.begin();
		const MapType* ending = list.end();
		for (; beginning != ending; ++beginning)
		{
			Add(beginning->first, beginning->second);
		}
	}

	Map(const Map& other)
		: buckets(other.buckets),
		maxBucketSize(other.maxBucketSize)
	{
	}

	Map(Map&& other)
		: buckets(std::move(other.buckets)),
		maxBucketSize(other.maxBucketSize)
	{
		other.maxBucketSize = 0;
	}

	Map& operator=(const Map& other)
	{
		if (this != &other)
		{
			buckets = other.buckets;
			maxBucketSize = other.maxBucketSize;
		}

		return *this;
	}

	Map& operator=(Map&& other)
	{
		if (this != &other)
		{
			buckets = std::move(other.buckets);
			maxBucketSize = other.maxBucketSize;

			other.maxBucketSize = 0;
		}

		return *this;
	}

public:
	ValueType& Add(const KeyType& key, const ValueType& val)
	{
		uint32 hash = GetHash(key);
		BucketType& bucket = FindBucket(hash);
		MapType* store = FindPair(bucket, key);
		if (store)
		{
			store->second = val;
		}
		else
		{
			Pair<KeyType, ValueType> pair = { key, val };
			uint32 index = bucket.Add(pair);
			store = &bucket[index];
		}

		return store->second;
	}

	bool TryFind(const Key& key, Value& value)
	{
		uint32 hash = GetHash(key);
		BucketType& bucket = FindBucket(hash);
		MapType* store = FindPair(bucket, key);
		if (store)
		{
			value = store->second;
			return true;
		}

		return false;
	}

	bool Remove(const KeyType& key)
	{
		uint32 hash = GetHash(key);
		BucketType& bucket = FindBucket(hash);
		return RemoveBucketElement(bucket, key);
	}

private:
	bool RemoveBucketElement(BucketType& bucket, const KeyType& key)
	{
		for (uint32 i = 0; i < buckets.Size(); ++i)
		{
			if (bucket[i].first == key)
			{
				bucket.Remove(i);
				return true;
			}
		}
		return false;
	}

	BucketType& FindBucket(uint32 hash)
	{
		uint32 bucketIndex = hash % buckets.Size();
		return buckets[bucketIndex];
	}

	MapType* FindPair(BucketType& bucket, const KeyType& key)
	{
		MapType* storage = nullptr;
		for (uint32 i = 0; i < bucket.Size(); ++i)
		{
			MapType& store = bucket[i];
			if (store.first == key)
			{
				storage = &store;
				break;
			}
		}
		return storage;
	}

	template <typename EmplaceType>
	ValueType& FindOrDefault(EmplaceType&& key)
	{
		uint32 hash = GetHash(key);
		BucketType& bucket = FindBucket(hash);
		MapType* storage = FindPair(bucket, key);

		if (!storage)
		{
			Pair<KeyType, ValueType> pair = { std::forward<EmplaceType>(key), ValueType() };
			uint32 index = bucket.Add(pair);
			storage = &bucket[index];
		}

		return storage->second;
	}

public:
	// TODO - Give a good compile error if a key is passed in with no GetHash() function
	ValueType& operator[](const KeyType& key)
	{
		return FindOrDefault(key);
	}

	ValueType& operator[](KeyType&& key)
	{
		return FindOrDefault(std::move(key));
	}

public:
	class Iterator final
	{
		friend class Map<Key, Value>;

	public:
		Iterator(Map<Key, Value>& map)
			: buckets(&map.buckets)
		{
			uint32 i = bucketIndex;
			for (; i < buckets->Size(); ++i)
			{
				if ((*buckets)[i].Size() > 0)
				{
					break;
				}
			}
			bucketIndex = i;
		}

		Iterator(Map<Key, Value>& map, uint32 bucketInd)
			: buckets(&map.buckets),
			bucketIndex(bucketInd)
		{
		}

		MapType& operator*()
		{
			return (*buckets)[bucketIndex][bucketElementIndex];
		}

		explicit operator bool() const
		{
			return bucketIndex < buckets->Size();
		}

		Iterator& operator++()
		{
			if (++bucketElementIndex >= (*buckets)[bucketIndex].Size())
			{
				bucketElementIndex = 0;
				uint32 i = bucketIndex + 1;
				for (; i < buckets->Size(); ++i)
				{
					if ((*buckets)[i].Size() > 0)
					{
						break;
					}
				}

				bucketIndex = i;
			}

			return *this;
		}

		friend bool operator!=(const Iterator& lhs, const Iterator& rhs)
		{
			return lhs.buckets != rhs.buckets || 
				lhs.bucketIndex != rhs.bucketIndex || 
				lhs.bucketElementIndex != rhs.bucketElementIndex;
		}

	private:
		Array<BucketType>* buckets;
		uint32 bucketIndex = 0;
		uint32 bucketElementIndex = 0;

	};

	class ConstIterator final
	{
		friend class Map<Key, Value>;

	public:
		ConstIterator(Map<Key, Value>& map)
			: buckets(&map.buckets)
		{
			uint32 i = bucketIndex;
			for (; i < buckets->Size(); ++i)
			{
				if ((*buckets)[i].Size() > 0)
				{
					break;
				}
			}
			bucketIndex = i;
		}

		ConstIterator(const Map<Key, Value>& map, uint32 bucketInd)
			: buckets(&map.buckets),
			bucketIndex(bucketInd)
		{
		}

		MapType& operator*()
		{
			return (*buckets)[bucketIndex][bucketElementIndex];
		}

		explicit operator bool() const
		{
			return bucketIndex < buckets->Size();
		}

		ConstIterator& operator++()
		{
			if (++bucketElementIndex >= (*buckets)[bucketIndex].Size())
			{
				bucketElementIndex = 0;
				uint32 i = bucketIndex + 1;
				for (; i < buckets->Size(); ++i)
				{
					if ((*buckets)[i].Size() > 0)
					{
						break;
					}
				}

				bucketIndex = i;
			}

			return *this;
		}

		friend bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs)
		{
			return lhs.buckets != rhs.buckets ||
				lhs.bucketIndex != rhs.bucketIndex ||
				lhs.bucketElementIndex != rhs.bucketElementIndex;
		}

	private:
		const Array<BucketType>* buckets;
		uint32 bucketIndex = 0;
		uint32 bucketElementIndex = 0;

	};

private:
	friend Iterator begin(Map& map) { return Iterator(map); }
	friend Iterator begin(const Map& map) { return ConstIterator(map); }
	friend Iterator end(Map& map) { return Iterator(map, map.buckets.Size()); }
	friend Iterator end(const Map& map) { return ConstIterator(map, map.buckets.Size()); }

private:
	static constexpr uint32 DefaultAmountOfBuckets = 32;
	Array<BucketType> buckets;
	uint32 maxBucketSize = 0;
};

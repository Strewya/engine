#ifndef UTIL_DATASTORE_H_
#define UTIL_DATASTORE_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include <deque>
#include <memory>
#include <set>
#include <typeinfo>
#include <unordered_map>
	/*** end header inclusion ***/

namespace Util
{
	/*
		interface IDataStore
		Description:
			Base interface to expose common functionality for the DataStore class.
			As the DataStore class is templated, this interface is to provide type agnostic operations.
	*/
	class IDataStore
	{
	public:
		virtual ~IDataStore() {}
		virtual uint AcquireDataIndex(int index = NOT_FOUND) = 0;
		virtual void ReleaseDataIndex(uint index) = 0;
		virtual size_t getHash() = 0;
	};



	/*
		template DataStore class
		Description:
			The class is a storage class to keep all of it's data in a linear sequence.
			The data type must be a publicly constructible class (ctor, cctor, assignment).
			All data is reference counted, and handle accessible.
			If the handles are lost, the data will remain in memory forever and effectively leak (until program end).
			It's primary purpose is to hold data that the State class points to.
		Examples:
			DataStore<int> store; //constructor with space reserved for 64 int values
			DataStore<int> store(256); //constructor with space reserved for 256 int values
			store.AcquireDataIndex(); //acquires a new handle, setting it's reference count to 1
			store.AcquireDataIndex(1); //acquires a handle to a value that is already used by another handle, incrementing it's reference count by 1
			store.ReleaseDataIndex(1); //releases the handle to a value, decrementing it's reference count
			store.getData(1); //returns the data at the specified handle index. may throw out of bounds exception if it's called with an unacquired index.
	*/
	template<typename T> class DataStore : public IDataStore
	{
	private:
		typedef std::deque<T> TypeCache;
		typedef std::deque<uint> ReferenceCount;
		typedef std::set<uint> FreeSlots;
		
		TypeCache _data;
		ReferenceCount _refs;
		FreeSlots _freeSlots;
		size_t _hash;

	public:
		DataStore()
		{
			_hash = typeid(T).hash_code();
		}

		size_t getHash()
		{
			return _hash;
		}

		uint AcquireDataIndex(int index = NOT_FOUND)
		{
			if(index == NOT_FOUND)
			{
				//new data index
				if(!_freeSlots.empty())
				{
					index = *_freeSlots.begin();
					_freeSlots.erase(_freeSlots.begin());
					_data[index] = T();
				}
				else
				{
					index = _data.size();
					_data.emplace_back();
					_refs.push_back(0);
				}
			}
			
			++_refs[index];
			return index;
		}

		void ReleaseDataIndex(uint index)
		{
			if(index != NOT_FOUND && _refs[index] != 0)
			{
				if(--_refs[index] == 0)
				{
					_freeSlots.insert(index);
				}
			}
		}

		T& getData(uint index)
		{
			return _data[index];
		}
	};



	/*
		class DataStoreRepository
		Description:
			This class is used to provide a source of DataStore classes.
			It contains a map of unique_ptrs to heap allocated DataStores of a certain type.
			The DataStores are created and acquired via a template method.
			The key for the DataStore pointers is the hash code of that specific type, determined by calling typeid.
	*/
	class DataStoreRepository
	{
	private:
		std::unordered_map<size_t, std::unique_ptr<IDataStore>> _storeMap;

	public:
		template<typename T> DataStore<T>& getDataStore()
		{
			auto hash = typeid(T).hash_code();
			auto it = _storeMap.find(hash);
			if(it == _storeMap.end())
			{
				it = _storeMap.insert(std::make_pair(hash, std::unique_ptr<IDataStore>(new DataStore<T>()))).first;
			}
			return *static_cast<DataStore<T>*>(it->second.get());
		}
	};
}

#endif //UTIL_DATASTORE_H_
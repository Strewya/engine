#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include <deque>
	/*** end header inclusion ***/

namespace Util
{
	template<typename T> struct AssetHolder
	{
		T* dataPtr;
		String filename;
		InstanceID id;
	};

	template<typename T, typename DEF = void> class AssetStore
	{
	public:
		AssetStore()
			: _idCounter(0)
		{}

		virtual ~AssetStore()
		{
			Clear();
		}

		void Clear()
		{
			typename Storage_t::iterator it = _assets.begin();
			for(; it != _assets.end(); ++it)
			{
				AssetHolder<T>& asset = *it;
				AssetUnloader(asset);
			}
		}

		T* CheckLoaded(const char* filename)
		{
			typename Storage_t::iterator it = _assets.begin();
			for(; it != _assets.end(); ++it)
			{
				AssetHolder<T>& asset = *it;
				if(asset.filename.compare(filename)==0)
				{
					return asset.dataPtr;
				}
			}
			return nullptr;
		}

		T* CheckLoaded(InstanceID id)
		{
			uint indexMask = 0xFFF;
			uint index = id & indexMask;
			if(index < _assets.size() && _assets[index].id == id)
			{
				return _assets[index].dataPtr;
			}
			return nullptr;
		}

		T* Acquire(const char* filename, DEF* loadArgs = nullptr)
		{
			InstanceID id;
			return Acquire(filename, id, loadArgs);
		}

		T* Acquire(const char* filename, InstanceID& outID, DEF* loadArgs = nullptr)
		{
			T* check = CheckLoaded(filename);
			if(check != nullptr)
			{
				return check;
			}
			
			uint index = FindFirstFreeIndex();
			AssetHolder<T>& asset = _assets[index];
			asset.dataPtr = Load(filename, loadArgs);
			if(asset.dataPtr == nullptr)
			{
				Util::GetDefaultLogger() << "Failed to load asset " << filename << Util::Logger::endl;
				return nullptr;
			}
			asset.id = _GenerateID(index);
			asset.filename.assign(filename);
			outID = asset.id;
			Util::GetDefaultLogger() << "Loaded asset " << filename << Util::Logger::endl;
			return asset.dataPtr;
		}
		
		bool Delete(InstanceID id)
		{
			uint indexMask = 0xFFF;
			uint index = id & indexMask;
			if(index < _assets.size() && _assets[index].id == id)
			{
				return AssetUnloader(_assets[index]);
			}
			return false;
		}

		bool Delete(T* ptr)
		{
			typename Storage_t::iterator it = _assets.begin();
			for(; it != _assets.end(); ++it)
			{
				AssetHolder<T>& asset = *it;
				if(asset.dataPtr == ptr)
				{
					return AssetUnloader(asset);
				}
			}
			return false;
		}

		bool Delete(const char* filename)
		{
			typename Storage_t::iterator it = _assets.begin();
			for(; it != _assets.end(); ++it)
			{
				AssetHolder<T>& asset = *it;
				if(asset.filename.compare(filename)==0)
				{
					return AssetUnloader(asset);
				}
			}
			return false;
		}

	protected:
		typedef T AssetType;
		typedef DEF LoadArgs;

		virtual AssetType* Load(const char* filename, const LoadArgs* loadArgs) = 0;
		virtual bool Unload(AssetType*& assetPtr) = 0;
		
		bool DeleteUnload(T*& asset)
		{
			delete asset;
			asset = nullptr;
			return true;
		}

	private:
		typedef std::deque<AssetHolder<T>> Storage_t;
		Storage_t _assets;
		InstanceID _idCounter;

		InstanceID _GenerateID(uint index)
		{
			uint indexMask = 0xFFF;
			InstanceID id = _idCounter++;
			id <<= 12;
			id = id | (index&indexMask);
			return id;
		}

		bool AssetUnloader(AssetHolder<T>& asset)
		{
			bool result = Unload(asset.dataPtr);
			if(result)
			{
				asset.dataPtr = nullptr;
				asset.filename.clear();
				asset.id = NOT_FOUND;
			}
			return result;
		}

		uint FindFirstFreeIndex()
		{
			typename Storage_t::iterator it = _assets.begin();
			for(; it != _assets.end(); ++it)
			{
				if(it->dataPtr == nullptr)
				{
					return it - _assets.end();
				}
			}
			_assets.emplace_back();
			return _assets.size()-1;
		}
	};
}
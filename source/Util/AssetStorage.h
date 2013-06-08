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
		typedef T AssetType;
		typedef DEF LoadArgs;

		AssetStore()
			: _idCounter(0), indexMask(0xFFF), maskBits(0)
		{
			uint tempMask = indexMask;
			for(; tempMask; ++maskBits)
			{
				tempMask &= tempMask-1;
			}
		}

		virtual ~AssetStore()
		{
			Clear();
		}

		void Clear()
		{
			typename Storage_t::iterator it = _assets.begin();
			for(; it != _assets.end(); ++it)
			{
				AssetUnloader(*it);
			}
		}

		AssetType* CheckLoaded(const char* filename)
		{
			typename Storage_t::iterator it = _assets.begin();
			for(; it != _assets.end(); ++it)
			{
				if(it->filename.compare(filename)==0)
				{
					return it->dataPtr;
				}
			}
			return nullptr;
		}

		AssetType* CheckLoaded(InstanceID id)
		{
			uint index = id & indexMask;
			if(index < _assets.size() && _assets[index].id == id)
			{
				return _assets[index].dataPtr;
			}
			return nullptr;
		}

		AssetType* Acquire(const char* filename, LoadArgs* loadArgs = nullptr)
		{
			//InstanceID id;
			return Acquire(filename, InstanceID(), loadArgs);
		}

		AssetType* Acquire(const char* filename, InstanceID& outID, LoadArgs* loadArgs = nullptr)
		{
			AssetType* check = CheckLoaded(filename);
			if(check != nullptr)
			{
				return check;
			}
			
			uint index = FindFirstFreeIndex();
			AssetHolder<AssetType>& asset = _assets[index];
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
			uint index = id & indexMask;
			if(index < _assets.size() && _assets[index].id == id)
			{
				return AssetUnloader(_assets[index]);
			}
			return false;
		}

		bool Delete(AssetType* ptr)
		{
			typename Storage_t::iterator it = _assets.begin();
			for(; it != _assets.end(); ++it)
			{
				if(it->dataPtr == ptr)
				{
					return AssetUnloader(*it);
				}
			}
			return false;
		}

		bool Delete(const char* filename)
		{
			typename Storage_t::iterator it = _assets.begin();
			for(; it != _assets.end(); ++it)
			{
				if(it->filename.compare(filename)==0)
				{
					return AssetUnloader(*it);
				}
			}
			return false;
		}

	protected:
		virtual AssetType* Load(const char* filename, const LoadArgs* loadArgs) = 0;
		virtual bool Unload(AssetType*& assetPtr) = 0;
		
		bool DeleteUnload(AssetType*& asset)
		{
			delete asset;
			asset = nullptr;
			return true;
		}

	private:
		typedef std::deque<AssetHolder<AssetType>> Storage_t;
		Storage_t _assets;
		InstanceID _idCounter;
		uint indexMask;
		uint maskBits;

		InstanceID _GenerateID(uint index)
		{
			InstanceID id = _idCounter++;
			id <<= maskBits;
			id = id | (index&indexMask);
			return id;
		}

		bool AssetUnloader(AssetHolder<AssetType>& asset)
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
					return it - _assets.begin();
				}
			}
			_assets.emplace_back();
			return _assets.size()-1;
		}
	};
}
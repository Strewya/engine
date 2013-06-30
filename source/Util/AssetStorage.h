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
		InstanceID id;
		String filename;
		T* dataPtr;
	};

	template<typename T, typename DEF = void> class AssetStore
	{	
	public:
		typedef T AssetType;
		typedef DEF LoadArgs;

		AssetStore();
		virtual ~AssetStore();

		AssetType* CheckLoaded(const char* filename);
		AssetType* CheckLoaded(InstanceID id);
		
		AssetType* Acquire(const char* filename, LoadArgs* loadArgs = nullptr);
		AssetType* Acquire(const char* filename, InstanceID& outID, LoadArgs* loadArgs = nullptr);
		
		void Clear();
		bool Delete(InstanceID id);
		bool Delete(AssetType* ptr);
		bool Delete(const char* filename);

	protected:
		virtual AssetType* Load(const char* filename, const LoadArgs* loadArgs) = 0;
		virtual bool Unload(AssetType*& assetPtr) = 0;
		
		bool DeleteUnload(AssetType*& asset);

	private:
		typedef std::deque<AssetHolder<AssetType>> Storage_t;
		Storage_t _assets;
		InstanceID _idCounter;
		uint32_t indexMask;
		uint32_t maskBits;

		InstanceID _GenerateID(uint32_t index);
		bool AssetUnloader(AssetHolder<AssetType>& asset);
		uint32_t FindFirstFreeIndex();
	};



	//****************************************************************************************
	//			ASSET STORE IMPLEMENTATION
	//****************************************************************************************


	template<typename T, typename DEF> AssetStore<T, DEF>::AssetStore()
		: _idCounter(0), indexMask(0xFFF), maskBits(0)
	{
		uint32_t tempMask = indexMask;
		for(; tempMask; ++maskBits)
		{
			tempMask &= tempMask-1;
		}
	}

	template<typename T, typename DEF> AssetStore<T, DEF>::~AssetStore()
	{
		Clear();
	}

	template<typename T, typename DEF> auto AssetStore<T, DEF>::CheckLoaded(const char* filename) -> AssetType*
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

	template<typename T, typename DEF> auto AssetStore<T, DEF>::CheckLoaded(InstanceID id) -> AssetType*
	{
		uint32_t index = id & indexMask;
		if(index < _assets.size() && _assets[index].id == id)
		{
			return _assets[index].dataPtr;
		}
		return nullptr;
	}

	template<typename T, typename DEF> auto AssetStore<T, DEF>::Acquire(const char* filename, LoadArgs* loadArgs = nullptr) -> AssetType*
	{
		//InstanceID id;
		return Acquire(filename, InstanceID(), loadArgs);
	}

	template<typename T, typename DEF> auto AssetStore<T, DEF>::Acquire(const char* filename, InstanceID& outID, LoadArgs* loadArgs = nullptr) -> AssetType*
	{
		AssetType* check = CheckLoaded(filename);
		if(check != nullptr)
		{
			return check;
		}
			
		uint32_t index = FindFirstFreeIndex();
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

	template<typename T, typename DEF> void AssetStore<T, DEF>::Clear()
	{
		typename Storage_t::iterator it = _assets.begin();
		for(; it != _assets.end(); ++it)
		{
			AssetUnloader(*it);
		}
	}

	template<typename T, typename DEF> bool AssetStore<T, DEF>::Delete(InstanceID id)
	{
		uint32_t index = id & indexMask;
		if(index < _assets.size() && _assets[index].id == id)
		{
			return AssetUnloader(_assets[index]);
		}
		return false;
	}

	template<typename T, typename DEF> bool AssetStore<T, DEF>::Delete(AssetType* ptr)
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

	template<typename T, typename DEF> bool AssetStore<T, DEF>::Delete(const char* filename)
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

	template<typename T, typename DEF> bool AssetStore<T, DEF>::DeleteUnload(AssetType*& asset)
	{
		delete asset;
		asset = nullptr;
		return true;
	}

	template<typename T, typename DEF> InstanceID AssetStore<T, DEF>::_GenerateID(uint32_t index)
	{
		InstanceID id = _idCounter++;
		id <<= maskBits;
		id = id | (index&indexMask);
		return id;
	}

	template<typename T, typename DEF> bool AssetStore<T, DEF>::AssetUnloader(AssetHolder<AssetType>& asset)
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

	template<typename T, typename DEF> uint32_t AssetStore<T, DEF>::FindFirstFreeIndex()
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
}
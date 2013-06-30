#pragma once
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Engine/Defines.h"
#include <deque>
#include <memory>
	/*** extra headers if needed (alphabetically ordered) ***/
	/*** end header inclusion ***/

namespace Util
{
	template<typename T> struct AssetHolder
	{
		typedef std::unique_ptr<T> Data;

		InstanceID id;
		Data dataPtr;
		String filename;
	};

	template<typename T, typename DEF = void> class AssetStore
	{	
	public:
		typedef T AssetType;
		typedef DEF LoadArgs;
		typedef AssetHolder<T> Asset;
		typedef typename Asset::Data AssetPtr;

		AssetStore();
		virtual ~AssetStore();

		Asset* CheckLoaded(const char* filename);
		Asset* CheckLoaded(InstanceID id);
		
		Asset* Acquire(const char* filename, LoadArgs* loadArgs = nullptr);
		
		void Clear();
		bool Delete(InstanceID id);
		//bool Delete(AssetType* ptr);
		bool Delete(const char* filename);

	protected:
		virtual bool Load(const char* filename, const LoadArgs* loadArgs, AssetPtr& asset) = 0;
		
		bool DeleteUnload(AssetPtr& asset);

	private:
		typedef std::deque<Asset> Storage_t;
		Storage_t _assets;
		InstanceID _idCounter;
		uint32_t indexMask;
		uint32_t maskBits;

		InstanceID _GenerateID(uint32_t index);
		bool AssetUnloader(Asset& asset);
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

	template<typename T, typename DEF> auto AssetStore<T, DEF>::CheckLoaded(const char* filename) -> Asset*
	{
		typename Storage_t::iterator it = _assets.begin();
		for(; it != _assets.end(); ++it)
		{
			Asset& ast = *it;
			if(ast.filename.compare(filename)==0)
			{
				return &ast;
			}
		}
		return nullptr;
	}
	//
	template<typename T, typename DEF> auto AssetStore<T, DEF>::CheckLoaded(InstanceID id) -> Asset*
	{
		uint32_t index = id & indexMask;
		if(index < _assets.size() && _assets[index].id == id)
		{
			return &_assets[index];
		}
		return nullptr;
	}

	template<typename T, typename DEF> auto AssetStore<T, DEF>::Acquire(const char* filename, LoadArgs* loadArgs) -> Asset*
	{
		Asset* check = CheckLoaded(filename);
		if(check != nullptr)
		{
			return check;
		}
			
		uint32_t index = FindFirstFreeIndex();
		Asset& asset = _assets[index];
		if(!Load(filename, loadArgs, asset.dataPtr))
		{
			Util::GetDefaultLogger() << "Failed to load asset " << filename << Util::Logger::endl;
			return nullptr;
		}
		asset.id = _GenerateID(index);
		asset.filename.assign(filename);
		Util::GetDefaultLogger() << "Loaded asset " << filename << Util::Logger::endl;
		return &asset;
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
	/*
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
	*/
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

	template<typename T, typename DEF> bool AssetStore<T, DEF>::DeleteUnload(AssetPtr& asset)
	{
		if(asset == nullptr)
		{
			return false;
		}
		asset.reset(nullptr);
		return true;
	}

	template<typename T, typename DEF> InstanceID AssetStore<T, DEF>::_GenerateID(uint32_t index)
	{
		InstanceID id = _idCounter++;
		id <<= maskBits;
		id |= (index&indexMask);
		return id;
	}

	template<typename T, typename DEF> bool AssetStore<T, DEF>::AssetUnloader(Asset& asset)
	{
		bool result = DeleteUnload(asset.dataPtr);
		if(result)
		{
			asset.dataPtr.reset(nullptr);
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
#pragma once
/********************************************
*	class:	ResourceCache
*	usage:
********************************************/
/******* C++ headers *******/
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
#include <Util/DataFile.h>
#include <Util/ObjectContainer.h>
/******* end header inclusion *******/

#define INVALID_ID -1

namespace Core
{
	template<typename RESOURCE, typename LOADER>
	class ResourceCache
	{
	public:
		template<typename ...ARG>
		bool init(ARG& ...args);
		bool shutdown();

		uint32_t getResourceID(const char* name) const;
		const RESOURCE& getResource(uint32_t id) const;

		void destroyResource(uint32_t id);

	protected:
		LOADER m_loader;
		ObjectContainer<RESOURCE> m_resources;

		friend class FileLoader;
		template<typename ...ARGS>
		bool loadFile(ARGS& ...args);
		template<typename ...ARGS>
		bool reloadFile(ARGS& ...args);
		template<typename ...ARGS>
		bool unloadFile(ARGS& ...args);
	};




	template<typename R>
	uint32_t findResourceByName(const std::vector<R>& data, const char* name)
	{
		assert(name != nullptr);
		using std::begin; using std::end;
		auto it = std::find_if(begin(data), end(data), [&](const R& resource)
		{
			return name == resource.m_name;
		});

		if(it != end(data))
		{
			return std::distance(begin(data), it);
		}
		return INVALID_ID;
	}

	template<typename R, typename L>
	template<typename ...ARG>
	bool ResourceCache<R, L>::init(ARG& ...args)
	{
		m_loader = L(args...);
		return true;
	}

	template<typename R, typename L>
	bool ResourceCache<R, L>::shutdown()
	{
		m_loader.unloadAll(m_resources);
		m_loader.clear();
		return true;
	}

	template<typename R, typename L>
	template<typename ...ARGS>
	bool ResourceCache<R, L>::loadFile(ARGS& ...args)
	{
		return m_loader.load(m_resources, args...);
	}

	template<typename R, typename L>
	template<typename ...ARGS>
	bool ResourceCache<R, L>::reloadFile(ARGS& ...args)
	{
		return m_loader.reload(m_resources, args...);
	}

	template<typename R, typename L>
	template<typename ...ARGS>
	bool ResourceCache<R, L>::unloadFile(ARGS& ...args)
	{
		return m_loader.unload(m_resources, args...);
	}

	template<typename R, typename L>
	uint32_t ResourceCache<R, L>::getResourceID(const char* name) const
	{
		using std::placeholders;
		return m_resources.getID(std::bind(findResourceByName, _1, name));
	}

	template<typename R, typename L>
	const R& ResourceCache<R, L>::getResource(uint32_t id) const
	{
		return m_resources.get(id);
	}

	template<typename R, typename L>
	void ResourceCache<R, L>::destroyResource(uint32_t id)
	{
		if(m_loader.unload(m_resources, id))
		{
			m_resources.remove(id);
		}
	}
}

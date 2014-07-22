#pragma once
/********************************************
*	class:	ObjectContainer
*	usage:
********************************************/
/******* C++ headers *******/
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

namespace Core
{
	template<typename T> class ObjectContainer
	{
	public:
		ObjectContainer(uint32_t initialSize = 0);

		uint32_t create();
		T& get(uint32_t id);
		void remove(uint32_t id);

	private:
		std::vector<T> m_data;
		std::vector<uint32_t> m_freeIds;
	};

	template<typename T> ObjectContainer<T>::ObjectContainer(uint32_t initialSize)
	{
		m_data.reserve(initialSize);
	}

	template<typename T> uint32_t ObjectContainer<T>::create()
	{
		uint32_t id = m_data.size();
		if(!m_freeIds.empty())
		{
			id = m_freeIds.back();
			m_freeIds.pop_back();
		}
		else
		{
			m_data.emplace_back();
		}
		return id;
	}

	template<typename T> T& ObjectContainer<T>::get(uint32_t id)
	{
		return m_data[id];
	}

	template<typename T> void ObjectContainer<T>::remove(uint32_t id)
	{
		m_freeIds.emplace_back(id);
	}
}

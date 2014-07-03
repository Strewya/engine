#pragma once
/********************************************
*	class:	ObjectContainer
*	usage:
********************************************/
/******* C++ headers *******/
#include <algorithm>
#include <cstdint>
#include <vector>
/******* common headers *******/
/******* extra headers *******/
/******* end header inclusion *******/

#define INVALID_ID -1

namespace Core
{
	template<typename T> class ObjectContainer
	{
	public:
		ObjectContainer(uint32_t initialSize = 0);

		uint32_t create();
		template<typename F> uint32_t getID(const F& fn) const;
		T& get(uint32_t id);
		void remove(uint32_t id);
		void clear();

	private:
		std::vector<T> m_data;
		std::vector<uint32_t> m_allocated;
		uint32_t m_firstFree;

		void setSize(uint32_t size);
	};

	template<typename T> ObjectContainer<T>::ObjectContainer(uint32_t initialSize)
		: m_firstFree(0)
	{
		setSize(initialSize);
	}

	template<typename T> uint32_t ObjectContainer<T>::create()
	{
		if(m_firstFree == m_allocated.size())
		{
			setSize(m_data.size() * 2);
		}
		uint32_t id = m_allocated[m_firstFree++];
		return id;
	}

	template<typename T>
	template<typename F> uint32_t ObjectContainer<T>::getID(const F& fn) const
	{
		for(size_t i = 0; i < m_firstFree; ++i)
		{
			if(fn(m_data[m_allocated[i]]))
			{
				return m_allocated[i];
			}
		}
		return INVALID_ID;
	}

	template<typename T> T& ObjectContainer<T>::get(uint32_t id)
	{
		return m_data[id];
	}

	template<typename T> void ObjectContainer<T>::remove(uint32_t id)
	{
		auto it = std::find(m_allocated.begin(), m_allocated.end(), id);
		auto index = std::distance(m_allocated.begin(), it);
		if(index < m_firstFree)
		{
			--m_firstFree;
			std::swap(m_allocated[index], m_allocated[m_firstFree]);
		}
	}

	template<typename T> void ObjectContainer<T>::clear()
	{
		m_firstFree = 0;
	}

	template<typename T> void ObjectContainer<T>::setSize(uint32_t size)
	{
		if(size > m_data.size())
		{
			m_data.resize(size);
			uint32_t i = m_allocated.size();
			std::generate_n(std::back_inserter(m_allocated), size, [&]() { return i++; });
		}
	}
}

#ifndef CORE_STATE_STATE_H_
#define CORE_STATE_STATE_H_
/********************************************
	class:	
	usage:	
********************************************/
	/*** common and C++ headers ***/
#include "Defines.h"
	/*** extra headers if needed (alphabetically ordered) ***/
#include "Util/DataStore.h"
	/*** end header inclusion ***/

namespace Core
{
	/*
		State class, version 2
		Description:
			Each instance of the State class is a wrapper around any data type.
			The wrapper contains only a index based handle to the actual data.
			The actual data is stored in a static container accessible by calling getDataStoreReference().
			Because of this, the wrappers are lightweight objects, which can be cheaply copied around.
			The data the wrappers point to is reference counted via ctors and dtor.
			The reference counting mechanism is part of the data container.
			The data type MUST be default constructible.
			The data itself is reference returned by calling a template method as<T>().
		Example usage:
			State n; //default constructor - n points to no data
			State n2(10); //template<T> constructor - n2 points to int(10)
			State n3 = n2; //copy constructor - n2 and n3 point to the same int(10)
			State n4 = State(16); //move constructor - n4 points to int(16)
			n = n3; //assignment operator - n, n2 and n3 point to the same int(10)
			n2 = State('a'); //move assignment operator - n2 points to char('a')
			n4 = 20; //template<T> assignment operator - n4 points to a new int(20)
			n.as<int>() = 42; //modifies the value n and n3 point to from 10 to 42
	*/
	class State
	{
	private:
		static Util::DataStoreRepository* _storeRepo;
		Util::IDataStore* _dataStore;
		int _dataHandle;

		void AddRef(int handle = NOT_FOUND);
		void Release();

	public:
		static void BindStoreRepository(Util::DataStoreRepository& repo);
		State();
		State(const State& rhs);
		State(State&& rhs);
		State& operator=(const State& rhs);
		State& operator=(State&& rhs);
		~State();
		bool isValid();

		template<typename T> State(const T& value)
			: _dataStore(&_storeRepo->getDataStore<T>()), _dataHandle(_storeRepo->getDataStore<T>().AcquireDataIndex())
		{
			as<T>() = value;
		}

		template<typename T> State& operator=(const T& rhs)
		{
			Util::DataStore<T>& store = _storeRepo->getDataStore<T>();
			if(_dataStore != &store)
			{
				Release();
				_dataStore = &store;
				AddRef();
			}
			as<T>() = rhs;
			return *this;
		}

		template<typename T> T& as()
		{
			Util::DataStore<T>& store = _storeRepo->getDataStore<T>();
			if(&store == _dataStore)
			{
				return store.getData(_dataHandle);
			}
			throw std::exception("State::as<T>(): Tried to access wrong type!");
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		simple base class for the State class
	*/
	/*
	class BaseState
	{
	private:

	public:
		virtual ~BaseState() {}
	};
	*/
	
	/*
		template State class
		Description:
			Each instance of a State class is a wrapper for the data type it is templated upon.
			The wrapper contains only a handle to the actual data.
			The actual data is stored in a static container accessible only to the wrapper.
			Because of this, the wrappers are lightweight objects, which can be freely copied.
			The data the wrappers point to is reference counted via ctors and dtor.
			The reference counting mechanism is part of the data container.
			The data type MUST be default constructible.
			The data is only mutable via an overloaded function 'value'.
		Example usage:
			State<int> number;
			State<int> number2(10);
			State<int> number3 = number2;
			number = number3;
			number.value(42);
			int numberData = number.value();
			const int& numberDataReference = number2.value();
	*/
	/*
	template<typename T> class State : public BaseState
	{
	private:
		Util::DataStore<T>& _dataStore;
		int _myDataIndex;
	public:
		State()
			: _dataStore(getDataStoreReference()), _myDataIndex(_dataStore.AcquireDataIndex())
		{}
	
		State(const T& value)
			: _dataStore(getDataStoreReference()), _myDataIndex(_dataStore.AcquireDataIndex())
		{
			_dataStore.getData(_myDataIndex) = value;
		}

		State(const State& rhs)
			: _dataStore(getDataStoreReference()), _myDataIndex(_dataStore.AcquireDataIndex(rhs._myDataIndex))
		{}

		State& operator=(const State& rhs)
		{
			if(this != &rhs)
			{
				_dataStore.ReleaseDataIndex(_myDataIndex);
				_myDataIndex = _dataStore.AcquireDataIndex(rhs._myDataIndex);
			}
			return *this;
		}

		~State()
		{
			_dataStore.ReleaseDataIndex(_myDataIndex);
		}

		const T& value() const
		{
			return _dataStore.getData(_myDataIndex);
		}

		void value(const T& val)
		{
			_dataStore.getData(_myDataIndex) = val;
		}
	};
	*/
}

#endif //CORE_STATE_STATE_H_
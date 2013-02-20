//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/State/State.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	Util::DataStoreRepository* State::_storeRepo = nullptr;

	void State::BindStoreRepository(Util::DataStoreRepository& repo)
	{
		_storeRepo = &repo;
	}

	void State::AddRef(int handle)
	{
		if(_dataStore != nullptr)
		{
			_dataHandle = _dataStore->AcquireDataIndex(handle);
		}
	}

	void State::Release()
	{
		if(_dataStore != nullptr)
		{
			_dataStore->ReleaseDataIndex(_dataHandle);
		}
	}

	State::State()
		: _dataStore(nullptr), _dataHandle(NOT_FOUND)
	{}

	State::State(const State& rhs)
		: _dataStore(nullptr), _dataHandle(NOT_FOUND)
	{
		_dataStore = rhs._dataStore;
		AddRef(rhs._dataHandle);
	}

	State::State(State&& rhs)
		: _dataStore(rhs._dataStore), _dataHandle(rhs._dataHandle)
	{
		rhs._dataStore = nullptr;
		rhs._dataHandle = NOT_FOUND;
	}

	State& State::operator=(const State& rhs)
	{
		if(this != &rhs)
		{
			Release();
			_dataStore = rhs._dataStore;
			AddRef(rhs._dataHandle); 
		}
		return *this;
	}

	State& State::operator=(State&& rhs)
	{
		if(this != &rhs)
		{
			Release();
			_dataStore = rhs._dataStore;
			_dataHandle = rhs._dataHandle;
				
			rhs._dataStore = nullptr;
			rhs._dataHandle = NOT_FOUND;
		}
		return *this;
	}

	State::~State()
	{
		Release();
	}

	bool State::isValid()
	{
		return _dataHandle != NOT_FOUND && _dataStore != nullptr;
	}
}
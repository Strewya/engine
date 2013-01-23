//headers should be ordered alphabetically, if not REORDER THEM NOW!
	/*** personal header ***/
#include "Core/State/State.h"
	/*** C++ headers ***/
	/*** extra headers ***/
	/*** end headers ***/

namespace Core
{
	void State::AddRef(uint handle)
	{
		if(_dataStore != nullptr)
		{
			_myDataIndex = _dataStore->AcquireDataIndex(handle);
		}
	}

	void State::Release()
	{
		if(_dataStore != nullptr)
		{
			_dataStore->ReleaseDataIndex(_myDataIndex);
		}
	}

	State::State()
		: _dataStore(nullptr), _myDataIndex(NOT_FOUND)
	{}

	State::State(const State& rhs)
		: _dataStore(nullptr), _myDataIndex(NOT_FOUND)
	{
		Release();
		_dataStore = rhs._dataStore;
		AddRef(rhs._myDataIndex);
	}

	State::State(State&& rhs)
		: _dataStore(rhs._dataStore), _myDataIndex(rhs._myDataIndex)
	{
		rhs._dataStore = nullptr;
		rhs._myDataIndex = NOT_FOUND;
	}

	State& State::operator=(const State& rhs)
	{
		if(this != &rhs)
		{
			Release();
			_dataStore = rhs._dataStore;
			AddRef(rhs._myDataIndex); 
		}
		return *this;
	}

	State& State::operator=(State&& rhs)
	{
		if(this != &rhs)
		{
			Release();
			_dataStore = rhs._dataStore;
			_myDataIndex = rhs._myDataIndex;
				
			rhs._dataStore = nullptr;
			rhs._myDataIndex = NOT_FOUND;
		}
		return *this;
	}

	State::~State()
	{
		Release();
	}

	bool State::isValid()
	{
		return _myDataIndex != NOT_FOUND && _dataStore != nullptr;
	}
}
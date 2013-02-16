
	/*** personal header ***/
#include "SGEngine.h"
	/*** extra headers ***/
#include "SGGameState.h"
#include "SGGameForm.h"
#include "SGGameAction.h"
#include "SGGameEntity.h"

#include "SGIGraphics.h"
#include "SGISound.h"
#include "SGScriptEngine.h"
#include "SGInputSystem.h"
	/*** end headers ***/

namespace SG
{
	Engine::Engine() : _hwnd(NULL)
	{
		InitializeInputConstants();
		getServices().Register(this);
	}

	void Engine::Init(HWND hwnd)
	{
		_hwnd = hwnd;
		_focused = true;

		SYSTEMTIME st;
		GetSystemTime(&st);
		Logger log;

		log << log.endline;
		log << "*********************************************" << log.endline;
		log	<< "* New program start @ ";
		log << st.wHour << ":" << st.wMinute << ":" << st.wSecond << " on " << st.wDay << "." << st.wMonth << "." << st.wYear << ".  ";
		log << "Application: " << _appSettings.GetTitle() << log.endline;
		log << "*********************************************" << log.endline << log.endline;

		/*
		String s = "my game is awesome";
		const std::collate<char>& coll = std::use_facet<std::collate<char> >(std::locale());
		long h = coll.hash(s.data(), s.data() + s.length());
		s += ".";
		long g = coll.hash(s.data(), s.data() + s.length());
		*/

		Entity* e = NULL;
		spAction initer(BasicInit::Make(*e));
		initer->setActive(true).Update(0);

		FormFactory::LoadTemplatesFromFile();
		EntityFactory::LoadTemplatesFromFile();
		getServices().getGraphics().LoadGraphics("resources.xml");
		getServices().getScript().File("scripts/main.lua").Do();
		
		_game = EntityFactory::Construct("game");

		ActionPriorityQueue::Ptr pq = ActionPriorityQueue::Make(*_game);
		Action::setDefaultQueue(pq);
		Action::getDefaultQueue().setActive(true);
		_game->getActions().Insert(pq);

		spAction draw = DrawGame::Make(*_game);
		draw->Start();
		_game->getActions().Insert(draw);
		
		
		if(_game->getActions().Exists("Startup"))
		{
			_game->getActions().Get("Startup")->Start();
		}
		else
		{
			String msg = "No startup action found in the game entity!";
			Logger() << msg << Logger::endline;
			int res = MessageBox(_hwnd, (msg + " Do you want to continue?").c_str(), "Warning", MB_YESNO | MB_ICONWARNING);
			if(res == IDNO)
				Shutdown();
		}


		_accumulator = 0;
		_oldFrameTime = GetTickCount();
	}

	Engine::~Engine()
	{
	}

	void Engine::Loop()
	{
		TIME currentFrameTime = GetTickCount();
		if(_focused)
		{
			_accumulator += currentFrameTime - _oldFrameTime;
			TIME timer = _appSettings.GetUpdateTime();
			if(_accumulator >= timer)
			{
				_accumulator -= timer;
				getServices().getInput().Dispatch();
				getServices().getInput().Clear();
				Action::getDefaultQueue().Update(timer);
				getServices().getSound().Update(timer);
			}
		}
		_oldFrameTime = currentFrameTime;
	}

	void Engine::Shutdown()
	{
		PostMessage(_hwnd, WM_DESTROY, 0, 0);
	}

	AppSettings& Engine::GetAppSettings()
	{
		return _appSettings;
	}

	HWND Engine::GetWindowHandle()
	{
		return _hwnd;
	}

	Entity* Engine::GetGame()
	{
		return _game.get();
	}

	void Engine::Focus()
	{
		_focused = true;
	}

	void Engine::Defocus()
	{
		_focused = false;
	}
}
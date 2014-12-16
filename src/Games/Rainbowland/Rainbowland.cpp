//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/Rainbowland.h>
/******* C++ headers *******/
#include <algorithm>
/******* extra headers *******/
#include <Games/GameLoopParams.h>
#include <Games/Rainbowland/InputBindings.h>
#include <Input/KeyCodes.h>
#include <Util/CollisionChecks.h>
#include <Util/Random.h>
#include <Util/ResourceFile.h>
#include <Util/Utility.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
#ifndef DEPLOY
	bool g_spawnEnabled = true;
	bool step = false;
	bool stepBack = false;
	double debugPauseTime = Time::STOP_TIME;
	uint32_t mx = 0;
	uint32_t my = 0;
#endif

	void RainbowlandGame::shutdown()
	{
		DEBUG_INFO("---------------------------------");
		DEBUG_STATUS(true);
		m_window->lockCursor(false);
		m_window->showCursor(true);

		status &= m_textureCache.shutdown();
		status &= m_fontCache.shutdown();
		status &= m_guiSystem.shutdown();
		status &= m_luaSystem.shutdown();
		status &= m_inputSystem.shutdown();
		status &= m_graphicsSystem.shutdown();
		m_window->setFullscreen(false);
		if(!status)
		{
			DEBUG_INFO("\nShutdown has failed! Bugs...");
		}
	}

	bool RainbowlandGame::init(Window& window)
	{
		m_window = &window;

		window.resize(1200, 900);
		window.showCursor(true);
		
		m_isRunning =
			m_graphicsSystem.init(m_fontCache, m_textureCache, window) &&
			m_inputSystem.init(window) &&
			m_luaSystem.init() &&
			m_guiSystem.init({(float)window.getSizeX(), (float)window.getSizeY()});
			m_fontCache.init(m_textureCache) &&
			m_imageCache.init(m_textureCache) &&
			m_textureCache.init(m_graphicsSystem) &&
			true;

		window.setFullscreen(true);

		if(m_isRunning)
		{
			setupInputBindings(*this);

			m_isRunning &= m_graphicsSystem.initVertexShader("Shaders/shader.hlsl");
			m_isRunning &= m_graphicsSystem.initPixelShader("Shaders/shader.hlsl");

			m_isRunning &= (bool)m_textureCache.load("Textures/font_t.png");
			m_isRunning &= (bool)m_textureCache.load("Textures/background.png");
			m_isRunning &= (bool)m_textureCache.load("Textures/shoota.png");
			m_isRunning &= (bool)m_textureCache.load("Textures/splatter.png");
			m_isRunning &= (bool)m_textureCache.load("Textures/bonus_bad.png");
			m_isRunning &= (bool)m_fontCache.load("Defs/font.font", m_luaSystem.getStack());
			m_defaultFont = m_fontCache.getResourceID("font");
			m_backgroundTexture = m_textureCache.getResourceID("Textures/background.png");
			m_charsTexture = m_textureCache.getResourceID("Textures/shoota.png");
			m_splatterTexture = m_textureCache.getResourceID("Textures/splatter.png");
			m_bonusTexture = m_textureCache.getResourceID("Textures/bonus_bad.png");
		}

		m_logicTimer.setTimeScale(Time::NORMAL_TIME);
		m_renderTimer.setTimeScale(Time::NORMAL_TIME);

		m_splatterImageDatabase =
		{
			{{92, 98}, 193, 115},
			{{453, 87}, 184, 128},
			{{799, 89}, 196, 145},
			{{1153, 83}, 285, 181},
			{{1573, 81}, 227, 151},
			
			{{85, 369}, 211, 162},
			{{494, 336}, 246, 200},
			{{888, 354}, 262, 160},
			{{1287, 350}, 236, 186},
			{{1714, 378}, 151, 112},
			
			{{61, 650}, 224, 160},
			{{497, 623}, 186, 192},
			{{949, 640}, 143, 105},
			{{1304, 648}, 212, 148},
			{{1656, 652}, 225, 132}
		};
		for( auto& r : m_splatterImageDatabase )
		{
			r.halfWidth /= 2;
			r.halfHeight /= 2;
			r.center.x += r.halfWidth;
			r.center.y += r.halfHeight;
		}

		for( uint32_t i = 0; i < BonusTypeCount; ++i )
		{
			m_bonusImageDatabase.emplace_back(Rect{{24 + static_cast<float>(i * 48), 24}, 24, 24});
		}
		for( uint32_t i = 0; i < WeaponTypeCount; ++i )
		{
			m_weaponImageDatabase.emplace_back(Rect{{24 + static_cast<float>(i * 48), 3 * 24}, 24, 24});
		}
		initGame(*this);

		DEBUG_INFO("---------------------------------");
		return m_isRunning;
	}

	bool RainbowlandGame::tick()
	{
		float fraction = 0;
		uint64_t unusedMicros = 0;
		uint64_t droppedTime = 0;
		static const uint64_t microsPerFrame = CORE_MICROS_PER_FRAME;

		for(uint32_t l = getLogicUpdateCount(m_logicTimer, microsPerFrame, fraction, unusedMicros, droppedTime); l--;)
		{
			if(!tickLogic(microsPerFrame))
			{
				m_isRunning = false;
				break;
			}
		}
		m_logicTimer.updateBy(droppedTime);

		uint64_t fullUpdateTime = m_logicTimer.getLastRealTimeMicros() + unusedMicros - m_renderTimer.getLastRealTimeMicros();
		tickRender(fullUpdateTime);
		
		return m_isRunning;
	}

	bool RainbowlandGame::tickLogic(uint64_t updateTime)
	{
		bool continueRunning = true;
		m_logicTimer.updateBy(updateTime);

		m_inputSystem.update(m_logicTimer);
		auto evs = m_inputSystem.getEvents();
		for(auto& e : evs)
		{
			if(!m_guiSystem.handleEvent(e))
			{
				for(auto& f : m_messageHandlers)
				{
					f(e);
				}
			}
		}
#ifndef DEPLOY
		if(step)
		{
			auto currentScale = m_gameplayTimer.getTimeScale();
			m_gameplayTimer.setTimeScale(debugPauseTime);
			debugPauseTime = currentScale;
		}
		if(stepBack)
		{
			auto currentScale = m_gameplayTimer.getTimeScale();
			m_gameplayTimer.setTimeScale(-debugPauseTime);
			debugPauseTime = currentScale;
		}
#endif
		m_gameplayTimer.updateBy(m_logicTimer.getDeltaMicros());
#ifndef DEPLOY
		if(step)
		{
			step = false;
			auto currentScale = m_gameplayTimer.getTimeScale();
			m_gameplayTimer.setTimeScale(debugPauseTime);
			debugPauseTime = currentScale;
		}
		if(stepBack)
		{
			stepBack = false;
			auto currentScale = -m_gameplayTimer.getTimeScale();
			m_gameplayTimer.setTimeScale(debugPauseTime);
			debugPauseTime = currentScale;
		}
#endif

		for(auto& obj : m_players)
		{
			obj.objectTimer.updateBy(m_gameplayTimer.getDeltaMicros());
		}
		for(auto& obj : m_monsters)
		{
			obj.objectTimer.updateBy(m_gameplayTimer.getDeltaMicros());
		}
		for(auto& obj : m_monsterSpawners)
		{
			obj.objectTimer.updateBy(m_gameplayTimer.getDeltaMicros());
		}
		for(auto& obj : m_bullets)
		{
			obj.objectTimer.updateBy(m_gameplayTimer.getDeltaMicros());
		}
		for(auto& obj : m_pickups)
		{
			obj.objectTimer.updateBy(m_gameplayTimer.getDeltaMicros());
		}

#ifndef DEPLOY
		if(g_spawnEnabled)
#endif
		//updateMonsterSpawners(*this);
		runMonsterAI(*this);
		
		movePlayers(*this);
		moveMonsters(*this);
		moveBullets(m_bullets);
		fixupCamera(*this);
		
		orientPlayers(m_players);
		orientMonsters(m_monsters);

		fireWeapons(*this);
		
		//checkMonsterHurtingPlayer(*this);
		checkPickups(*this);

		checkPlayerDeath(*this);
		checkLevelup(*this);
		
		updateBullets(m_bullets, m_monsters);
		updateBonuses(*this);
		updatePickups(*this);
		updateDefenseMatrix(*this);
		updateTimeCapsule(*this);
		
		updatePerks(*this);
		VKillLocations locations;
		killMonsters(*this, locations);
		generatePickups(locations, *this);
		generateSplatter(locations, *this);
		
		updateGuiLabels(*this);

		if(m_players.size() == 0)
		{
			m_deathTimer.updateBy(m_logicTimer.getDeltaMicros());
			if(m_deathTimer.getCurrentMicros() >= (uint64_t)Time::secondsToMicros(5))
			{
				cleanGame(*this);
				initGame(*this);
			}
		}

		return continueRunning;
	}








	void RainbowlandGame::tickRender(uint64_t updateTime)
	{
		m_renderTimer.updateBy(updateTime);

		m_graphicsSystem.begin();
	
		m_graphicsSystem.setPerspectiveProjection();
		m_graphicsSystem.applyCamera(m_camera);


		{
			//draw splatters to texture render target
			m_graphicsSystem.v3_setTextureAsRenderTarget();
			auto indices = m_graphicsSystem.v3_makeSolidQuadIndices();
			m_graphicsSystem.setTransparencyMode(true);
			m_graphicsSystem.v3_setTexture(m_splatterTexture);
			Vec2 splatterImgSize{1969, 885};
			for(auto splatter : m_splatters)
			{
				auto sdb = m_splatterImageDatabase[splatter.splatterImage];
				float ratio = sdb.halfWidth / sdb.halfHeight;
				sdb.halfWidth /= splatterImgSize.x;
				sdb.halfHeight /= splatterImgSize.y;
				sdb.center /= splatterImgSize;
				auto vertices = m_graphicsSystem.v3_makeQuadVertices({}, {ratio, 1});
				vertices[0].setTextureCoords(sdb.left(), sdb.bottom());
				vertices[1].setTextureCoords(sdb.right(), sdb.bottom());
				vertices[2].setTextureCoords(sdb.left(), sdb.top());
				vertices[3].setTextureCoords(sdb.right(), sdb.top());
				
				m_graphicsSystem.v3_setVertices(vertices);
				m_graphicsSystem.v3_setIndices(indices);
				m_graphicsSystem.v3_setInstanceData({splatter.transform}, {splatter.color});
				m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				m_graphicsSystem.v3_draw(indices.size(), 1);
			}
			
			m_splatters.clear();
			m_graphicsSystem.v3_clearTextureAsRenderTarget();
		}
		m_graphicsSystem.setPerspectiveProjection();
		m_graphicsSystem.applyCamera(m_camera);
		{
			auto vertices = m_graphicsSystem.v3_makeQuadVertices({}, m_playingField.halfSize());
			vertices[0].setTextureCoords(0, 0);
			vertices[1].setTextureCoords(1, 0);
			vertices[2].setTextureCoords(0, 1);
			vertices[3].setTextureCoords(1, 1);
			auto indices = m_graphicsSystem.v3_makeSolidQuadIndices();
			
			m_graphicsSystem.v3_setVertices(vertices);
			m_graphicsSystem.v3_setIndices(indices);
			m_graphicsSystem.v3_setInstanceData({{}}, {{0.6f, 0.6f, 0.6f}});
			m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			m_graphicsSystem.setTransparencyMode(false);
			m_graphicsSystem.v3_setTexture(m_backgroundTexture);
			m_graphicsSystem.v3_draw(indices.size(), 1);
			
			//draw the texture render target over the screen
			m_graphicsSystem.setTransparencyMode(true);
			m_graphicsSystem.v3_setTextureFromRenderTarget();
			m_graphicsSystem.v3_draw(indices.size(), 1);
		}

		{
			// pickups
			for( auto& pickup : m_pickups )
			{
				auto imageRect = pickup.bonus != BonusTypeCount ?
					m_bonusImageDatabase[pickup.bonus] :
					m_weaponImageDatabase[pickup.weapon];
				imageRect.center /= {240, 240};
				imageRect.halfWidth /= 240;
				imageRect.halfHeight /= 240;

				drawTexturedQuad(m_graphicsSystem, m_bonusTexture, imageRect, {1, 1}, pickup.transform, pickup.color);
			}
		}

		{
			//monsters
			Rect r{{122, 866}, 120, 88};
			r.center /= Vec2{986, 971};
			r.halfWidth /= 986;
			r.halfHeight /= 971;
			float ratio = r.halfWidth / r.halfHeight;
			std::vector<Transform> tfs;
			std::vector<Color> fills;
			tfs.reserve(m_monsters.size());
			fills.reserve(m_monsters.size());
			for(auto& obj : m_monsters)
			{
				tfs.emplace_back(obj.transform);
				fills.emplace_back(obj.color);
			}

			drawMultipleTexturedQuads(m_graphicsSystem, m_charsTexture, r, {ratio, 1}, tfs, fills);

			auto inds = m_graphicsSystem.v3_makeHollowCircleIndices(18);
			m_graphicsSystem.v3_setVertices(m_graphicsSystem.v3_makeCircleVertices({}, 0.8f, 18));
			m_graphicsSystem.v3_setIndices(inds);
			m_graphicsSystem.v3_setInstanceData(tfs, fills);
			m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			m_graphicsSystem.v3_draw(inds.size(), m_monsters.size());
		}
		{
			//players
			Rect r{{514, 71}, 94, 51};
			r.center /= Vec2{986, 971};
			r.halfWidth /= 986;
			r.halfHeight /= 971;
			float ratio = r.halfWidth / r.halfHeight;

			auto vertices = m_graphicsSystem.v3_makeQuadVertices({}, {ratio, 1});
			vertices[0].setTextureCoords(r.left(), r.bottom());
			vertices[1].setTextureCoords(r.right(), r.bottom());
			vertices[2].setTextureCoords(r.left(), r.top());
			vertices[3].setTextureCoords(r.right(), r.top());
			auto indices = m_graphicsSystem.v3_makeSolidQuadIndices();
			
			m_graphicsSystem.v3_setVertices(vertices);
			m_graphicsSystem.v3_setIndices(indices);
			m_graphicsSystem.v3_setTexture(m_charsTexture);
			m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			std::vector<Transform> tfs;
			std::vector<Color> fills;
			tfs.reserve(m_players.size());
			fills.reserve(m_players.size());
			for(auto& obj : m_players)
			{
				tfs.emplace_back(obj.transform);
				fills.emplace_back();
			}
			m_graphicsSystem.v3_setInstanceData(tfs, fills);
			m_graphicsSystem.v3_draw(indices.size(), m_players.size());
#ifdef _DEBUG
			auto inds = m_graphicsSystem.v3_makeHollowCircleIndices(18);
			m_graphicsSystem.v3_setVertices(m_graphicsSystem.v3_makeCircleVertices({}, 1.0f, 18));
			m_graphicsSystem.v3_setIndices(inds);
			m_graphicsSystem.v3_setInstanceData(tfs, fills);
			m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			m_graphicsSystem.v3_draw(inds.size(), m_players.size());
#endif
			for(auto& obj : m_players)
			{
				if(obj.currentWeapon.ammo == 0)
				{
					Transform textTf{obj.transform.position, {0.03f, 0.03f}, 0};
					drawText(m_graphicsSystem, m_defaultFont, "RELOADING", textTf, {0, 0, 0}, TJ_Center, false);
					textTf.position.y -= 1;
					auto timeRemaining = Time::microsToSeconds(obj.currentWeapon.reloadDelay - obj.weaponTimer.getCurrentMicros());
					std::string remaining = std::to_string(timeRemaining);
					drawText(m_graphicsSystem, m_defaultFont, remaining, textTf, {0, 0, 0}, TJ_Center, false);
				}
			}
		}
		
		for(auto& obj : m_bullets)
		{
			drawLine(m_graphicsSystem, obj.trail, {1, 1, 1, 0}, obj.position, {1, 1, 1, 1}, {}, {});
		}

		if(m_defenseMatrixActive)
		{
			Transform t{m_defenseMatrixArea.center, {1,1}, 0};
			drawSolidCircle(m_graphicsSystem, m_defenseMatrixArea.radius, 36, t, {0.25f, 0.42f, 0.76f, 0.2f});
		}
		if(m_timeCapsuleActive)
		{
			Transform t{m_timeCapsuleArea.center, {1, 1}, 0};
			auto vertices = m_graphicsSystem.v3_makeCircleVertices({}, m_timeCapsuleArea.radius, 36);
			auto indices = m_graphicsSystem.v3_makeSolidCircleIndices(36);
			for(auto& v : vertices)
			{
				v.setDiffuse(0.75f, 0.42f, 0.2f, 0.0f);
			}
			vertices[0].setDiffuse(0.75f, 0.42f, 0.2f, 0.8f);
			
			m_graphicsSystem.v3_setVertices(vertices);
			m_graphicsSystem.v3_setIndices(indices);
			m_graphicsSystem.v3_setInstanceData({t}, {{}});
			m_graphicsSystem.v3_setTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_graphicsSystem.v3_draw(indices.size(), 1);
		}

		//****************************
		//			gui from now on
		//****************************
		m_graphicsSystem.clearCamera();
		m_graphicsSystem.setOrthographicProjection();
		if(m_players.size() == 0)
		{
			drawText(m_graphicsSystem, m_defaultFont, "HAHA YOU ARE DEAD", {}, {0, 0, 0}, TJ_Center, false);
		}

		Transform tf;
		tf.scale.set(0.75f, 0.75f);
		tf.position.set(10-(float)m_window->getSizeX()*0.5f, 300);
		for(auto& player : m_players)
		{
			for(auto& b : player.bonuses)
			{
				std::string text = m_bonusDatabase[b.type].name;
				auto remaining = b.duration - b.timer.getCurrentMicros();
				text += " " + std::to_string(static_cast<uint32_t>(Time::microsToSeconds(remaining) + 1));
				drawText(m_graphicsSystem, m_defaultFont, text, tf, {0, 0, 0}, TJ_Left, false);

				tf.position.y -= 20;
			}
		}
#ifdef _DEBUG
		{
			auto text = std::to_string(m_gameplayTimer.getTimeScale());
			drawText(m_graphicsSystem, m_defaultFont, text, tf, {0, 0, 0}, TJ_Left, false);

			tf.position.y -= 20;

			text = std::to_string(m_camera.getPosition().z);
			drawText(m_graphicsSystem, m_defaultFont, text, tf, {0, 0, 0}, TJ_Left, false);
			tf.position.y -= 20;
		}
#endif
		int64_t timeLeft = m_defenseMatrixMicros - m_defenseMatrixTimer.getCurrentMicros();
		uint32_t displayTime = static_cast<uint32_t>(Time::microsToSeconds(timeLeft) + 1);
		if(timeLeft < 0) displayTime = 0;
		{
			auto text = "Defense matrix: " + std::to_string(displayTime);
			drawText(m_graphicsSystem, m_defaultFont, text, tf, {0, 0, 0}, TJ_Left, false);
		}
		tf.position.y -= 20;
		
		timeLeft = m_timeCapsuleMicros - m_timeCapsuleTimer.getCurrentMicros();
		displayTime = static_cast<uint32_t>(Time::microsToSeconds(timeLeft) + 1);
		if(timeLeft < 0) displayTime = 0;
		{
			auto text = "Time capsule: " + std::to_string(displayTime);
			drawText(m_graphicsSystem, m_defaultFont, text, tf, {0, 0, 0}, TJ_Left, false);
		}

		m_guiSystem.draw(m_graphicsSystem);

		m_graphicsSystem.setPerspectiveProjection();
		m_graphicsSystem.applyCamera(m_camera);

		if( m_players.size() > 0 )
		{
			Transform t;
			t.position = m_players[0].aim;
			t.scale.set(0.1f, 0.1f);
			drawSolidCircle(m_graphicsSystem, 1, 18, t, {});
		}


		/*
		static Transform framerateTf;
		framerateTf.position.set(0.5f*m_window->getSizeX() - 170, 0.5f*m_window->getSizeY() - 10);
		framerateTf.scale.set(0.5f, 0.5f);
		m_graphicsSystem.setOrthographicProjection();
		m_graphicsSystem.clearCamera();
		m_graphicsSystem.drawText(m_defaultFont, "ms per frame: " + std::to_string(Time::microsToMilis(m_logicTimer.getDeltaMicros())), framerateTf, Color(0, 0, 0), 0, false);
		*/

		m_graphicsSystem.present();
	}
}

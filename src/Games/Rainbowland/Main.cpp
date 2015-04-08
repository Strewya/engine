//headers should be ordered alphabetically, if not REORDER THEM NOW!
/******* precompiled header *******/
#include <stdafx.h>
/******* personal header *******/
#include <Games/Rainbowland/Main.h>
/******* C++ headers *******/
/******* extra headers *******/
#include <Graphics/Camera.h>
#include <Graphics/Mesh/Mesh.h>
#include <Graphics/Vertex.h>
#include <Util/Cache.h>
#include <Util/Color.h>
#include <Util/Transform.h>
#include <Util/Utility.h>
#include <Window/Window.h>
/******* end headers *******/

namespace Core
{
   bool Game::shutdown()
   {
      CORE_SHUTDOWN_START(Rainbowland);

      CORE_STATUS_AND(graphicsSystem.shutdown());

      CORE_SHUTDOWN_END(Rainbowland);
   }

   bool Game::init(Window& window)
   {
      CORE_INIT_START(Rainbowland);

      this->window = &window;

      window.resize(MONITOR_RESOLUTION, MONITOR_RESOLUTION);
      window.showCursor(true);

      //window.setFullscreen(true);

      CORE_STATUS_AND(graphicsSystem.init(window, {1200, 900}));

      textureAtlasHandle = graphicsSystem.textures.loadFromFile(CORE_RESOURCE("Textures/rainbowland_atlas.tif"));
      mainShaderHandle = graphicsSystem.shaders.loadFromFile(CORE_RESOURCE("Shaders/shader"), Vertex::getDescription());
      healthShaderHandle = graphicsSystem.shaders.loadFromFile(CORE_RESOURCE("Shaders/health"), Vertex::getDescription());

      CORE_STATUS_AND(textureAtlasHandle.isNull() == false);
      CORE_STATUS_AND(mainShaderHandle.isNull() == false);
      CORE_STATUS_AND(healthShaderHandle.isNull() == false);

      CORE_INIT_END(Rainbowland);
   }

   bool Game::tickLogic(uint64_t updateTime)
   {
      bool running = true;

      return running;
   }

   void Game::tickRender(uint64_t updateTime)
   {
      struct Dude
      {
         Transform transform;
         struct VisualPart
         {
            Color color;
            struct MeshId
            {
               int id;
            } meshId;
         } visualPart;
      } guy;

      Mesh mesh = makeSolidQuad({}, {1, 1});
      mesh.vertices[0].texCoord.set(0, 0);
      mesh.vertices[1].texCoord.set(1, 0);
      mesh.vertices[2].texCoord.set(0, 1);
      mesh.vertices[3].texCoord.set(1, 1);

      Camera camera;
      camera.setPosition({0, 0, -10});
      
      graphicsSystem.begin();

      graphicsSystem.setPerspectiveProjection();
      graphicsSystem.applyCamera(camera);
      
      //graphicsSystem.renderer.setCulling(true);
      //graphicsSystem.renderer.setTransparency(false);
      graphicsSystem.renderer.setTexture(graphicsSystem.textures.getData(textureAtlasHandle));
      graphicsSystem.renderer.setShader(graphicsSystem.shaders.getData(mainShaderHandle));
      graphicsSystem.renderer.setVertexTopology(TriangleList);
      graphicsSystem.renderer.render(Transform{}, Color{}, mesh.vertices, mesh.indices);
      
      /*
      auto& mesh = m_meshStore.getMesh(guy.visualPart.meshId);
      m_gfxProxy.renderMesh(guy.transform, guy.color, mesh);
         m_graphicsSystem.setTexture(mesh.textureId);
         m_graphicsSystem.setShader(mesh.shaderId);
         m_graphicsSystem.setBlendMode(mesh.blendModeId);
         m_graphicsSystem.renderMesh(guy.transform, guy.color, mesh.vertices, mesh.indices);
      */
      graphicsSystem.present();
   }
}

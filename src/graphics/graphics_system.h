#pragma once
/********************************************
*  contents:   main graphics system interface
*  usage:
********************************************/
/******* c++ headers *******/
#include <array>
/******* common headers *******/
#include "graphics/dx_include.h"
#include "utility/types.h"
/******* extra headers *******/
#include "graphics/dx_renderer.h"
#include "graphics/graphics_types.h"
#include "graphics/font/font_cache.h"
#include "graphics/shader/pixel/pixel_shader_cache.h"
#include "graphics/shader/pixel/pixel_shader_loader.h"
#include "graphics/shader/vertex/vertex_shader_cache.h"
#include "graphics/shader/vertex/vertex_shader_loader.h"
#include "graphics/texture/texture_cache.h"
#include "graphics/texture/texture_file_loader.h"
#include "utility/geometry/rect.h"
#include "utility/memory.h"
/******* end header inclusion *******/

namespace core
{
   struct Camera;
   struct Color;
   struct Mesh;
   struct Rect;
   struct Transform;
   struct Vertex;
   struct FontDescriptor;

   enum TextJustification;

   struct GraphicsSystem;

   namespace graphics
   {
      GraphicsSystem* init(Memory m, u32 textureSlots, u32 shaderSlots, u64 window, u32 width, u32 height);
      void shutdown(GraphicsSystem* gfx);

      void frameUpdate(GraphicsSystem* gfx/*, pushBufferWithSubmittedRenderables...*/);

      // #todo push buffer rendering once i find out how casey implemented it
      // meanwhile, just change the entire graphics api to function based
      
      f32 getWindowWidth(GraphicsSystem* gfx);
      f32 getWindowHeight(GraphicsSystem* gfx);

      void begin(GraphicsSystem* gfx);
      void present(GraphicsSystem* gfx);

      void setOrthographicProjection(GraphicsSystem* gfx);
      void setPerspectiveProjection(GraphicsSystem* gfx);
      void applyCamera(GraphicsSystem* gfx, const Camera& camera);
      void clearCamera(GraphicsSystem* gfx);
      v2 screenToWorld(GraphicsSystem* gfx, const Camera& cam, v2 screenPos);
      v2 worldToScreen(GraphicsSystem* gfx, const Camera& cam, v2 worldPos);

      void setCulling(GraphicsSystem* gfx, bool isEnabled);
      void setTransparency(GraphicsSystem* gfx, bool isEnabled);

      void renderMesh(GraphicsSystem* gfx, Transform t, Color c, const Mesh& mesh, Material material);

      HealthVertexBuffer allocateVertexBuffer(GraphicsSystem* gfx, u32 size);
      IndexBuffer allocateIndexBuffer(GraphicsSystem* gfx, u32 size);

      HTexture loadTextureFromFile(GraphicsSystem* gfx, const char* filename);
      HVertexShader loadVertexShaderFromFile(GraphicsSystem* gfx, const char* filename, VertexType vType);
      HPixelShader loadPixelShaderFromFile(GraphicsSystem* gfx, const char* filename);
      HFont processFont(GraphicsSystem* gfx, FontDescriptor font);

      void unload(GraphicsSystem* gfx, HTexture handle);
      void unload(GraphicsSystem* gfx, HVertexShader handle);
      void unload(GraphicsSystem* gfx, HPixelShader handle);
   }
}

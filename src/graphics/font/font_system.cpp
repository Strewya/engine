//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "graphics/font/font_system.h"
/******* c++ headers *******/
#include <string.h>
/******* extra headers *******/
#include "graphics/font/font_descriptor.h"
#include "graphics/mesh/mesh.h"
#include "graphics/vertex.h"
#include "utility/color.h"
#include "utility/geometry/rect.h"
#include "utility/geometry/vec2.h"
#include "utility/utility.h"
#include "utility/memory.h"
/******* end headers *******/

namespace core
{
   //*****************************************************************
   //          FILE STATIC FUNCTION DECLARATIONS
   //*****************************************************************
   core_internal f32 generateVertices(HealthVertexBuffer& vertices, const char* text, u32 characterCount, const Rect* glyphs, v2 scale, f32 tw, f32 th);
   core_internal f32 generateLineVertices(HealthVertexBuffer& vertices, const char* text, const Rect* glyphs, f32 tw, f32 th, v2 scale, u32 start, u32 end, f32 x, f32 y);
   core_internal void generateIndices(IndexBuffer& indices, u32 numberOfCharacters);
   core_internal void justifyAxisX(HealthVertexBuffer& vertices, u32 justify, u32 vert, f32 lineEnd, f32 boxHW);
   core_internal void justifyAxisY(HealthVertexBuffer& vertices, u32 justify, f32 fontHeight, f32 rowEnd, f32 boxHH);

   //*****************************************************************
   //          INIT
   //*****************************************************************
   bool FontSystem::init(LinearAllocator& a, TextureCache& textures)
   {
      CORE_INIT_START(FontSystem);

      m_textures = &textures;
      m_allocator.tag = "Font system allocator";
      m_allocator.allocated = 0;
      m_allocator.size = Megabytes(10);
      m_allocator.memory = allocate(a, m_allocator.size, 1);

      CORE_STATUS_AND(m_allocator.memory != nullptr);

      CORE_INIT_END;
   }

   //*****************************************************************
   //          SHUTDOWN
   //*****************************************************************
   bool FontSystem::shutdown()
   {
      CORE_SHUTDOWN_START(FontSystem);

      CORE_SHUTDOWN_END;
   }

   //*****************************************************************
   //          MAKE TEXT MESH
   //*****************************************************************
   Mesh FontSystem::makeTextMesh(const char* text, const FontDescriptor& fd, v2 scale, TextJustification justify_x, TextJustification justify_y)
   {
      Mesh result{};
      result.pshader = fd.pshader;
      result.vshader = fd.vshader;
      result.texture = fd.fontTexture;
      result.topology = TriangleList;

      auto textureData = m_textures->getData(fd.fontTexture);
      f32 textureWidth = (f32)textureData.width;
      f32 textureHeight = (f32)textureData.height;
      f32 fontHeight = fd.height*scale.y;

      auto characterCount = (u32)strlen(text);
      result.vertices.reserve(characterCount * 4);
      generateIndices(result.indices, characterCount);

      u32 lastNewline = 0;
      f32 yPosition = 0;
      for( u32 charIndex = 0; charIndex < characterCount; ++charIndex )
      {
         if( text[charIndex] == '\n' )
         {
            auto verts = (u32)result.vertices.size();
            auto x = generateLineVertices(result.vertices, text, fd.glyphs, textureWidth, textureHeight, scale, lastNewline, charIndex, 0, yPosition);
            justifyAxisX(result.vertices, justify_x, verts, x, 0);
            lastNewline = charIndex + 1;
            yPosition -= fontHeight;
         }
      }
      auto verts = (u32)result.vertices.size();
      auto x = generateLineVertices(result.vertices, text, fd.glyphs, textureWidth, textureHeight, scale, lastNewline, characterCount, 0, yPosition);
      justifyAxisX(result.vertices, justify_x, verts, x, 0);
      justifyAxisY(result.vertices, justify_y, fontHeight, 0, 0);

      return result;
   }

   //*****************************************************************
   //          MAKE TEXT MESH
   //*****************************************************************
   Mesh FontSystem::makeTextMesh(const char* text, const FontDescriptor& fd, v2 scale, TextJustification justify_x, TextJustification justify_y, Rect clipBox)
   {
      // #refactor
      // things to deal with:
      // + scaling
      // + new lines \n
      // + line breaks on words
      // + clipping inside the box for words
      // - clipping inside the box for letters
      Mesh result{};
      result.pshader = fd.pshader;
      result.vshader = fd.vshader;
      result.texture = fd.fontTexture;
      result.topology = TriangleList;

      auto textureData = m_textures->getData(fd.fontTexture);
      f32 textureWidth = (f32)textureData.width;
      f32 textureHeight = (f32)textureData.height;

      auto characterCount = (u32)strlen(text);
      result.vertices.reserve(characterCount * 4);
      generateIndices(result.indices, characterCount);

      u32 characterIndex = 0;
      u32 lineStart = 0;
      u32 lastValidBreakpoint = 0;
      f32 boxWidth = clipBox.halfSize.x * 2;
      f32 boxHeight = clipBox.halfSize.y * 2;
      f32 currentLinePosX = -clipBox.halfSize.x;
      f32 currentLinePosY = clipBox.halfSize.y;
      f32 fontHeight = fd.height*scale.y;
      f32 currentLineWidth = 0;

      for( ;; )
      {
         if( currentLinePosY - fontHeight <= -clipBox.halfSize.y )
         {
            currentLinePosY += fontHeight;
            break;
         }
         char character = text[characterIndex];
         if( character == 0 )
         {
            auto vertStart = (u32)result.vertices.size();
            auto x = generateLineVertices(result.vertices, text, fd.glyphs, textureWidth, textureHeight, scale, lineStart, characterIndex, currentLinePosX, currentLinePosY);
            justifyAxisX(result.vertices, justify_x, vertStart, x, clipBox.halfSize.x);
            break;
         }
         if( character >= 32 && character <= 126 )
         {
            Rect characterRect = fd.glyphs[character - 32];
            characterRect.halfSize *= scale;
            f32 characterWidth = characterRect.width();
            if( character == ' ' )
            {
               lastValidBreakpoint = characterIndex;
            }
            if( currentLineWidth + characterWidth >= boxWidth )
            {
               if( lineStart == lastValidBreakpoint )
               {
                  currentLineWidth += characterWidth;
                  ++characterIndex;
               }
               else
               {
                  auto end = lastValidBreakpoint;
                  auto vertStart = (u32)result.vertices.size();
                  auto x = generateLineVertices(result.vertices, text, fd.glyphs, textureWidth, textureHeight, scale, lineStart, end, currentLinePosX, currentLinePosY);
                  justifyAxisX(result.vertices, justify_x, vertStart, x, clipBox.halfSize.x);
                  characterIndex = lineStart = lastValidBreakpoint = end + 1;
                  currentLineWidth = 0;
                  currentLinePosY -= fontHeight;
               }
            }
            else
            {
               currentLineWidth += characterWidth;
               ++characterIndex;
            }
         }
         else if( character == '\n' )
         {
            auto end = characterIndex;
            auto vertStart = (u32)result.vertices.size();
            auto x = generateLineVertices(result.vertices, text, fd.glyphs, textureWidth, textureHeight, scale, lineStart, end, currentLinePosX, currentLinePosY);
            justifyAxisX(result.vertices, justify_x, vertStart, x, clipBox.halfSize.x);
            characterIndex = lineStart = lastValidBreakpoint = end + 1;
            currentLineWidth = 0;
            currentLinePosY -= fontHeight;
         }
         else
         {
            ++characterIndex;
         }
      }

      justifyAxisY(result.vertices, justify_y, fontHeight, currentLinePosY, clipBox.halfSize.y);

      return result;
   }



   //*****************************************************************
   //          GENERATE INDICES
   //*****************************************************************
   void generateIndices(IndexBuffer& indices, u32 numberOfCharacters)
   {
      indices.reserve(numberOfCharacters * 6);
      for( u32 i = 0; i < numberOfCharacters; ++i )
      {
         auto offset = i * 4;
         indices.emplace_back(offset + 0);
         indices.emplace_back(offset + 1);
         indices.emplace_back(offset + 2);
         indices.emplace_back(offset + 2);
         indices.emplace_back(offset + 3);
         indices.emplace_back(offset + 0);
      }
   }

   //*****************************************************************
   //          GENERATE VERTICES
   //*****************************************************************
   f32 generateVertices(HealthVertexBuffer& vertices, const char* text, u32 characterCount, const Rect* glyphs, v2 scale, f32 tw, f32 th)
   {
      f32 x = 0;
      f32 y = 0;
      for( u32 i = 0; i < characterCount; ++i )
      {
         Rect glyph = glyphs[text[i] - 32];
         f32 tv_top = glyph.bottom() / th;
         f32 tv_bot = glyph.top() / th;
         f32 tu_left = glyph.left() / tw;
         f32 tu_rght = glyph.right() / tw;
         glyph.halfSize *= scale;

         vertices.push_back({{x, y - glyph.height(), 0}, {1, 1, 1, 1}, {tu_left, tv_bot}, 0});
         vertices.push_back({{x, y - 0, 0}, {1, 1, 1, 1}, {tu_left, tv_top}, 0});
         x += glyph.width();
         vertices.push_back({{x, y - 0, 0}, {1, 1, 1, 1}, {tu_rght, tv_top}, 0});
         vertices.push_back({{x, y - glyph.height(), 0}, {1, 1, 1, 1}, {tu_rght, tv_bot}, 0});
      }
      return x;
   }

   //*****************************************************************
   //          GENERATE LINE VERTICES
   //*****************************************************************
   f32 generateLineVertices(HealthVertexBuffer& vertices, const char* text, const Rect* glyphs, f32 tw, f32 th, v2 scale, u32 start, u32 end, f32 x, f32 y)
   {
      while( start != end )
      {
         Rect glyph = glyphs[text[start] - 32];
         f32 tv_top = glyph.bottom() / th;
         f32 tv_bot = glyph.top() / th;
         f32 tu_left = glyph.left() / tw;
         f32 tu_rght = glyph.right() / tw;
         glyph.halfSize *= scale;

         vertices.push_back({{x, y - glyph.height(), 0}, {1, 1, 1, 1}, {tu_left, tv_bot}, 0});
         vertices.push_back({{x, y - 0, 0}, {1, 1, 1, 1}, {tu_left, tv_top}, 0});
         x += glyph.width();
         vertices.push_back({{x, y - 0, 0}, {1, 1, 1, 1}, {tu_rght, tv_top}, 0});
         vertices.push_back({{x, y - glyph.height(), 0}, {1, 1, 1, 1}, {tu_rght, tv_bot}, 0});

         ++start;
      }
      return x;
   }

   //*****************************************************************
   //          JUSTIFY AXIS X
   //*****************************************************************
   void justifyAxisX(HealthVertexBuffer& vertices, u32 justify, u32 vert, f32 lineEnd, f32 boxHW)
   {
      lineEnd += boxHW;
      auto boxEnd = boxHW * 2;
      auto offset = (boxEnd - lineEnd)*0.5f*justify;
      for( ; vert < vertices.size(); ++vert )
      {
         vertices[vert].position.x += offset;
      }
   }

   //*****************************************************************
   //          JUSTIFY AXIS Y
   //*****************************************************************
   void justifyAxisY(HealthVertexBuffer& vertices, u32 justify, f32 fontHeight, f32 rowEnd, f32 boxHH)
   {
      auto bottom = rowEnd - fontHeight - boxHH;
      auto boxEnd = boxHH * 2;
      auto offset = (boxEnd + bottom)*0.5f*justify; //bottom is negative
      for( auto& vert : vertices )
      {
         vert.position.y -= offset;
      }
   }
}

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
#include "util/color.h"
#include "util/geometry/rect.h"
#include "util/geometry/vec2.h"
#include "util/geometry/vec3.h"
#include "util/utility.h"
/******* end headers *******/

namespace core
{
   //*****************************************************************
   //          FILE STATIC FUNCTION DECLARATIONS
   //*****************************************************************
   static float generateVertices(HealthVertexBuffer& vertices, const char* text, uint32_t characterCount, const Rect* glyphs, vec2f scale, float tw, float th);
   static float generateLineVertices(HealthVertexBuffer& vertices, const char* text, const Rect* glyphs, float tw, float th, vec2f scale, uint32_t start, uint32_t end, float x, float y);
   static void generateIndices(IndexBuffer& indices, uint32_t numberOfCharacters);
   static void justifyAxisX(HealthVertexBuffer& vertices, uint32_t justify, uint32_t vert, float lineEnd, float boxHW);
   static void justifyAxisY(HealthVertexBuffer& vertices, uint32_t justify, float fontHeight, float rowEnd, float boxHH);

   //*****************************************************************
   //          INIT
   //*****************************************************************
   bool FontSystem::init(const TextureManager& textures)
   {
      CORE_INIT_START(FontSystem);

      m_textures = &textures;

      CORE_INIT_END(FontSystem);
   }

   //*****************************************************************
   //          SHUTDOWN
   //*****************************************************************
   bool FontSystem::shutdown()
   {
      CORE_SHUTDOWN_START(FontSystem);

      CORE_SHUTDOWN_END(FontSystem);
   }

   //*****************************************************************
   //          MAKE TEXT MESH
   //*****************************************************************
   Mesh FontSystem::makeTextMesh(const char* text, const FontDescriptor& fd, vec2f scale, vec2i justification)
   {
      Mesh result{};
      result.pshader = fd.pshader;
      result.vshader = fd.vshader;
      result.texture = fd.fontTexture;
      result.topology = TriangleList;

      auto textureData = m_textures->getData(fd.fontTexture);
      float textureWidth = (float)textureData.width;
      float textureHeight = (float)textureData.height;
      float fontHeight = fd.height*scale.y;

      auto characterCount = strlen(text);
      result.vertices.reserve(characterCount * 4);
      generateIndices(result.indices, characterCount);
      float width = generateVertices(result.vertices, text, characterCount, fd.glyphs, scale, textureWidth, textureHeight);
      justifyAxisX(result.vertices, justification.x, 0, width, 0);
      justifyAxisY(result.vertices, justification.y, fontHeight, 0, 0);

      return result;
   }

   //*****************************************************************
   //          MAKE TEXT MESH
   //*****************************************************************
   Mesh FontSystem::makeTextMesh(const char* text, const FontDescriptor& fd, vec2f scale, vec2i justification, Rect clipBox)
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
      float textureWidth = (float)textureData.width;
      float textureHeight = (float)textureData.height;

      auto characterCount = strlen(text);
      result.vertices.reserve(characterCount * 4);
      generateIndices(result.indices, characterCount);

      uint32_t characterIndex = 0;
      uint32_t lineStart = 0;
      uint32_t lastValidBreakpoint = 0;
      float boxWidth = clipBox.halfSize.x * 2;
      float boxHeight = clipBox.halfSize.y * 2;
      float currentLinePosX = -clipBox.halfSize.x;
      float currentLinePosY = clipBox.halfSize.y;
      float fontHeight = fd.height*scale.y;
      float currentLineWidth = 0;

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
            auto vertStart = result.vertices.size();
            auto x = generateLineVertices(result.vertices, text, fd.glyphs, textureWidth, textureHeight, scale, lineStart, characterIndex, currentLinePosX, currentLinePosY);
            justifyAxisX(result.vertices, justification.x, vertStart, x, clipBox.halfSize.x);
            break;
         }
         if( character >= 32 && character <= 126 )
         {
            Rect characterRect = fd.glyphs[character - 32];
            characterRect.halfSize *= scale;
            float characterWidth = characterRect.width();
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
                  auto vertStart = result.vertices.size();
                  auto x = generateLineVertices(result.vertices, text, fd.glyphs, textureWidth, textureHeight, scale, lineStart, end, currentLinePosX, currentLinePosY);
                  justifyAxisX(result.vertices, justification.x, vertStart, x, clipBox.halfSize.x);
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
            auto vertStart = result.vertices.size();
            auto x = generateLineVertices(result.vertices, text, fd.glyphs, textureWidth, textureHeight, scale, lineStart, end, currentLinePosX, currentLinePosY);
            justifyAxisX(result.vertices, justification.x, vertStart, x, clipBox.halfSize.x);
            characterIndex = lineStart = lastValidBreakpoint = end + 1;
            currentLineWidth = 0;
            currentLinePosY -= fontHeight;
         }
         else
         {
            ++characterIndex;
         }
      }

      justifyAxisY(result.vertices, justification.y, fontHeight, currentLinePosY, clipBox.halfSize.y);

      return result;
   }



   //*****************************************************************
   //          GENERATE INDICES
   //*****************************************************************
   void generateIndices(IndexBuffer& indices, uint32_t numberOfCharacters)
   {
      indices.reserve(numberOfCharacters * 6);
      for( uint32_t i = 0; i < numberOfCharacters; ++i )
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
   float generateVertices(HealthVertexBuffer& vertices, const char* text, uint32_t characterCount, const Rect* glyphs, vec2f scale, float tw, float th)
   {
      float x = 0;
      float y = 0;
      for( uint32_t i = 0; i < characterCount; ++i )
      {
         Rect glyph = glyphs[text[i] - 32];
         float tv_top = glyph.bottom() / th;
         float tv_bot = glyph.top() / th;
         float tu_left = glyph.left() / tw;
         float tu_rght = glyph.right() / tw;
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
   float generateLineVertices(HealthVertexBuffer& vertices, const char* text, const Rect* glyphs, float tw, float th, vec2f scale, uint32_t start, uint32_t end, float x, float y)
   {
      while( start != end )
      {
         Rect glyph = glyphs[text[start] - 32];
         float tv_top = glyph.bottom() / th;
         float tv_bot = glyph.top() / th;
         float tu_left = glyph.left() / tw;
         float tu_rght = glyph.right() / tw;
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
   void justifyAxisX(HealthVertexBuffer& vertices, uint32_t justify, uint32_t vert, float lineEnd, float boxHW)
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
   void justifyAxisY(HealthVertexBuffer& vertices, uint32_t justify, float fontHeight, float rowEnd, float boxHH)
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

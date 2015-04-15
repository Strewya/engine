#pragma once
/********************************************
*  contents:   DXRenderer
*  usage:
********************************************/
/******* C++ headers *******/
/******* common headers *******/
#include <Graphics/DXInclude.h>
/******* extra headers *******/
#include <Graphics/Typedefs.h>
#include <Graphics/Vertex.h>
#include <Util/Vec2Fwd.h>
/******* end header inclusion *******/

namespace Core
{
   class DXTexture;
   class VertexShader;
   class PixelShader;
   class Transform;
   class Color;

   enum VertexTopology
   {
      LineList,
      LineStrip,
      TriangleList,
      TriangleStrip,
   };

   class DXRenderer
   {
   public:
      bool init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11SamplerState* samplerState);
      bool shutdown();

      void setProjection(const XMMATRIX& matrix);
      void setView(const XMMATRIX& matrix);

      void setCulling(bool isEnabled);
      void setTransparency(bool isEnabled);

      void setTexture(const DXTexture& texture);
      void setVertexTopology(VertexTopology topology);
      void setShader(const VertexShader& shader);
      void setShader(const PixelShader& shader);
      void render(Transform transform, Color color, HealthVertexBuffer vertices, IndexBuffer indices);

   private:
      XMMATRIX m_camView;
      XMMATRIX m_camProjection;

      ID3D11Device* m_dev;
      ID3D11DeviceContext* m_devcon;
      ID3D11SamplerState* m_samplerState;
      //owning resources
      ID3D11BlendState* m_transparency;
      ID3D11RasterizerState* m_cullingEnabled;
      ID3D11RasterizerState* m_cullingDisabled;

      void* m_texture;
      void* m_inputLayout;
      void* m_vertexShader;
      void* m_pixelShader;
      void* m_cullingMode;
      void* m_transparencyMode;
      int32_t m_topology;

      HealthVertexBuffer m_verticesToDraw;
      IndexBuffer m_indicesToDraw;
   };
}

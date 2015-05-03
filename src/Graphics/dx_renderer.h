#pragma once
/********************************************
*  contents:   direct3d specific renderer
*  usage:
********************************************/
/******* c++ headers *******/
/******* common headers *******/
#include "graphics/dx_include.h"
/******* extra headers *******/
#include "graphics/graphics_types.h"
#include "graphics/vertex.h"
#include "util/geometry/vec2_fwd.h"
/******* end header inclusion *******/

namespace Core
{
   class Texture;
   class VertexShader;
   class PixelShader;
   class Transform;
   class Color;

   class DXRenderer
   {
   public:
      bool init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11SamplerState* samplerState);
      bool shutdown();

      void setProjection(const XMMATRIX& matrix);
      void setView(const XMMATRIX& matrix);

      void setVertexTopology(D3D_PRIMITIVE_TOPOLOGY topology);
      void setTexture(ID3D11ShaderResourceView* shaderResourceView);
      void setInputLayout(ID3D11InputLayout* layout);
      void setShader(ID3D11VertexShader* shader);
      void setShader(ID3D11PixelShader* shader);

      void setBlendState(ID3D11BlendState* blendState);
      void setRasterizerState(ID3D11RasterizerState* rasterizerState);
      void render(Transform transform, Color color, HealthVertexBuffer vertices, IndexBuffer indices);

   private:
      XMMATRIX m_camView;
      XMMATRIX m_camProjection;

      ID3D11Device* m_dev;
      ID3D11DeviceContext* m_devcon;
      ID3D11SamplerState* m_samplerState;
      
      ID3D11ShaderResourceView* m_texture;
      ID3D11InputLayout* m_inputLayout;
      ID3D11VertexShader* m_vertexShader;
      ID3D11PixelShader* m_pixelShader;
      ID3D11BlendState* m_blendState;
      ID3D11RasterizerState* m_rasterizerState;
      D3D_PRIMITIVE_TOPOLOGY m_topology;
   };
}

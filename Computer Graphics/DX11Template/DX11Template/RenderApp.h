#pragma once 
#include "BaseApp.h"
#include "Utils.h"
#include <vector>
#include "Vertex.h"
namespace soku {
	
	class RenderApp : public BaseApp {
	public:
		RenderApp(int width = 1920, int height = 1080);
		~RenderApp();

		virtual bool Initialize();
	protected:
		virtual void Update() override;
		virtual void Render() override;
		virtual void UpdateGUI(float deltaTime) override;
		DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

		Microsoft::WRL::ComPtr <ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr <ID3D11PixelShader> m_pixelShader;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_canvasTexture;
		Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_canvasShaderResourceView;
		std::vector<Vec4> pixels;
		Vec4 canvasColor;
		
		UINT m_canvasWidth;
		UINT m_canvasHeight;

	private:
		UINT m_indexCount;
	};
}
#pragma once 
#include "BaseApp.h"
#include "Utils.h"
#include <vector>
#include "Vertex.h"
#include "Geometry.h"
#include "Camera.h"

namespace soku {
	
	class RenderApp : public BaseApp {
	public:
		RenderApp(int width = 1920, int height = 1080);
		~RenderApp();

		virtual bool Initialize();
	protected:
		Vector3 TranslateWorldToScreen(const Vector2& worldPos);
		Vector3 TranselateScreenToBox(const Vector2& worldPos);
	protected:
		virtual void Update() override;
		virtual void Render() override;
		virtual void UpdateGUI(float deltaTime) override;
		void SetTime(uint64_t& currTime);
		void UpdateColor();
		void SetPixelColor(const int& x, const int& y);
		Vector4 TraceRay(const Ray& in_ray);

		Hit LoopCollision(const Ray& in_ray);

		DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

		Microsoft::WRL::ComPtr <ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr <ID3D11PixelShader> m_pixelShader;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_canvasTexture;
		Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_canvasShaderResourceView;
		std::vector<Vector4> pixels;
		Vec4 canvasColor;
		
		UINT m_canvasWidth;
		UINT m_canvasHeight;
	private:
		UINT m_indexCount;

		Camera m_camera;
		std::vector<std::shared_ptr<Model>> models;
	};
}
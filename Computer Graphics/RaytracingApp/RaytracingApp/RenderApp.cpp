#include "pch.h"
namespace soku {
	RenderApp::RenderApp(int width, int height)
		:BaseApp(width, height),
		m_indexCount(0),
		m_canvasWidth(width),
		m_canvasHeight(height)
	{
		circle = std::make_shared<Circle>(0.1f, Vector2(0.5f,0.f));	}
	bool RenderApp::Initialize()
	{
		using DirectX::SimpleMath::Vector2;
		using DirectX::SimpleMath::Vector4;
		using Microsoft::WRL::ComPtr;
		if (!BaseApp::Initialize())
		{
			return false;
		}
		const std::vector<Vertex> vertices =
		{
			{ { -1.0f, -1.0f, 0.0f, 1.0f }, { 0.f, 1.f },},
			{ {  1.0f, -1.0f, 0.0f, 1.0f }, { 1.f, 1.f },},
			{ {  1.0f,  1.0f, 0.0f, 1.0f }, { 1.f, 0.f },},
			{ { -1.0f,  1.0f, 0.0f, 1.0f }, { 0.f, 0.f },},
		};
		const std::vector<uint16_t> indices = {
			3,1,0,
			2,1,3,
		};

		
		m_indexCount = UINT(indices.size());
		// Create Vertex buffer 
		Utils::CreateVertexBuffer(vertices, m_vertexBuffer, m_device);

		// Create Index Buffer
		Utils::CreateIndexBuffer(indices, m_indexBuffer, m_device);

		// Create Input Layout
		std::vector<D3D11_INPUT_ELEMENT_DESC> elements = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		Utils::CreateVertexShaderAndInputLayout(L"VertexShader.hlsl", m_vertexShader, elements, m_inputLayout, m_device);

		// Create Pixel Shader
		Utils::CreatePixelShader(
			L"PixelShader.hlsl",
			m_pixelShader,
			m_device
		);
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.ArraySize = textureDesc.MipLevels = 1;
		textureDesc.Width = m_canvasWidth;
		textureDesc.Height = m_canvasHeight;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_device->CreateTexture2D(&textureDesc, nullptr, m_canvasTexture.GetAddressOf());
		m_device->CreateShaderResourceView(m_canvasTexture.Get(), nullptr, m_canvasShaderResourceView.GetAddressOf());

		D3D11_SAMPLER_DESC samplDesc;
		ZeroMemory(&samplDesc, sizeof(samplDesc));
		samplDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplDesc.MinLOD = 0;
		samplDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		m_device->CreateSamplerState(&samplDesc, m_samplerState.GetAddressOf());
		return true;
	}
	RenderApp::~RenderApp()
	{
	}
	void RenderApp::UpdateGUI(float deltaTime)
	{
		ImGui::SliderFloat("Center Position X", &(circle->center.x), -1.7777f, 1.7777f);
		ImGui::SliderFloat("Center Position Y", &(circle->center.y), -1.f, 1.f);
		ImGui::SliderFloat("Circle Radius", &(circle->radius), 0.0f, 1.0f);
		ImGui::SliderFloat("Circle Theta", &(circle->m_theta), 0.f, 3.141592f*2);

	}
	Vector2 RenderApp::TranslateWorldToScreen(const Vector2& worldPos)
	{
		float x = ((worldPos.x * (2.0f/ m_canvasWidth))-1.0f) * GetAspectRatio();
		float y = ((worldPos.y * (2.0f / m_canvasHeight)) - 1.0f) * -1.f;
		return Vector2(x, y);
	}
	void RenderApp::Update()
	{	
		/*float alpha = 0.01f;
		circle->m_theta += alpha;*/
		std::vector<Vec4> image(m_canvasWidth * m_canvasHeight, Vec4{ 0.8f,0.8f,0.8f,1.0f });
		for (size_t y = 0; y < m_canvasHeight; y++)
		{
			for (size_t x = 0; x < m_canvasWidth; x++)
			{
				int currIdx = y * m_canvasWidth + x;
				auto worldPos = TranslateWorldToScreen(Vector2(x, y));
				if (circle->IsInCircleTheta(worldPos))
				{
					image[currIdx] = Vec4{ 1.0f,0.0f,0.f,1.f };
				}
			}
		}
		Utils::UpdateConstantBuffer(image, m_canvasTexture, m_context);
	}
	void RenderApp::Render()
	{
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
		m_context->ClearDepthStencilView(m_depthStencilView.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
		m_context->OMSetDepthStencilState(m_ds.Get(), 0);
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		m_context->IASetInputLayout(m_inputLayout.Get());
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
		m_context->PSSetShaderResources(0, 1, m_canvasShaderResourceView.GetAddressOf());

		m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		m_context->RSSetState(m_rasterizerState.Get());

		m_context->DrawIndexed(m_indexCount, 0, 0);
	}
}
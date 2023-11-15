#include "RenderApp.h"
#include "windows.h"
namespace soku {
	RenderApp::RenderApp(int width, int height)
		:BaseApp(width, height),
		m_indexCount(0),
		m_canvasWidth(width / 80),
		m_canvasHeight(height / 80)
	{
	}
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
		m_indexCount = indices.size();

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

		// Create SamplerState
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // D3D11_FILTER_MIN_MAG_MIP_LINEAR
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.MinLOD = 0;

		m_device->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());
		
		
		D3D11_TEXTURE2D_DESC canvasDesc;
		ZeroMemory(&canvasDesc, sizeof(canvasDesc));
		canvasDesc.Width = m_canvasWidth;
		canvasDesc.Height = m_canvasHeight;
		canvasDesc.MipLevels = canvasDesc.ArraySize = 1;
		canvasDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		canvasDesc.SampleDesc.Count = 1;
		canvasDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		canvasDesc.Usage = D3D11_USAGE_DYNAMIC;
		canvasDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		m_device->CreateTexture2D(&canvasDesc, nullptr, m_canvasTexture.GetAddressOf());
		m_device->CreateShaderResourceView(m_canvasTexture.Get(), nullptr, m_basicCanvastexture.GetAddressOf());

		return true;
	}
	RenderApp::~RenderApp()
	{

	}
	void RenderApp::Update()
	{
		std::vector<Vec4> pixels(m_canvasWidth * m_canvasHeight, Vec4{ 0.8f, 0.8f, 0.8f, 1.0f });
		pixels[0 + m_canvasWidth * 0] = Vec4{ 1.0f, 0.0f, 0.0f, 1.0f };
		pixels[1 + m_canvasWidth * 0] = Vec4{ 1.0f, 1.0f, 0.0f, 1.0f };

		// Update texture buffer
		D3D11_MAPPED_SUBRESOURCE ms;
		m_context->Map(m_canvasTexture.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, pixels.data(), pixels.size() * sizeof(Vec4));
		m_context->Unmap(m_canvasTexture.Get(), NULL);

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

		m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		m_context->PSSetShaderResources(0, 1, m_basicCanvastexture.GetAddressOf());
		m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
		m_context->RSSetState(m_rasterizerState.Get());

		m_context->DrawIndexed(m_indexCount, 0, 0);
	}
}
#include "RenderApp.h"
namespace soku {
	RenderApp::RenderApp(int width, int height)
		:BaseApp(width, height),
		m_indexCount(0),
		m_canvasWidth(1920),
		m_canvasHeight(1280)
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
		m_indexCount = UINT(indices.size());

		// Create Image ShaderResourceView
		Image img1;
		img1.ReadFromFile("../Data/image_1.jpg", m_device,m_context);
		Image img2;
		img2.ReadFromFile("../Data/image_1.jpg", m_device, m_context); 
		std::vector<Image> imageData = { img1, img2};
		images = std::make_unique< std::vector<Image>>(imageData);

		__int64 prevTime;
		__int64 currTime;
		double delTime;
		/*{
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
			for (size_t i = 0; i < 100; i++)
			{
				img1.BoxBlur(m_context);
			}
			prevTime = currTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
			delTime = (currTime - prevTime) * m_timer.GetTickSeconds();
			std::cout << "elapesed time of BoxBlur() :  " << delTime << std::endl;
		}*/
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
			for (size_t i = 0; i < 2; i++)
			{
				img1.BoxBlurOMP(m_context);
			}
			prevTime = currTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
			delTime = (currTime - prevTime) * m_timer.GetTickSeconds();
			std::cout << "elapesed time of BoxBlurOMP() :  " << delTime << std::endl;
		}


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

		return true;
	}
	RenderApp::~RenderApp()
	{

	}
	void RenderApp::UpdateGUI(float deltaTime)
	{
	/*	if (ImGui::RadioButton("pause Button", m_appPaused))
		{
			m_timer.Stop();
		}*/

	}
	void RenderApp::Update()
	{
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
		
		Image* p = images->data();
		std::vector<ID3D11ShaderResourceView*> arr;
		for (size_t i = 0; i < images->size(); i++)
		{
			arr.emplace_back(p[i].m_ShaderResourceView.Get());
		}
		m_context->PSSetSamplers(0, 1, images->data()[0].m_samplerState.GetAddressOf());

		m_context->PSSetShaderResources(0, images->size(), arr.data());

		m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		
		m_context->RSSetState(m_rasterizerState.Get());

		m_context->DrawIndexed(m_indexCount, 0, 0);
	}
}
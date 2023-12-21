#include "RenderApp.h"
#include <algorithm>
namespace soku {
	RenderApp::RenderApp(int width, int height)
		:BaseApp(width, height),
		m_indexCount(0),
		m_canvasWidth(width),
		m_canvasHeight(height)
	{
		float s = 0.8f;
		Vector3 p0(-s, -s, 2.0f);
		Vector3 p1(-s, s, 2.0f);
		Vector3 p2(s,s, 2.0f);
		Vector3 p3(s, -s, 2.0f);

		//std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(p1, p2, p3);
		std::shared_ptr<Rectangle> rect = std::make_shared<Rectangle>(p0, p1, p2, p3);

		models.push_back(rect);
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
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.MinLOD = 0;
		samplerDesc.Filter = D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

		if (FAILED(m_device->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf())))
		{
			std::cout << "CreateSamplerState Failed" << std::endl;
		}

		// Create Canvas Texture
		D3D11_TEXTURE2D_DESC canvasTexture;
		ZeroMemory(&canvasTexture, sizeof(canvasTexture));
		canvasTexture.Width = m_canvasWidth;
		canvasTexture.Height = m_canvasHeight,
		canvasTexture.ArraySize = canvasTexture.MipLevels = 1;
		canvasTexture.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		canvasTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		canvasTexture.Usage = D3D11_USAGE_DYNAMIC;
		canvasTexture.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		canvasTexture.SampleDesc.Count = 1;
		
		
		if (FAILED(m_device->CreateTexture2D(&canvasTexture, nullptr, m_canvasTexture.GetAddressOf())))
		{
			std::cout << "CreateTexture2D(canvas) Failed" << std::endl;
		}

		// Create ShaderResourceView
		m_device->CreateShaderResourceView(m_canvasTexture.Get(), nullptr, m_canvasShaderResourceView.GetAddressOf());
		UpdateColor();
		return true;
	}
	Vector3 RenderApp::TranslateWorldToScreen(const Vector2& worldPos)
	{
		float xScale = 2.0f / (m_canvasWidth - 1);
		float yScale = 2.0f / (m_canvasHeight - 1);
		float aspect = (float)m_canvasWidth / m_canvasHeight;

		return Vector3(aspect * (worldPos.x * xScale - 1.0f),-1.0f*(worldPos.y * yScale-1.0f), 0.0f);
	}
	Vector3 RenderApp::TranselateScreenToBox(const Vector2& worldPos)
	{
		float xScale = 2.0f / (m_canvasWidth - 1);
		float yScale = 2.0f / (m_canvasHeight - 1);

		return Vector3((worldPos.x * xScale - 1.0f), -1.0f * (worldPos.y * yScale - 1.0f), 0.0f);
	}
	RenderApp::~RenderApp()
	{

	}
	void RenderApp::UpdateGUI(float deltaTime)
	{
		//ImGui::SliderFloat4("leble", canvasColor.v, 0.0f, 1.0f);
	}
	void RenderApp::SetTime(uint64_t& currTime) {
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	}
	void RenderApp::UpdateColor()
	{
		Vec4 color = { 1.0f,0.f,0.f,1.f };
		pixels = std::vector<Vector4>(m_canvasWidth * m_canvasHeight, Vector4{ 0.0f,0.0f,0.0f,1.0f });
		uint64_t prevTime, currTime;
		SetTime(prevTime);

		for (int y = 0; y < m_canvasHeight; y++)
		{
			for (int x = 0; x < m_canvasWidth; x++)
			{
				SetPixelColor(x, y);
			}
		}
		SetTime(currTime);
		uint64_t delTime = currTime - prevTime;
		std::cout << delTime * secondPerTick;
		Utils::UpdateConstantBuffer(pixels, m_canvasTexture, m_context);
	}
	void RenderApp::SetPixelColor(const int& x,const int& y)
	{
		int currIdx = x + y * m_canvasWidth;
		Vector2 currPos(x, y);
		Vector3 screenPos = TranslateWorldToScreen(currPos);
		Vector3 rayDir = (screenPos - m_camera.m_pos);
		rayDir.Normalize();
		Ray ray(Vector3(screenPos.x, screenPos.y, 0.0f), rayDir);
		pixels[currIdx] = TraceRay(ray);
	}
	Vector4 RenderApp::TraceRay(const Ray& in_ray)
	{
		Hit hit;
		Vector4 color(0.0f, 0.0f, 0.f, 1.f);
		hit = LoopCollision(in_ray);
		if (hit.distance > 0.0f && hit.hitModel)
		{
			return hit.hitModel->GetColor();
		}
		return color;
	}
	Hit RenderApp::LoopCollision(const Ray& in_ray)
	{
		Hit hit;
		for (size_t modelIndex = 0; modelIndex < models.size(); modelIndex++)
		{
			Hit currHit = models[modelIndex]->CheckCollision(in_ray);
			if (currHit.distance > 0.0f)
			{
				if (hit.distance < 0.0f)
					hit = currHit;
				else if (hit.distance > currHit.distance)
					hit = currHit;
			}
		}
		return hit;
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

		m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		m_context->PSSetShaderResources(0, 1, m_canvasShaderResourceView.GetAddressOf());
		m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
		m_context->RSSetState(m_rasterizerState.Get());

		m_context->DrawIndexed(m_indexCount, 0, 0);
	}
}
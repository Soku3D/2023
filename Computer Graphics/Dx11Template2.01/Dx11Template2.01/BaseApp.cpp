#include "BaseApp.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace soku
{
	BaseApp* appPtr = nullptr;

	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return appPtr->BaseProc(hWnd, msg, wParam, lParam);
	}

	BaseApp::BaseApp(int width, int height)
		:m_width(width),
		m_height(height),
		m_viewport(D3D11_VIEWPORT()),
		m_wnd(nullptr),
		msQuality(0),
		swapChainFormat(DXGI_FORMAT_B8G8R8A8_UNORM),
		m_appPaused(false)
	{
		//assert(appPtr == nullptr);
		appPtr = this;
		m_timer.Reset();
	}
	BaseApp::~BaseApp()
	{
		appPtr = nullptr;
		DestroyWindow(m_wnd);
	}
	bool BaseApp::Initialize()
	{
		if (!InitWindow())
		{
			std::cerr << "InitWindow() Failed";
			return false;
		}
		if (!InitDirectX())
		{
			std::cerr << "InitDirectX() Failed";
			return false;
		}
		if (!InitGUI())
		{
			std::cerr << "InitGUI() Failed";
			return false;
		}
		return true;
	}
	bool BaseApp::InitWindow()
	{
		WNDCLASSEX wc =
		{
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			WndProc,
			0,
			0,
			GetModuleHandle(NULL),
			NULL,
			NULL,
			NULL,
			NULL,
			L"sokuClass",
			NULL

		};
		RegisterClassEx(&wc);

		RECT rect = { 0 ,0, (LONG)m_width, (LONG)m_height };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);

		m_wnd = CreateWindow(wc.lpszClassName, L"test",
			WS_OVERLAPPEDWINDOW,
			10, 10,
			rect.right - rect.left,
			rect.bottom - rect.top,
			NULL,
			NULL,
			wc.hInstance,
			NULL
		);

		ShowWindow(m_wnd, SW_SHOWDEFAULT);
		UpdateWindow(m_wnd);

		return true;
	}
	bool BaseApp::InitDirectX()
	{
		// create device, context, swapchain
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

		const D3D_FEATURE_LEVEL featureLevels[2] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_0
		};
		D3D_FEATURE_LEVEL featureLevel;
		UINT createDeviceflags = 0;
#if defined(DEBUG) || (_DEBUG)
		createDeviceflags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		if (FAILED(D3D11CreateDevice(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			createDeviceflags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&featureLevel,
			context.GetAddressOf()
		)))
		{
			std::cout << "D3D11CreateDevice Failed";
			return false;
		}
		device->CheckMultisampleQualityLevels(
			swapChainFormat, 2, &msQuality);

		if (FAILED(device.As(&m_device)))
		{
			std::cout << " device.As() Failed";
			return false;
		}
		if (FAILED(context.As(&m_context)))
		{
			std::cout << " context.As() Failed";
			return false;
		}

		DXGI_SWAP_CHAIN_DESC scDesc;
		ZeroMemory(&scDesc, sizeof(scDesc));
		scDesc.BufferCount = 2;
		scDesc.Windowed = TRUE;
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDesc.BufferDesc.Width = m_width;
		scDesc.BufferDesc.Height = m_height;
		scDesc.BufferDesc.Format = swapChainFormat;
		scDesc.BufferDesc.RefreshRate.Denominator = 1;
		scDesc.BufferDesc.RefreshRate.Numerator = 60;
		scDesc.SampleDesc.Count = msQuality > 0 ? 4 : 1;
		scDesc.SampleDesc.Quality = msQuality > 0 ? msQuality - 1 : 0;
		scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		scDesc.OutputWindow = m_wnd;
		scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		if (FAILED(D3D11CreateDeviceAndSwapChain(
			NULL,
				D3D_DRIVER_TYPE_HARDWARE,
				NULL,
				createDeviceflags,
				featureLevels,
				1,
				D3D11_SDK_VERSION,
				& scDesc,
				m_swapChain.GetAddressOf(),
				& m_device,
				& featureLevel,
				& m_context
				)))
				{
					std::cout << "D3D11CreateDeviceAndSwapChain() Failed";
					return false;
		}

			//Create RenderTargetView
			Microsoft::WRL::ComPtr<ID3D11Texture2D> backbuffer;
			m_swapChain->GetBuffer(0, IID_PPV_ARGS(backbuffer.GetAddressOf()));
			if (backbuffer)
			{
				m_device->CreateRenderTargetView(backbuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());
			}
			else {
				std::cout << "CreateRenderTargetView() failed." << std::endl;
				return false;
			}

			//ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
			m_viewport.TopLeftX = 0;
			m_viewport.TopLeftY = 0;
			m_viewport.Width = float(m_width);
			m_viewport.Height = float(m_height);
			m_viewport.MinDepth = 0.0f;
			m_viewport.MaxDepth = 1.0f;

			D3D11_RASTERIZER_DESC rsDesc;
			ZeroMemory(&rsDesc, sizeof(rsDesc));
			rsDesc.FillMode = D3D11_FILL_SOLID;
			rsDesc.CullMode = D3D11_CULL_NONE;
			rsDesc.FrontCounterClockwise = false;
			m_device->CreateRasterizerState(&rsDesc, m_rasterizerState.GetAddressOf());

			//m_context->RSSetState(m_rasterizerState.Get());
			m_context->RSSetViewports(1, &m_viewport);


			// Create depth buffer
			Microsoft::WRL::ComPtr<ID3D11Texture2D> depthBuffer;
			D3D11_TEXTURE2D_DESC dbDesc;
			dbDesc.Width = m_width;
			dbDesc.Height = m_height;
			dbDesc.MipLevels = dbDesc.ArraySize = 1;
			dbDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			dbDesc.SampleDesc.Count = msQuality > 0 ? 4 : 1;
			dbDesc.SampleDesc.Quality = msQuality > 0 ? msQuality - 1 : 0;
			dbDesc.Usage = D3D11_USAGE_DEFAULT;
			dbDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
			dbDesc.CPUAccessFlags = 0;
			dbDesc.MiscFlags = 0;

			m_device->CreateTexture2D(&dbDesc, NULL, depthBuffer.GetAddressOf());
			m_device->CreateDepthStencilView(depthBuffer.Get(), NULL, m_depthStencilView.GetAddressOf());

			D3D11_DEPTH_STENCIL_DESC dsDesc;
			ZeroMemory(&dsDesc, sizeof(dsDesc));
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsDesc.DepthEnable = true;
			if (FAILED(m_device->CreateDepthStencilState(&dsDesc, m_ds.GetAddressOf())))
			{
				std::cout << "createDepthstencilState() failed";
				return false;
			}
			return true;
	}
	bool BaseApp::InitGUI()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.DisplaySize = ImVec2((float)m_width, (float)m_height);
		ImGui::StyleColorsLight();
		ImGui_ImplDX11_Init(m_device.Get(), m_context.Get());
		ImGui_ImplWin32_Init(m_wnd);
		return true;
	}
	LRESULT BaseApp::BaseProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		{
			return true;
		}
		switch (msg)
		{
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				m_appPaused = true;
				m_timer.Stop();
			}
			else
			{
				m_appPaused = false;
				m_timer.Start();
			}
			return 0;
		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
		}
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}
	int BaseApp::Run()
	{
		MSG msg = { 0 };
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, m_wnd, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				if (!m_appPaused)
				{
					m_timer.Tick();

					ImGui_ImplDX11_NewFrame();
					ImGui_ImplWin32_NewFrame();
					ImGui::NewFrame();
					ImGui::Begin("Gui");
					UpdateGUI(ImGui::GetIO().DeltaTime);
					ImGui::End();
					ImGui::Render();

					Update();
					Render();
					ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
					m_swapChain->Present(1, 0);
				}
				else
				{
					UpdateGUI(ImGui::GetIO().DeltaTime);
					Sleep(100);
				}
				
			}
		}
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		DestroyWindow(m_wnd);

		return (int)msg.wParam;
	}
}
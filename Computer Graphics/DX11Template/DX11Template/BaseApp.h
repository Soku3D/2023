#pragma once
#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <wrl.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
namespace soku
{
	class BaseApp {
	public:
		BaseApp(int width, int height);
		virtual ~BaseApp();

		LRESULT BaseProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		int Run();

		virtual bool Initialize();
	private:
		bool InitWindow();
		bool InitDirectX();
		bool InitGUI();
	private:
		UINT m_width;
		UINT m_height;
		D3D11_VIEWPORT m_viewport;
		HWND m_wnd;

		DXGI_FORMAT swapChainFormat;
		UINT msQuality;
	protected:
		virtual void Render() = 0;
		virtual void Update() = 0;
		virtual void UpdateGUI(float deltaTime) = 0;
	protected:
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;

		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> m_renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_ds;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;
	};
}
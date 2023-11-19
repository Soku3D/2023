#pragma once 
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include "Vertex.h"
#include "Utils.h"
namespace soku {
	class Image {
	public:
		Image();
		void ReadFromFile(const std::string& filePath,
			Microsoft::WRL::ComPtr <ID3D11Device>& device,
			Microsoft::WRL::ComPtr <ID3D11DeviceContext>& context);
	public:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
		Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> m_ShaderResourceView;
		void Render(Microsoft::WRL::ComPtr <ID3D11DeviceContext>& context);
	public:
		void BoxBlur(Microsoft::WRL::ComPtr <ID3D11DeviceContext>& context);
		void BoxBlurOMP(Microsoft::WRL::ComPtr <ID3D11DeviceContext>& context);

	private:
		int m_width;
		int m_height;
		int m_channels;
		std::vector<Vec4> m_pixels;
		std::vector<Vec4> m_blur;
		std::string filePath;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;
			
	};
}
#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <omp.h>
namespace soku {
	Image::Image()
	{
	}
	void Image::ReadFromFile(const std::string& filePath,
		Microsoft::WRL::ComPtr <ID3D11Device>& device,
		Microsoft::WRL::ComPtr <ID3D11DeviceContext>& context)
	{
		this->filePath = filePath;
		stbi_uc* image = stbi_load(filePath.c_str(), &m_width, &m_height,
			&m_channels, 0);
		m_pixels.resize(m_width * m_height);

		for (size_t i = 0; i < m_width * m_height; i++)
		{
			m_pixels[i].v[0] = image[m_channels * i ] / 255.f;
			m_pixels[i].v[1] = image[m_channels * i + 1] / 255.f;
			m_pixels[i].v[2] = image[m_channels * i + 2] / 255.f;
			m_pixels[i].v[3] = 1.f;
		}
		delete[] image;

		Utils::CreateSamplerState(m_samplerState, device);
		Utils::CreateShaderResourceView(m_width, m_height,
			m_texture2D, m_ShaderResourceView, device);
		Utils::UpdateConstantBuffer(m_pixels, m_texture2D, context);

	}
	void Image::Render(Microsoft::WRL::ComPtr <ID3D11DeviceContext>& context)
	{
		context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
		context->PSSetShaderResources(0, 1, m_ShaderResourceView.GetAddressOf());
	}

}
#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <omp.h>
#include <algorithm>
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

		float alpha = 1.3f;
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
	void Image::BoxBlur(Microsoft::WRL::ComPtr <ID3D11DeviceContext>& context)
	{
		std::vector<Vec4> BlurImage(m_width * m_height, Vec4{ 0.0f,0.0f,0.0f,1.0f });
		float alpha = 0.2f;

		for (int x = 0; x < m_width; x++)
		{
			for (int y = 0; y < m_height; y++)
			{
				int idx = y * m_width + x;
				int min_i = y * m_width;
				int max_i = (y + 1) * m_width - 1;
				for (int i = -2; i <= 2; i++)
				{
					int currIdx = std::clamp(idx + i, min_i, max_i);
					BlurImage[idx] += (m_pixels[currIdx] * alpha);
				}
			}
		}
		std::swap(m_pixels, BlurImage);
		std::fill(BlurImage.begin(), BlurImage.end(), Vec4{ 0.0f,0.0f,0.0f,1.0f });

		for (int x = 0; x < m_width; x++)
		{
			for (int y = 0; y < m_height; y++)
			{
				int idx = y * m_width + x;
				int min_i = x;
				int max_i = (m_height - 1) * m_width + x;
				for (int i = -2; i <= 2; i++)
				{
					int currIdx = std::clamp(idx + i * m_width, min_i, max_i);
					BlurImage[idx] += (m_pixels[currIdx] * alpha);
				}
			}
		}
		std::swap(m_pixels, BlurImage);
		Utils::UpdateConstantBuffer(m_pixels, m_texture2D, context);
	}
	void Image::BoxBlurOMP(Microsoft::WRL::ComPtr <ID3D11DeviceContext>& context)
	{
		std::vector<Vec4> BlurImage(m_width * m_height, Vec4{ 0.0f,0.0f,0.0f,1.0f });
		float alpha = 0.2f;
#pragma omp parallel for
		for (int x = 0; x < m_width; x++)
		{
			for (int y = 0; y < m_height; y++)
			{
				int idx = y * m_width + x;
				int min_i = y * m_width;
				int max_i = (y + 1) * m_width - 1;
				for (int i = -2; i <= 2; i++)
				{
					int currIdx = std::clamp(idx + i, min_i, max_i);
					BlurImage[idx] += (m_pixels[currIdx] * alpha);
				}
			}
		}
		std::swap(m_pixels, BlurImage);
		std::fill(BlurImage.begin(), BlurImage.end(), Vec4{ 0.0f,0.0f,0.0f,1.0f });
#pragma omp parallel for
		for (int x = 0; x < m_width; x++)
		{
			for (int y = 0; y < m_height; y++)
			{
				int idx = y * m_width + x;
				int min_i = x;
				int max_i = (m_height - 1) * m_width + x;
				for (int i = -2; i <= 2; i++)
				{
					int currIdx = std::clamp(idx + i * m_width, min_i, max_i);
					BlurImage[idx] += (m_pixels[currIdx] * alpha);
				}
			}
		}
		std::swap(m_pixels, BlurImage);
		Utils::UpdateConstantBuffer(m_pixels, m_texture2D, context);

	}
	void Image::Render(Microsoft::WRL::ComPtr <ID3D11DeviceContext>& context)
	{
		context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
		context->PSSetShaderResources(0, 1, m_ShaderResourceView.GetAddressOf());
	}

}
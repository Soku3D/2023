#pragma once 

#include <vector>
#include <d3d11.h>
#include <wrl.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <iostream>
#include <string>
#include <iterator>
#include <fstream>
namespace soku {
	class Utils {
	public:
		static void CheckResult(HRESULT& hr, ID3DBlob* error)
		{
			if (FAILED(hr))
			{
				if (error)
				{
					std::cout << (char*)error->GetBufferPointer();
				}
			}
		}
		template <typename T_VERTEX>
		static void CreateVertexBuffer(
			const std::vector<T_VERTEX>& vertices,
			Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer,
			Microsoft::WRL::ComPtr <ID3D11Device>& device)
		{
			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));
			bufferDesc.ByteWidth = UINT(sizeof(T_VERTEX) * vertices.size());
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.StructureByteStride = sizeof(T_VERTEX);

			D3D11_SUBRESOURCE_DATA subData;
			subData.pSysMem = vertices.data();
			subData.SysMemPitch = 0;
			subData.SysMemSlicePitch = 0;
			if (FAILED(device->CreateBuffer(&bufferDesc, &subData, vertexBuffer.GetAddressOf())))
			{
				std::cout << "Create VertexBuffer Failed";
			}
		}
		template <typename T_INDEX>
		static void CreateIndexBuffer(
			const std::vector<T_INDEX>& indices,
			Microsoft::WRL::ComPtr<ID3D11Buffer>& indexBuffer,
			Microsoft::WRL::ComPtr <ID3D11Device>& device)
		{
			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));
			bufferDesc.ByteWidth = UINT(sizeof(T_INDEX) * indices.size());
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.StructureByteStride = sizeof(T_INDEX);

			D3D11_SUBRESOURCE_DATA subData;
			subData.pSysMem = indices.data();
			subData.SysMemPitch = 0;
			subData.SysMemSlicePitch = 0;
			if (FAILED(device->CreateBuffer(&bufferDesc, &subData, indexBuffer.GetAddressOf())))
			{
				std::cout << "Create indexBuffer Failed";
			}
		}
		template <typename T_CONSTANT>
		static void CreateConstantBuffer(T_CONSTANT& constant,
			Microsoft::WRL::ComPtr<ID3D11Buffer>& constantBuffer,
			Microsoft::WRL::ComPtr <ID3D11Device>& device)
		{
			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));
			bufferDesc.ByteWidth = UINT(sizeof(T_CONSTANT));
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA subData;
			subData.pSysMem = &constant;
			subData.SysMemPitch = 0;
			subData.SysMemSlicePitch = 0;
			if (FAILED(device->CreateBuffer(&bufferDesc, &subData, constantBuffer.GetAddressOf())))
			{
				std::cout << "Create constantBuffer Failed";
			}
		}
		static std::vector<unsigned char> CreateShaderByCSO(const std::string filePath)
		{
			std::ifstream input(filePath, std::ios::binary);
			std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
			return buffer;
		}
		static void CreateVertexShaderAndInputLayout(
			const std::wstring& shaderFilePath,
			Microsoft::WRL::ComPtr <ID3D11VertexShader>& vertexShader,
			std::vector<D3D11_INPUT_ELEMENT_DESC>& elements,
			Microsoft::WRL::ComPtr <ID3D11InputLayout>& inputlayout,
			Microsoft::WRL::ComPtr <ID3D11Device>& device)
		{
#if defined(DEBUG) || (_DEBUG)
			Microsoft::WRL::ComPtr<ID3DBlob> shader;
			Microsoft::WRL::ComPtr<ID3DBlob> error;
			HRESULT hr = D3DCompileFromFile(shaderFilePath.c_str(),
				0, 0, "main", "vs_5_0", NULL, NULL,
				&shader, &error);
			CheckResult(hr, error.Get());
			device->CreateVertexShader(shader->GetBufferPointer(), shader->GetBufferSize(),
				NULL, &vertexShader);
			device->CreateInputLayout(elements.data(), UINT(elements.size()), shader->GetBufferPointer()
				, shader->GetBufferSize(), &inputlayout);
#endif
#ifndef _DEBUG
			std::vector<unsigned char> buffer = CreateShaderByCSO("VertexShader.cso");
			device->CreateVertexShader(buffer.data(), buffer.size(),
				NULL, &vertexShader);
			device->CreateInputLayout(elements.data(), UINT(elements.size()), buffer.data(), buffer.size(), \
				&inputlayout);
#endif	
		}
		static void CreatePixelShader(
			const std::wstring& shaderFilePath,
			Microsoft::WRL::ComPtr <ID3D11PixelShader>& pixelShader,
			Microsoft::WRL::ComPtr <ID3D11Device>& device)
		{
#if defined(DEBUG) || (_DEBUG)
			Microsoft::WRL::ComPtr<ID3DBlob> shader;
			Microsoft::WRL::ComPtr<ID3DBlob> error;
			HRESULT hr = D3DCompileFromFile(shaderFilePath.c_str(),
				0, 0, "main", "ps_5_0", NULL, NULL,
				&shader, &error);
			CheckResult(hr, error.Get());
			device->CreatePixelShader(shader->GetBufferPointer(), shader->GetBufferSize(),
				NULL, &pixelShader);
#endif
#ifndef DEBUG
			std::vector<unsigned char> buffer = CreateShaderByCSO("PixelShader.cso");
			device->CreatePixelShader(buffer.data(), buffer.size(),
				NULL, &pixelShader);
#endif	

		}
		template <typename T_CONST>
		static void UpdateConstantBuffer(std::vector<T_CONST>& constant,
			Microsoft::WRL::ComPtr<ID3D11Texture2D>& buffer,
			Microsoft::WRL::ComPtr <ID3D11DeviceContext>& context)
		{
			D3D11_MAPPED_SUBRESOURCE sub;
			context->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
			memcpy(sub.pData, constant.data(), sizeof(T_CONST) * constant.size());
			context->Unmap(buffer.Get(), 0);
		}
		
	};
}
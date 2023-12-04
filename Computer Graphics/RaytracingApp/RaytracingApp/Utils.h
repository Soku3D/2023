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
		static void CreateVertexShaderAndInputLayout(
			const std::wstring& shaderFilePath,
			Microsoft::WRL::ComPtr <ID3D11VertexShader>& vertexShader,
			std::vector<D3D11_INPUT_ELEMENT_DESC>& elements,
			Microsoft::WRL::ComPtr <ID3D11InputLayout>& inputlayout,
			Microsoft::WRL::ComPtr <ID3D11Device>& device)
		{
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
		}
		static void CreatePixelShader(
			const std::wstring& shaderFilePath,
			Microsoft::WRL::ComPtr <ID3D11PixelShader>& pixelShader,
			Microsoft::WRL::ComPtr <ID3D11Device>& device)
		{
			Microsoft::WRL::ComPtr<ID3DBlob> shader;
			Microsoft::WRL::ComPtr<ID3DBlob> error;
			HRESULT hr = D3DCompileFromFile(shaderFilePath.c_str(),
				0, 0, "main", "ps_5_0", NULL, NULL,
				&shader, &error);
			CheckResult(hr, error.Get());

			device->CreatePixelShader(shader->GetBufferPointer(), shader->GetBufferSize(),
				NULL, &pixelShader);
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
		static void CreateSamplerState(
			Microsoft::WRL::ComPtr<ID3D11SamplerState>& samplerState,
			Microsoft::WRL::ComPtr <ID3D11Device>& device,
			D3D11_TEXTURE_ADDRESS_MODE mode = D3D11_TEXTURE_ADDRESS_CLAMP,
			D3D11_FILTER filter = D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT)
		{
			D3D11_SAMPLER_DESC samplerDesc;
			ZeroMemory(&samplerDesc, sizeof(samplerDesc));
			samplerDesc.AddressU = mode;
			samplerDesc.AddressV = mode;
			samplerDesc.AddressW = mode;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			samplerDesc.MinLOD = 0;
			samplerDesc.Filter = D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

			if (FAILED(device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf())))
			{
				std::cout << "CreateSamplerState Failed" << std::endl;
			}
		}
		static void CreateShaderResourceView(
			UINT width,
			UINT height,
			Microsoft::WRL::ComPtr <ID3D11Texture2D>& texture,
			Microsoft::WRL::ComPtr <ID3D11ShaderResourceView>& shaderResourceView,
			Microsoft::WRL::ComPtr <ID3D11Device>& device
			)
		{
			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(textureDesc));
			textureDesc.Width = width;
			textureDesc.Height = height,
			textureDesc.ArraySize = textureDesc.MipLevels = 1;
			textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			textureDesc.Usage = D3D11_USAGE_DYNAMIC;
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			textureDesc.SampleDesc.Count = 1;


			if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, texture.GetAddressOf())))
			{
				std::cout << "CreateShaderResourceView() Failed" << std::endl;
			}

			// Create ShaderResourceView
			device->CreateShaderResourceView(texture.Get(), nullptr, shaderResourceView.GetAddressOf());
		}
		
	};
}
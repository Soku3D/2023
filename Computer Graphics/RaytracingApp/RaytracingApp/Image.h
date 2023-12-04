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
		std::vector<struct Vec4> m_pixels;
		std::vector<struct Vec4> m_blur;
		std::string filePath;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;
			
	};
}
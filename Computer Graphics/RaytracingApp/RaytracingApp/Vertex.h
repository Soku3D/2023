namespace soku {
	struct Vec4
	{
		float v[4];
		Vec4 operator*(const float& alpha)
		{
			Vec4 vec;
			for (size_t i = 0; i < 3; i++)
			{
				vec.v[i] = this->v[i] * alpha;
			}
			return vec;

		}
		Vec4 operator+=(const Vec4& v1)
		{
			for (size_t i = 0; i < 3; i++)
			{
				this->v[i] += v1.v[i];
			}
			return *this;
		}
		Vec4 operator+(const Vec4& vec1)
		{
			Vec4 vec;
			for (size_t i = 0; i < 3; i++)
			{
				vec.v[i] = this->v[i] + vec1.v[i]; 
			}
			return vec;
		}
	};
	
	
	struct Vec2
	{
		float v[2];
	};

	struct Vertex
	{
		Vec4 pos;
		Vec2 uv;
	};
}

#pragma once
#include "directxtk/SimpleMath.h"
namespace soku {
	using DirectX::SimpleMath::Vector3;
	class Camera {
	public:
		Camera()
			:m_pos(Vector3(0.0f,0.0f,-1.0f))
		{
		}
		Camera(Vector3 pos):
			m_pos(pos)
		{
		}
		DirectX::SimpleMath::Vector3 m_pos;
	};
}
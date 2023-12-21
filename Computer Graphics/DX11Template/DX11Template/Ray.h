#pragma once
#include "directxtk/SimpleMath.h"
namespace soku {
	using DirectX::SimpleMath::Vector2;
	using DirectX::SimpleMath::Vector3;
	using DirectX::SimpleMath::Vector4;
	class Ray {
	public:
		Ray(Vector3 start, Vector3 dir);
		Vector3 start;
		Vector3 dir;
	};
}
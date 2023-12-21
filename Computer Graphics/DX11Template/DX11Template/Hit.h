#pragma once
#include "directxtk/SimpleMath.h"
#include <memory>
namespace soku {
	class Hit{
	public:
		Hit():
			distance(-1.f),
			hitModel(nullptr)
		{}
		DirectX::SimpleMath::Vector3 hitPos;
		DirectX::SimpleMath::Vector3 hitNormal;
		float distance;

		class Model* hitModel;
	};
}
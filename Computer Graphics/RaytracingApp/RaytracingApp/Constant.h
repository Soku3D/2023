namespace soku {

	struct Constant {
		DirectX::SimpleMath::Matrix model;
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix projection;
		float deltaTime;
		DirectX::SimpleMath::Vector3 dumy;
	};
}
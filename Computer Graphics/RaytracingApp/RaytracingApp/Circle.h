namespace soku {
	class Circle {
	public:
		Circle();
		Circle(float radius, Vector2 center);
		bool IsInCircle(Vector2& point);
		bool IsInCircleTheta(Vector2& point);
		float radius;
		Vector2 center;
		float m_theta;
		float m_r;
	};
}
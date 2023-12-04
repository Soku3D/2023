#include "pch.h"

namespace soku {
	Circle::Circle()
	:radius(50.0f),
	center(Vector2(0,0))
	{
	}
	Circle::Circle(float radius, Vector2 center)
	:radius(radius),
	center(center)
	{
		m_r = center.Length();
		m_theta = 0.0f;
	}
	bool Circle::IsInCircle(Vector2& point)
	{
		Vector2 centerToPoint = point - center;
		float distance = centerToPoint.Length();
		return distance < radius;
	}
	bool Circle::IsInCircleTheta(Vector2& point)
	{
		float r = center.Length();
		float x = r * std::cos(m_theta);
		float y = r * std::sin(m_theta);
		center = Vector2(x,y);
		Vector2 centerToPoint = point - center;
		float distance = centerToPoint.Length();
		return distance < radius;
	}
}
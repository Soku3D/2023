#include "Geometry.h"

namespace soku {
	Hit Circle::CheckCollision(const Ray& in_ray)
	{
		return Hit();

	}
	Hit Sphere::CheckCollision(const Ray& in_ray)
	{
		return Hit();
	}
	Hit Triangle::CheckCollision(const Ray& in_ray)
	{
		Hit hit;
		Vector3 s = in_ray.start;
		Vector3 d = in_ray.dir;
		float t = GetDistance(in_ray);
		Vector3 p = s + d * t;

		Vector3 p01 = p1 - p;
		Vector3 p02 = p2 - p;
		Vector3 p03 = p3 - p;

		w3 = p01.Cross(p02).Length() * 0.5f;
		w1 = p02.Cross(p03).Length() * 0.5f;
		w2 = p03.Cross(p01).Length() * 0.5f;

		if (std::abs((w1+w2+w3)-area) < 0.001f)
		{
			w1 /= area;
			w2 /= area;
			w3 /= area;
			m_color = w1 * c1 + w2 * c2 + w3 * c3;
			hit.hitModel = this;
			hit.distance = t;
			hit.hitPos = p;
			hit.hitNormal = areaVector;
		}
		return hit;
	}
	float Triangle::GetDistance(const Ray& in_ray)
	{
		Vector3 s = in_ray.start;
		Vector3 d = in_ray.dir;

		float t = (p1 - s).Dot(areaVector) / d.Dot(areaVector);
		return t;
	}
	
	Rectangle::Rectangle(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3)
	{
		t0 = Triangle(p0, p1, p2);
		t1 = Triangle(p0, p2, p3);
	}
	Hit Rectangle::CheckCollision(const Ray& in_ray)
	{
		Hit h0 = t0.CheckCollision(in_ray);
		Hit h1 = t1.CheckCollision(in_ray);
		return h0.distance > h1.distance ? h0 : h1;
	}

}
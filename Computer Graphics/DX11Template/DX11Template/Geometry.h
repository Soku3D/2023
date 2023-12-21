#pragma once
#include "Ray.h"
#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include "Hit.h"
namespace soku{
	class Model {
	public:
		Model(Vector4 color = Vector4(0.8f,0.2f,0.2f,1.0f)) :
			m_color(color) 
		{}
		virtual Hit CheckCollision(const Ray& in_ray) = 0;
		void SetColor(const Vector4& inColor) { m_color = inColor; }
		Vector4 GetColor()const { return m_color; }
	protected:
		Vector4 m_color;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shader;
	};
	class Sphere: public Model {
	public:
		Sphere();
		Sphere(float radius, Vector3 center):
			m_radius(radius),
			m_center(center)
		{}
		virtual Hit CheckCollision(const Ray& in_ray) override;
	private:
		float m_radius;
		Vector3 m_center;
	};
	class Circle :public Model {
	public:
		Circle();
		Circle(float radius, Vector3 center);
		virtual Hit CheckCollision(const Ray& in_ray) override;
	private:
		float m_radius;
		Vector3	m_center;
	};
	class Triangle : public Model {
	public:
		Triangle() {}
		Triangle(Vector3 p1, Vector3 p2, Vector3 p3)
			:p1(p1),p2(p2),p3(p3),
			Model(),
			w1(0.f),
			w2(0.f),
			w3(0.f)
		{
			Vector3 p21 = p1 - p2;
			Vector3 p23 = p3 - p2;
			c1 = Vector4(1.0f, 0.0f, 0.f, 1.f);
			c2 = Vector4(0.0f, 1.0f, 0.f, 1.f);
			c3 = Vector4(0.0f, 0.0f, 1.f, 1.f);

			areaVector = p23.Cross(p21);
			area = areaVector.Length() *0.5f;
			areaVector.Normalize();
		}
		virtual Hit CheckCollision(const Ray& in_ray) override;
		float GetDistance(const Ray& in_ray);
	private:
		Vector3 p1, p2, p3;
		Vector4 c1, c2, c3;
		float w1, w2, w3;
		float area;
		Vector3 areaVector;
	};
	class Rectangle : public Model {
	public:
		Rectangle() {}
		Rectangle(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3);
	private:
		Triangle t0, t1;
		virtual Hit CheckCollision(const Ray& in_ray) override;
		float d0,d1;
	};
}
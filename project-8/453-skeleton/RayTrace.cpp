#include <iostream>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RayTrace.h"

using namespace std;
using namespace glm;

Sphere::Sphere(vec3 c, float r, int ID)
{
	centre = c;
	radius = r;
	id = ID;
}

//------------------------------------------------------------------------------
// This is part 2.1 of your assignment. At the moment, the spheres are not showing
// up. Implement this method to make them show up.
//
// Make sure you set all of the appropriate fields in the Intersection object.
//------------------------------------------------------------------------------
Intersection Sphere::getIntersection(Ray ray)
{
	Intersection i{};
	i.id = id;
	i.material = material;

	// You are required to implement this intersection.
	//
	// NOTE: You _must_ set these values appropriately for each case:
	//
	// No Intersection:
	// i.numberOfIntersections = 0;
	//
	// Intersection:
	// i.normal = **the normal at the point of intersection **
	// i.point = **the point of intersection**
	// i.numberOfIntersections = 1; // for a single intersection
	//
	// If you get fancy and implement things like refraction, you may actually
	// want to track more than one intersection. You'll need to change
	// The intersection struct in that case.
	vec3 oc = ray.origin - centre;
	double a = dot(ray.direction, ray.direction);
	double b = 2.0 * dot(oc, ray.direction);
	double c = dot(oc, oc) - radius * radius;
	float disc = b * b - 4 * a * c;

	if (disc < 0)
	{
		i.numberOfIntersections = 0;
	}
	else if (disc > 0)
	{
		float r = (-b + sqrt(disc)) / (2 * a);
		float l = -b / a - r;

		if (abs(l) < abs(r))
		{
			i.point = ray.origin + l * ray.direction;
		}
		else
		{
			i.point = ray.origin + r * ray.direction;
		}
		i.numberOfIntersections = 2;
	}
	else
	{
		float l = (-0.5 * b / a);

		i.point = ray.origin + l * ray.direction;
		i.numberOfIntersections = 1;
	}
	if (i.point != vec3(0, 0, 0))
		i.normal = normalize(i.point - centre);
	return i;
}

Cylinder::Cylinder(vec3 c, float r, int ID)
{
	center = c;
	radius = r;
	id = ID;
}

//------------------------------------------------------------------------------
// This is part 2.2 of your assignment. At the moment, the cylinders are not showing
// up. Implement this method to make them show up.
//
// Make sure you set all of the appropriate fields in the Intersection object.
//------------------------------------------------------------------------------
Intersection Cylinder::getIntersection(Ray ray)
{
	Intersection i{};
	i.id = id;
	i.material = material;

	// You are required to implement this intersection.
	//
	// NOTE: You _must_ set these values appropriately for each case:
	//
	// No Intersection:
	// i.numberOfIntersections = 0;
	//
	// Intersection:
	// i.normal = **the normal at the point of intersection **
	// i.point = **the point of intersection**
	// i.numberOfIntersections = 1; // for a single intersection
	//
	// If you get fancy and implement things like refraction, you may actually
	// want to track more than one intersection. You'll need to change
	// The intersection struct in that case.
	float a = (ray.direction.x * ray.direction.x) + (ray.direction.z * ray.direction.z);
	float b = 2 * (ray.direction.x * (ray.origin.x - center.x) + ray.direction.z * (ray.origin.z - center.z));
	float c = (ray.origin.x - center.x) * (ray.origin.x - center.x) + (ray.origin.z - center.z) * (ray.origin.z - center.z) - (radius * radius);

	float delta = b * b - 4 * (a * c);
	if (delta < 0)
		return i;

	float t1 = (-b - sqrt(delta)) / (2 * a);
	float t2 = (-b + sqrt(delta)) / (2 * a);
	float t;

	if (t1 > t2)
		t = t2;
	else
		t = t1;

	float r = ray.origin.y + t * ray.direction.y;

	if ((r >= center.y) and (r <= center.y + height))
	{
		i.point = ray.origin + ray.direction * t;
	}

	{
		auto a = center - height / 2;
		auto b = center + height / 2;
		auto ba = b - a;
		auto pa = i.point - a;
		auto baba = dot(ba, ba);
		auto paba = dot(pa, ba);
		auto h2 = paba / baba;
		i.normal = (pa - ba * h2) / radius;
	}
	return i;
}

Plane::Plane(vec3 p, vec3 n, int ID)
{
	point = p;
	normal = n;
	id = ID;
}

float dot_normalized(vec3 v1, vec3 v2)
{
	return glm::dot(glm::normalize(v1), glm::normalize(v2));
}

void debug(char *str, vec3 a)
{
	cout << "debug:" << str << ": " << a.x << ", " << a.y << ", " << a.z << endl;
}
// --------------------------------------------------------------------------
void Triangles::initTriangles(int num, vec3 *t, int ID)
{
	id = ID;
	for (int i = 0; i < num; i++)
	{
		triangles.push_back(Triangle(*t, *(t + 1), *(t + 2)));
		t += 3;
	}
}

Intersection Triangles::intersectTriangle(Ray ray, Triangle triangle)
{
	// From https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
	const float EPSILON = 0.0000001;
	auto vertex0 = triangle.p1;
	auto vertex1 = triangle.p2;
	auto vertex2 = triangle.p3;

	glm::vec3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = glm::cross(ray.direction, edge2);
	a = glm::dot(edge1, h);
	if (a > -EPSILON && a < EPSILON)
	{
		return Intersection{}; // no intersection
	}
	f = 1.0 / a;
	s = ray.origin - vertex0;
	u = f * glm::dot(s, h);
	if (u < 0.0 || u > 1.0)
	{
		return Intersection{}; // no intersection
	}
	q = glm::cross(s, edge1);
	v = f * glm::dot(ray.direction, q);
	if (v < 0.0 || u + v > 1.0)
	{
		return Intersection{}; // no intersection
	}
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * glm::dot(edge2, q);
	// ray intersection
	if (t > EPSILON)
	{
		Intersection p;
		p.point = ray.origin + ray.direction * t;
		p.normal = glm::normalize(glm::cross(edge1, edge2));
		p.material = material;
		p.numberOfIntersections = 1;
		p.id = id;
		return p;
	}
	else
	{
		// This means that there is a line intersection but not a ray intersection.
		return Intersection{}; // no intersection
	}
}

Intersection Triangles::getIntersection(Ray ray)
{
	Intersection result{};
	result.material = material;
	result.id = id;
	float min = 9999;
	result = intersectTriangle(ray, triangles.at(0));
	if (result.numberOfIntersections != 0)
		min = glm::distance(result.point, ray.origin);
	for (unsigned int i = 1; i < triangles.size(); i++)
	{
		Intersection p = intersectTriangle(ray, triangles.at(i));
		if (p.numberOfIntersections != 0 && glm::distance(p.point, ray.origin) < min)
		{
			min = glm::distance(p.point, ray.origin);
			result = p;
		}
	}

	result.material = material;
	result.id = id;
	return result;
}

Intersection Plane::getIntersection(Ray ray)
{
	Intersection result;
	result.material = material;
	result.id = id;
	result.normal = normal;
	if (dot(normal, ray.direction) >= 0)
		return result;
	float s = dot(point - ray.origin, normal) / dot(ray.direction, normal);
	// if(s<0.00001)return result;
	result.numberOfIntersections = 1;
	result.point = ray.origin + s * ray.direction;
	return result;
}

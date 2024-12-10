#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/vector_query.hpp>

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "imagebuffer.h"
#include "RayTrace.h"
#include "Scene.h"
#include "Lighting.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int hasIntersection(Scene const &scene, Ray ray, int skipID)
{
	for (auto &shape : scene.shapesInScene)
	{
		Intersection tmp = shape->getIntersection(ray);
		if (
				shape->id != skipID && tmp.numberOfIntersections != 0 && glm::distance(tmp.point, ray.origin) > 0.00001 && glm::distance(tmp.point, ray.origin) < glm::distance(ray.origin, scene.lightPosition) - 0.01)
		{
			return tmp.id;
		}
	}
	return -1;
}

Intersection getClosestIntersection(Scene const &scene, Ray ray, int skipID)
{ // get the nearest
	Intersection closestIntersection;
	float min = std::numeric_limits<float>::max();
	for (auto &shape : scene.shapesInScene)
	{
		if (skipID == shape->id)
		{
			// Sometimes you need to skip certain shapes. Useful to
			// avoid self-intersection. ;)
			continue;
		}
		Intersection p = shape->getIntersection(ray);
		float distance = glm::distance(p.point, ray.origin);
		if (p.numberOfIntersections != 0 && distance < min)
		{
			min = distance;
			closestIntersection = p;
		}
	}
	return closestIntersection;
}

glm::vec3 raytraceSingleRay(Scene const &scene, Ray const &start_ray, int level)
{
	// TODO: Part 3: Somewhere in this function you will need to add the code to determine
	//               if a given point is in shadow or not. Think carefully about what parts
	//               of the lighting equation should be used when a point is in shadow.
	// TODO: Part 4: Somewhere in this function you will need to add the code that does reflections and refractions.
	//               NOTE: The ObjectMaterial class already has a parameter to store the object's
	//               reflective properties. Use this parameter + the color coming back from the
	//               reflected array and the color from the phong shading equation.
	Ray ray = start_ray;
	PhongReflection phong;
	auto result = getClosestIntersection(scene, ray, -1);
	vec3 color(1, 1, 1); // find intersection
	phong.ray = ray;
	phong.scene = scene;
	phong.material = result.material;
	phong.intersection = result;

	if (!result.numberOfIntersections)
		return vec3(0, 0, 0); // black;

	color *= phong.I();

	auto shadowRay = Ray{phong.p(), phong.l()};
	auto shadowIntersection = getClosestIntersection(scene, shadowRay, result.id);
	auto lightLength = length(result.point - scene.lightPosition);
	auto shadowLength = length(result.point - shadowIntersection.point);
	if (
			shadowIntersection.numberOfIntersections > 0 &&
			shadowLength < lightLength)
		return vec3(0, 0, 0);

	for (int i = 1; i < level; i++)
	{
		auto prev_material = result.material;
		ray = Ray(result.point, reflect(ray.direction, result.normal));
		result = getClosestIntersection(scene, ray, result.id);
		phong.ray = ray;
		phong.intersection = result;
		phong.material = result.material;
		if (!result.numberOfIntersections || prev_material.reflectionStrength == vec3(0))
			break;
		color += phong.I() * prev_material.reflectionStrength;
	}

	// Cannot implement refraction without transparency.
	
	// for (int i = 0; i < level; i++)
	// {
	// 	auto ratio = result.material.refractiveIndex;
	// 	auto cosI = dot(ray.direction,result.normal);
	// 	auto sin2T = ratio * ratio * (1 - cosI *cosI);
	// 	if(sin2T > 1){
	// 		break;
	// 	}
	// 	// TODO:check if not transparent.
	// 	if (result.material.refractiveIndex == 1)
	// 		break;
	// 	ray = Ray(result.point, refract(ray.direction, result.normal,ratio));
	// 	result = getClosestIntersection(scene, ray, result.id);
	// }
	return color;
}

struct RayAndPixel
{
	Ray ray;
	int x;
	int y;
};

std::vector<RayAndPixel> getRaysForViewpoint(Scene const &scene, ImageBuffer &image, glm::vec3 viewPoint)
{
	// This is the function you must implement for part 1
	//
	// This function is responsible for creating the rays that go
	// from the viewpoint out into the scene with the appropriate direction
	// and angles to produce a perspective image.
	int x = 0;
	int y = 0;
	std::vector<RayAndPixel> rays;

	vec3 focal_point(0, 0, 2);
	// TODO: Part 1: Currently this is casting rays in an orthographic style.
	//               You need to change this code to project them in a pinhole camera style.
	for (float i = -1; x < image.Width(); x++)
	{
		y = 0;
		for (float j = -1; y < image.Height(); y++)
		{
			glm::vec3 viewPointOrthographic(i - viewPoint.x, j - viewPoint.y, 0);
			glm::vec3 direction = normalize(viewPointOrthographic - focal_point);
			Ray r = Ray(vec3{}, direction);
			rays.push_back({r, x, y});
			j += 2.f / image.Height();
		}
		i += 2.f / image.Width();
	}
	return rays;
}

void raytraceImage(Scene const &scene, ImageBuffer &image, glm::vec3 viewPoint)
{
	// Reset the image to the current size of the screen.
	image.Initialize();

	// Get the set of rays to cast for this given image / viewpoint
	std::vector<RayAndPixel> rays = getRaysForViewpoint(scene, image, viewPoint);

	// This loops processes each ray and stores the resulting pixel in the image.
	// final color into the image at the appropriate location.
	//
	// I've written it this way, because if you're keen on this, you can
	// try and parallelize this loop to ensure that your ray tracer makes use
	// of all of your CPU cores
	//
	// Note, if you do this, you will need to be careful about how you render
	// things below too
	// std::for_each(std::begin(rays), std::end(rays), [&] (auto const &r) {
	for (auto const &r : rays)
	{
		glm::vec3 color = raytraceSingleRay(scene, r.ray, 3);
		image.SetPixel(r.x, r.y, color);
	}
}

// EXAMPLE CALLBACKS
class Assignment5 : public CallbackInterface
{

public:
	Assignment5()
	{
		viewPoint = glm::vec3(0, 0, 0);
		scene = initScene1();
		raytraceImage(scene, outputImage, viewPoint);
	}

	virtual void keyCallback(int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		{
			shouldQuit = true;
		}

		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		{
			scene = initScene1();
			raytraceImage(scene, outputImage, viewPoint);
		}

		if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		{
			scene = initScene2();
			raytraceImage(scene, outputImage, viewPoint);
		}
	}

	bool shouldQuit = false;

	ImageBuffer outputImage;
	Scene scene;
	glm::vec3 viewPoint;
};
// END EXAMPLES

int main()
{
	Log::debug("Starting main");

	// WINDOW
	glfwInit();

	// Change your image/screensize here.
	int width = 800;
	int height = 800;
	Window window(width, height, "CPSC 453");

	GLDebug::enable();

	// CALLBACKS
	std::shared_ptr<Assignment5> a5 = std::make_shared<Assignment5>(); // can also update callbacks to new ones
	window.setCallbacks(a5);																					 // can also update callbacks to new ones

	// RENDER LOOP
	while (!window.shouldClose() && !a5->shouldQuit)
	{
		glfwPollEvents();

		glEnable(GL_FRAMEBUFFER_SRGB);
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		a5->outputImage.Render();

		window.swapBuffers();
	}

	// Save image to file:
	// outpuImage.SaveToFile("foo.png")

	glfwTerminate();
	return 0;
}

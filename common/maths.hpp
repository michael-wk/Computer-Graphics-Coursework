#pragma once

#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

class Maths
{
public:
	//Transformation matrices
	static glm::mat4 translate(const glm::vec3& v);;

	static glm::mat4 scale(const glm::vec3& s);

	static float radians(float angle);
	static glm::mat4 rotate(const float& angle, glm::vec3 v);


	// view and projection matrices
	static glm::mat4 view(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& worldUp);
	static glm::mat4 perspective(float fov, float aspect, float near, float far);


	//glm vertices functions
	static float length(const glm::vec3& v);
	static float length(const glm::vec2& v);
	static float dot(const glm::vec3& a, const glm::vec3& b);
	static glm::vec3 cross(const glm::vec3& a, const glm::vec3& b);

};
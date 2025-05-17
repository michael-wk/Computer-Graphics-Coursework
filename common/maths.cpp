#include <common/maths.hpp>
#include <glm/gtc/matrix_transform.hpp>


// Returns the translation matrix for given vector
glm::mat4 Maths::translate(const glm::vec3& v)
{
    glm::mat4 translate(1.0f);
    translate[3][0] = v.x, translate[3][1] = v.y, translate[3][2] = v.z;
    return translate;
}


// returns the scaling matrix for given vector v
glm::mat4 Maths::scale(const glm::vec3& v)
{
    glm::mat4 scale(1.0f);
    scale[0][0] = v.x; scale[1][1] = v.y; scale[2][2] = v.z;
    return scale;
}


float Maths::radians(float angle)
{
    return angle * 3.1416f / 180.0f;
}

glm::mat4 Maths::rotate(const float& angle, glm::vec3 v)
{
    v = glm::normalize(v);
    float c = cos(angle);
    float s = sin(angle);
    float x2 = v.x * v.x, y2 = v.y * v.y, z2 = v.z * v.z;
    float xy = v.x * v.y, xz = v.x * v.z, yz = v.y * v.z;
    float xs = v.x * s, ys = v.y * s, zs = v.z * s;

    glm::mat4 rotate;
    rotate[0][0] = (1 - c) * x2 + c;
    rotate[0][1] = (1 - c) * xy + zs;
    rotate[0][2] = (1 - c) * xz - ys;
    rotate[1][0] = (1 - c) * xy - zs;
    rotate[1][1] = (1 - c) * y2 + c;
    rotate[1][2] = (1 - c) * yz + xs;
    rotate[2][0] = (1 - c) * xz + ys;
    rotate[2][1] = (1 - c) * yz - xs;
    rotate[2][2] = (1 - c) * z2 + c;

    return rotate;
}

// view matrix
glm::mat4 Maths::view(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& worldUp)
{
    // z axis
    glm::vec3 forward = glm::normalize(eye - target);
    // x axis
    glm::vec3 right = glm::normalize(glm::cross(worldUp, forward)); 
    // y axis
    glm::vec3 vertical = glm::cross(forward, right);

    glm::mat4 view(1.0f); // identity matrix

    view[0][0] = right.x;
    view[1][0] = right.y;
    view[2][0] = right.z;
    view[3][0] = -glm::dot(right, eye);

    view[0][1] = vertical.x;
    view[1][1] = vertical.y;
    view[2][1] = vertical.z;
    view[3][1] = -glm::dot(vertical, eye);

    view[0][2] = forward.x;
    view[1][2] = forward.y;
    view[2][2] = forward.z;
    view[3][2] = -glm::dot(forward, eye);
    return view;
}


// prospective matrix
glm::mat4 Maths::perspective(float fov, float aspect, float near, float far)
{
    float tanFOV = tan(fov / 2.0f);
    glm::mat4 res(0.0f);

    res[0][0] = 1.0f / (aspect * tanFOV);
    res[1][1] = 1.0f / (tanFOV);
    res[2][2] = -(far + near) / (far - near);
    res[2][3] = -1.0f;
    res[3][2] = -(2.0f * far * near) / (far - near);
    return res;
}



// vector functions
float Maths::length(const glm::vec3& v) // magnitude 
{
    return glm::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float Maths::length(const glm::vec2& v) // magnitude 
{
    return glm::sqrt(v.x * v.x + v.y * v.y);
}


float Maths::dot(const glm::vec3& a, const glm::vec3& b) // dot product
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

glm::vec3 Maths::cross(const glm::vec3& a, const glm::vec3& b) // cross product
{
	return glm::vec3(
        a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

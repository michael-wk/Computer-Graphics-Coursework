









#include <common/camera.hpp>
#include <common/maths.hpp>

Camera::Camera(const glm::vec3 Eye, const glm::vec3 Target)
{
    eye = Eye;
    target = Target;
}

void Camera::calculateMatrices()
{
    // Calculate the view matrix
    //view = glm::lookAt(eye, eye + front, worldUp);

	view = Maths::view(eye, eye + front, worldUp);


    // Calculate the projection matrix
    //projection = glm::perspective(fov, aspect, near, far);
	projection = Maths::perspective(fov, aspect, near, far);

    //
}

void Camera::calculateCameraVectors()
{
    front = glm::vec3(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch));
    right = glm::normalize(Maths::cross(front, worldUp));
    up = Maths::cross(right, front);
}
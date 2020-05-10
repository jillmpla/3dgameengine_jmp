#include "Camera.hpp"
#include "glm/gtc/matrix_transform.hpp" 

Camera::Camera(glm::vec3 eye1, glm::vec3 lookAt1, glm::vec3 up1, float fovDegrees1, float nearPlane1, float farPlane1, int bufferWidth1, int bufferHeight1) {
	eye = eye1;
	lookAt = lookAt1;
	up = up1;
	fov = glm::radians(fovDegrees1);
	nearPlane = nearPlane1;
	farPlane = farPlane1;
	bufferWidth = float(bufferWidth1);
	bufferHeight = float(bufferHeight1);
}

void Camera::updateBufferSize(int width1, int height1) {
	bufferWidth = width1;
	bufferHeight = height1;
}

glm::mat4 Camera::getViewMatrix() {
	glm::mat4 cameraMat = glm::lookAt(eye, lookAt, up);
	return cameraMat;
}

glm::mat4 Camera::getProjectionMatrix() {
	float asp = bufferWidth / bufferHeight;
	glm::mat4 projMat = glm::perspective(fov, asp, nearPlane, farPlane);
	return projMat;
}

void Camera::rotateRight(float degrees) {
	float deg = glm::radians(degrees);

	glm::vec4 v = glm::vec4(lookAt, 1.0f);
	glm::mat4 Tn = glm::translate(glm::mat4(1.0f), -eye);
	glm::mat4 R = glm::rotate(glm::mat4(1.0f), deg, up);
	glm::mat4 Tc = glm::translate(glm::mat4(1.0f), eye);
	glm::mat4 T = Tc*R*Tn;
	glm::vec4 other = T * v;
	lookAt = glm::vec3(other);
}

void Camera::rotateUp(float degrees) {
	float deg = glm::radians(degrees);

	glm::vec3 forward = glm::normalize(lookAt - eye);
	glm::vec3 localX = glm::normalize(glm::cross(forward, up));

	glm::vec4 v = glm::vec4(lookAt, 1.0f);
	glm::mat4 Tn = glm::translate(glm::mat4(1.0f), -eye);
	glm::mat4 R = glm::rotate(glm::mat4(1.0f), deg, localX);
	glm::mat4 Tc = glm::translate(glm::mat4(1.0f), eye);
	glm::mat4 T = Tc*R*Tn;
	glm::vec4 other = T * v;
	lookAt = glm::vec3(other);
}

void Camera::forward(float distance) {
	glm::vec3 forward = glm::normalize(lookAt - eye);
	glm::vec3 forwardAfter = forward * distance;

	glm::vec4 v = glm::vec4(lookAt, 1.0f);
	glm::vec4 q = glm::vec4(eye, 1.0f);

	glm::mat4 Tn = glm::translate(glm::mat4(1.0f), forwardAfter);
	glm::vec4 other = Tn * v;
	glm::vec4 other1 = Tn * q;

	lookAt = glm::vec3(other);
	eye = glm::vec3(other1);
}

void Camera::strafeRight(float distance) {
	glm::vec3 forward = glm::normalize(lookAt - eye);
	glm::vec3 localX = glm::normalize(glm::cross(forward, up));
	glm::vec3 forwardAfter = localX * distance;

	glm::vec4 v = glm::vec4(lookAt, 1.0f);
	glm::vec4 q = glm::vec4(eye, 1.0f);

	glm::mat4 Tn = glm::translate(glm::mat4(1.0f), forwardAfter);
	glm::vec4 other = Tn * v;
	glm::vec4 other1 = Tn * q;

	lookAt = glm::vec3(other);
	eye = glm::vec3(other1);
}

float Camera::getNearPlane() {
	return nearPlane;
}

glm::vec3 Camera::getEye() {
	return eye;
}

#include "camera.h"
#include "block.h"
#include "bx/math.h"
#include <SDL2/SDL.h>

Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, -12.0f);
    yaw = -90.0f;
    pitch = 0.0f;
    scale = 0.01f;
    width = 640;
    height = 480;
    worldup = glm::vec3(0.0f, 1.0f, 0.0f);
    this->front = glm::vec3(0.0f, 0.0f, 1.0f);
    this->movement_speed = 0.1f;
    this->mouse_sensitivity = 0.1f;
}

void Camera::view()
{

    float cam_rotation[16];
    bx::Vec3 Position = {position.x, position.y, position.z};
    glm::vec3 tmp = position + front;
    bx::Vec3 Tmp = {tmp.x, tmp.y, tmp.z};
    bx::Vec3 Up = {up.x, up.y, up.z};

    bx::mtxLookAt(cam_rotation, Position, Tmp, Up);

    float cam_translation[16];
    bx::mtxTranslate(cam_translation, position.x, position.y, position.z);

    float cam_transform[16];
    bx::mtxMul(cam_transform, cam_translation, cam_rotation);

    float view[16];
    bx::mtxInverse(view, cam_transform);

    float proj[16];
    bx::mtxProj(
        proj, 60.0f, width / height, 0.1f, 100.0f,
        bgfx::getCaps()->homogeneousDepth);

    bgfx::setViewTransform(0, cam_rotation, proj);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = movement_speed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position += right * velocity;
    if (direction == RIGHT)
        position -= right * velocity;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::update_camera_vectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(
        this->front, worldup)); // normalize the vectors, because their length
                                // gets closer to 0 the more you look up or down
                                // which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}

void Camera::process_mouse_movement(float xoffset, float yoffset)
{
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw -= xoffset;
    pitch += yoffset;
    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // update Front, Right and Up Vectors using the updated Euler angles
    update_camera_vectors();
}
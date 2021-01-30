#include "camera.h"
#include "bgfx/bgfx.h"
#include "bx/math.h"
#include <SDL2/SDL.h>

Camera::Camera()
{
    x = 0.0f;
    y = 0.0f;
    z = -12.0f;
    yaw = 0.0f;
    pitch = 0.0f;
    scale = 0.01f;
    prev_mouse_x = 0;
    prev_mouse_y = 0;
    width = 640;
    height = 480;
}

void Camera::view()
{
    // simple input code for orbit camera
    int mouse_x, mouse_y;
    const int buttons = SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
    if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0) {
        int delta_x = mouse_x - prev_mouse_x;
        int delta_y = mouse_y - prev_mouse_y;
        yaw += float(-delta_x) * scale;
        pitch += float(-delta_y) * scale;
    }

    prev_mouse_x = mouse_x;
    prev_mouse_y = mouse_y;

    float cam_rotation[16];
    bx::mtxRotateXYZ(cam_rotation, pitch, yaw, 0.0f);

    float cam_translation[16];
    bx::mtxTranslate(cam_translation, x, y, z);

    float cam_transform[16];
    bx::mtxMul(cam_transform, cam_translation, cam_rotation);

    float view[16];
    bx::mtxInverse(view, cam_transform);

    float proj[16];
    bx::mtxProj(
        proj, 60.0f, width / height, 0.1f, 100.0f,
        bgfx::getCaps()->homogeneousDepth);

    bgfx::setViewTransform(0, view, proj);
}
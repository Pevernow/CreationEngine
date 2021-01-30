#ifndef _CAMERA_H_
#define _CAMERA_H_
class Camera
{
public:
    Camera();
    void view();
    float x, y, z;
    float yaw, pitch, scale;
    int prev_mouse_x, prev_mouse_y;
    float width, height;
};
#endif
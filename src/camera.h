#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    JUMP
};

class Camera
{
public:
    Camera();
    void view();
    void processKeyboard(
        Camera_Movement direction,
        float deltaTime); // dir:0-GO,1-BACK,2-LEFT,3-RIGHT，4-UP
    void update_camera_vectors();
    void process_mouse_movement(float xoffset, float yoffset);
    void update_camera_position(float deltaTime);
    void on_left_click();
    glm::vec3 position;
    float ys;
    float yaw, pitch, scale;
    float width, height;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldup;
    float movement_speed;
    float mouse_sensitivity;
    glm::vec3 choosepos;
};
#endif
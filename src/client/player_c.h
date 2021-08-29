#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../world.h"
#include "renderer.h"

#include <string>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    JUMP
};

class Player_c
{
public:
    Player_c();
    void view();
    void processKeyboard(
        Camera_Movement direction,
        float deltaTime); // dir:0-GO,1-BACK,2-LEFT,3-RIGHT，4-UP

    void process_mouse_movement(float xoffset, float yoffset);
    void update_camera_position(float deltaTime);
    void on_left_click(int delayMS);
    void on_right_click(int delayMS);
    void fillBagInCreation();
    World* world;
    TypeManager_c* tm;
    Renderer* drawer;
    string pointThing;
    float yaw, pitch, scale;
    glm::vec3 position;
    Inventory bag;
    int wieldItemIndex; // from 1 to 8
private:
    void update_camera_vectors();
    void hideChunkByViewRange(int viewRange);
    bool jump;
    glm::vec3 lastChunkPos;

private:
    void updateRayPoint();
    glm::vec3 eyePosition;
    float ys;
    float width, height;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldup;
    float movement_speed;
    float mouse_sensitivity;
    glm::vec3 choosepos;
    glm::vec3 placePos;

    float breakTime;
};
#endif
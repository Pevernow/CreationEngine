#include "player_c.h"
#include "../world.h"
#include "bx/math.h"
#include "ray.h"
#include "typemanager_c.h"
#include <SDL2/SDL.h>

#include <iostream>

Player_c::Player_c()
{
    position = glm::vec3(3.0f, 15.0f, 3.0f);
    yaw = -90.0f;
    pitch = 0.0f;
    scale = 0.01f;
    ys = 0.0f;
    width = 640;
    height = 480;
    worldup = glm::vec3(0.0f, 1.0f, 0.0f);
    this->front = glm::vec3(0.0f, 0.0f, 1.0f);
    this->movement_speed = 0.005f;
    this->mouse_sensitivity = 0.1f;
    wieldItemIndex = 1; // item 0 not use
    jump = false;
    bag.items.resize(8 * 4 + 1); // item 0 not use
}

void Player_c::update_camera_position(float deltaTime)
{
    // Physics
    if (jump == true) {
        ys = 6.0;
        jump = false;
    }
    position.y += ys * deltaTime / 1000;
    ys -= 18.0 * deltaTime / 1000;
    if (world->get_node(floor(position.x), floor(position.y), floor(position.z))
            .id != 0) {
        ys = 0;

        // Come back to ground
        int i = floor(position.y);
        while (world->get_node(floor(position.x), i, floor(position.z)).id != 0)
            i++;
        position.y = i;
    }
    eyePosition = position;
    eyePosition.y += 1.5;
    updateRayPoint();
    return;
}

void Player_c::updateRayPoint()
{
    glm::vec3 lastPosition = {0, 0, 0};
    for (Ray ray(eyePosition, front); ray.getLength() < 6; ray.step(0.05)) {
        Block& block = world->get_node(
            floor(ray.getEnd().x), floor(ray.getEnd().y),
            floor(ray.getEnd().z));
        if (block.id != 0) {
            placePos = floor(lastPosition);
            choosepos = floor(ray.getEnd());
            lastPosition = ray.getEnd();
            break;
        }
        lastPosition = ray.getEnd();
    }
    pointThing =
        tm->idToName(world
                         ->get_node(
                             floor(lastPosition.x), floor(lastPosition.y),
                             floor(lastPosition.z))
                         .id);
}

void Player_c::hideChunkByViewRange(int viewRange)
{
    glm::vec3 pos = floor(position);
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;
    getChunkMinPosition(x, y, z);

    if (glm::vec3(x, y, z) == lastChunkPos) {
        // Need not reflush
        return;
    }

    // clean
    for (int i = 0, l = world->worldmap.size(); i < l; i++) {
        world->worldmap[i].show = false;
    }
    for (int i = x - 16 * (viewRange - 1), maxx = x + 16 * (viewRange - 1);
         i <= maxx; i += 16) {
        for (int j = z - 16 * (viewRange - 1), maxz = z + 16 * (viewRange - 1);
             j <= maxz; j += 16) {
            // forceLoadChunk
            world->get_chunk(i, y, j).show = true;
        }
    }

    lastChunkPos = glm::vec3(x, y, z);
    return;
}

void Player_c::view()
{
    hideChunkByViewRange(8);

    float view[16];
    bx::Vec3 Pos = {eyePosition.x, eyePosition.y, eyePosition.z};
    glm::vec3 tmp = eyePosition + front;
    bx::Vec3 Tmp = {tmp.x, tmp.y, tmp.z};
    bx::Vec3 Up = {up.x, up.y, up.z};

    bx::mtxLookAt(view, Pos, Tmp, Up);

    float proj[16];
    bx::mtxProj(
        proj, 60.0f, width / height, 0.1f, 100.0f,
        bgfx::getCaps()->homogeneousDepth);

    bgfx::setViewTransform(0, view, proj);
    return;
}

void Player_c::processKeyboard(Camera_Movement direction, float deltaTime)
{
    glm::vec3 lastpos = position;
    float velocity = movement_speed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position += right * velocity;
    if (direction == RIGHT)
        position -= right * velocity;
    position.y = lastpos.y;
    if (direction == JUMP && position.y == floor(position.y) &&
        world->get_node(
                 floor(position.x), floor(position.y) - 1, floor(position.z))
                .id != 0) {
        jump = true;
    }
    if ( // position.x < 0 || position.y < 0 || position.z < 0 ||
        world->get_node(floor(position.x), floor(position.y), floor(position.z))
            .id != 0) {
        position = lastpos;
    }

    glm::vec3 pos = floor(position);
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;
    getChunkMinPosition(x, y, z);

    if (glm::vec3(x, y, z) != lastChunkPos) {
        // reflush
        drawer->cache = true;
    }

    return;
}

// calculates the front vector from the Player_c's (updated) Euler Angles
void Player_c::update_camera_vectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(
        this->front, worldup)); // normalposition.ze the vectors, because their
                                // length gets closer to 0 the more you look up
                                // or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
    return;
}

void Player_c::process_mouse_movement(float xoffset, float yoffset)
{
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw += xoffset;
    pitch += yoffset;
    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    /*
if (yaw > 180)
    yaw -= 360;
if (yaw < -180)
    yaw += 360;
    */

    // update Front, Right and Up Vectors using the updated Euler angles
    update_camera_vectors();
    return;
}

void Player_c::on_left_click(int delayMS)
{
    if (breakTime >= 1000) {
        string itemID = world->nodeIDtoItemID(
            world->get_node(choosepos.x, choosepos.y, choosepos.z).id);
        bag.putItem(ItemStack(itemID, 1));
        world->set_node(choosepos.x, choosepos.y, choosepos.z, "air");
        breakTime = 0;
    } else {
        breakTime += delayMS;
    }
    return;
}

void Player_c::on_right_click(int delayMS)
{
    if (breakTime >= 300) {
        if (bag.items[wieldItemIndex].num > 0 &&
            world->get_node(placePos.x, placePos.y, placePos.z).id == 0) {
            bool ret = world->set_node(
                placePos.x, placePos.y, placePos.z,
                bag.items[wieldItemIndex].id.c_str());
            if (ret) {
                bag.items[wieldItemIndex].num -= 1;
            }
            breakTime = 0;
        }
    } else {
        breakTime += delayMS;
    }

    return;
}

void Player_c::fillBagInCreation()
{
    int i = 1; // item 0 not use
    for (auto& item : tm->itemmodel) {
        bag.items[i] = ItemStack(item.first, 64);
        i++;
    }
}
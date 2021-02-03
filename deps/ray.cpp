#include "ray.h"

Ray::Ray(const glm::vec3& position, float yaw, float pitch)
    : m_rayStart(position), m_rayEnd(position), yaw(yaw), pitch(pitch)
{
}

void Ray::step(float scale)
{
    auto& p = m_rayEnd;

    p.x -= glm::cos(yaw) * scale;
    p.z -= glm::sin(yaw) * scale;
    p.y -= glm::tan(pitch) * scale;
}

const glm::vec3& Ray::getEnd() const
{
    return m_rayEnd;
}

float Ray::getLength() const
{
    return glm::distance(m_rayStart, m_rayEnd);
}

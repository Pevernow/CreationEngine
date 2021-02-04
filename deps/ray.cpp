#include "ray.h"

Ray::Ray(const glm::vec3& position, const glm::vec3& front)
    : m_rayStart(position), m_rayEnd(position), front(front)
{
}

/*
void Ray::step(float scale)
{
    auto& p = m_rayEnd;

    p.x -= glm::cos(yaw) * scale;
    p.z -= glm::sin(yaw) * scale;
    p.y -= glm::tan(pitch) * scale;
}*/

void Ray::step(float scale)
{
    m_rayEnd += front * scale;
}

const glm::vec3& Ray::getEnd() const
{
    return m_rayEnd;
}

float Ray::getLength() const
{
    return glm::distance(m_rayStart, m_rayEnd);
}

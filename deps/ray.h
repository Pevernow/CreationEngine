#ifndef RAY_H_INCLUDED
#define RAY_H_INCLUDED

#include "glm/glm.hpp"

class Ray
{
public:
    Ray(const glm::vec3& position, const glm::vec3& front);

    void step(float scale);

    const glm::vec3& getEnd() const;

    float getLength() const;

private:
    glm::vec3 m_rayStart;
    glm::vec3 m_rayEnd;
    glm::vec3 front;
};

#endif // RAY_H_INCLUDED

#include "Camera.h"

#define MOVEMENT_SPEED 50.0f

Camera::Camera() : m_up(glm::vec3(0, -1, 0)), m_right(glm::vec3(1, 0, 0))
{

}

void Camera::SetPosition(glm::vec3 a_Pos)
{
  m_transform.SetPosition(glm::vec3(a_Pos.x, a_Pos.y, a_Pos.z));
}

void Camera::Translate(glm::vec3 a_Pos)
{
  m_transform.Translate(glm::vec3(a_Pos.x, a_Pos.y, a_Pos.z));
}

void Camera::Move(glm::vec3 a_direction, float a_deltaTime)
{
  Translate(a_direction * MOVEMENT_SPEED * a_deltaTime);
}
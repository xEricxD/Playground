#include "Camera.h"
#include "Engine.h"

#define MOVEMENT_SPEED 1000.0f

Camera::Camera() : m_up(glm::vec3(0, -1, 0)), m_right(glm::vec3(1, 0, 0))
{

}

void Camera::SetPosition(glm::vec3 a_Pos)
{
  m_transform.SetLocalPosition(glm::vec3(a_Pos.x, a_Pos.y, a_Pos.z));
}

void Camera::Translate(glm::vec3 a_Pos)
{
  m_transform.Translate(glm::vec3(a_Pos.x, a_Pos.y, a_Pos.z));
}

void Camera::Update(float a_dt)
{
  //Camera movement
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) Move(glm::vec3(1, 0, 0), a_dt);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) Move(glm::vec3(-1, 0, 0), a_dt);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) Move(glm::vec3(0, -1, 0), a_dt);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) Move(glm::vec3(0, 1, 0), a_dt);
}

void Camera::Move(glm::vec3 a_direction, float a_deltaTime)
{
  Translate(a_direction * MOVEMENT_SPEED * a_deltaTime);
}
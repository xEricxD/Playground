#include "Camera.h"

#define ROTATE_SPEED 0.5f
#define MOVEMENT_SPEED 20.0f

Camera::Camera() : m_changed(true), m_up(glm::vec3(0, 1, 0)), m_rotation(0), m_projectionMatrix(1), m_position(0)
{
  Update();
}

void Camera::SetPosition(glm::vec3 a_Pos)
{
  m_position = glm::vec3(a_Pos.x, a_Pos.y, a_Pos.z);
  m_changed = true;
}

void Camera::Translate(glm::vec3 a_Pos)
{
  m_position += glm::vec3(a_Pos.x, a_Pos.y, a_Pos.z);
  m_changed = true;
}

void Camera::SetRotation(glm::vec2 a_rotation)
{
  a_rotation *= ROTATE_SPEED;
  m_rotation = a_rotation;

  if (m_rotation.y > 89.0f)
    m_rotation.y = 89.0f;
  else if (m_rotation.y < -89.0f)
    m_rotation.y = -89.0f;

  m_changed = true;
}

void Camera::Rotate(glm::vec2 a_rotation)
{
  a_rotation *= ROTATE_SPEED;
  m_rotation += a_rotation;

  if (m_rotation.y > 89.0f)
    m_rotation.y = 89.0f;
  else if (m_rotation.y < -89.0f)
    m_rotation.y = -89.0f;

  m_changed = true;
}

void Camera::SetProjectionMatrix(float a_fov, float a_aspectRatio, float a_nearclip, float a_farclip)
{
  m_projectionMatrix = glm::perspective(glm::radians(a_fov), a_aspectRatio, a_nearclip, a_farclip);
}

glm::mat4 Camera::GetViewMatrix()
{
  Update();
  return m_viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix()
{
  return m_projectionMatrix;
}

void Camera::Move(Movement a_direction, float a_deltaTime)
{
  switch (a_direction)
  {
  case(FORWARD) :
    Translate(m_direction *   MOVEMENT_SPEED * a_deltaTime);
    break;
  case(BACKWARD) :
    Translate(-m_direction *  MOVEMENT_SPEED * a_deltaTime);
    break;
  case(LEFT) :
    Translate(-m_right *      MOVEMENT_SPEED * a_deltaTime);
    break;
  case(RIGHT) :
    Translate(m_right *       MOVEMENT_SPEED * a_deltaTime);
    break;
  case(UP) :
    Translate(m_up *          MOVEMENT_SPEED * a_deltaTime);
    break;
  case(DOWN) :
    Translate(-m_up *         MOVEMENT_SPEED * a_deltaTime);
    break;
  default:
    break;
  }
}

void Camera::Update()
{
  if (m_changed)
  {
    //update view matrix
    m_direction.x = sin(glm::radians(m_rotation.x)) * cos(glm::radians(m_rotation.y));
    m_direction.y = sin(glm::radians(m_rotation.y));
    m_direction.z = cos(glm::radians(m_rotation.x)) * cos(glm::radians(m_rotation.y));
    m_direction = -glm::normalize(m_direction);

    m_right = glm::normalize(glm::cross(m_direction, glm::vec3(0, 1.0f, 0)));
    m_up = glm::cross(m_right, m_direction);

    m_viewDirection = m_position + m_direction;
    m_viewMatrix = glm::lookAt(m_position, m_viewDirection, m_up);

    m_changed = false;
  }
}
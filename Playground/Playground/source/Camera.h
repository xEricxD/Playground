#pragma once
#include "Includes.h"
#include "TransformComponent.h"

// Very simple 2D camera class
class Camera
{
public:
  Camera();
  ~Camera() {}

  void Update(float a_dt);

  void Move(glm::vec3 a_direction, float a_deltaTime);

  void SetPosition(glm::vec3 a_pos); //set position in world space
  void Translate(glm::vec3 a_pos); //move in local space

  TransformComponent &GetTransform() { return m_transform; }
  glm::vec3 &GetPosition() { return m_transform.GetPosition(); }
  glm::vec3 &GetRight() { return m_right; }
  glm::vec3 &GetUp() { return m_up; }
  
private:
  TransformComponent m_transform;

  glm::vec3 m_up;
  glm::vec3 m_right;
};


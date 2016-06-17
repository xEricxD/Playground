#pragma once
#include "Includes.h"

enum Movement 
{
  FORWARD = 0,
  BACKWARD = 1,
  LEFT = 2,
  RIGHT = 3,
  UP = 4,
  DOWN = 5
};

class Camera
{
public:
  Camera();
  ~Camera() {}

  void Move(Movement a_direction, float a_deltaTime);

  void SetPosition(glm::vec3 a_pos); //set position in world space
  void SetRotation(glm::vec2 a_rotation);
  void SetProjectionMatrix(float a_fov, float a_aspectRatio, float a_nearclip, float a_farclip);

  void Translate(glm::vec3 a_pos); //move in local space
  void Rotate(glm::vec2 a_rotation);

  glm::mat4 GetViewMatrix();
  glm::mat4 GetProjectionMatrix();
  glm::vec3 GetPosition() { return m_position; }
  glm::vec3 GetForward() { return m_direction; }
  glm::vec3 GetRight() { return m_right; }
  glm::vec3 GetUp() { return m_up; }
  glm::vec2 GetRotation() { return m_rotation; }
  
private:
  void Update();
  glm::mat4 m_viewMatrix;
  glm::mat4 m_projectionMatrix;

  glm::vec3 m_position;
  glm::vec3 m_viewDirection; //view direction
  glm::vec3 m_up; //up vector
  glm::vec3 m_right; //right vector
  glm::vec3 m_direction;

  glm::vec2 m_rotation;

  bool m_changed;
};


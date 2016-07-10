#include "TransformComponent.h"

TransformComponent::TransformComponent() : m_changed(true)
{
  m_position = glm::vec3(0, 0, 0); 
  m_scale = glm::vec3(1, 1, 1);
  m_rotation = glm::vec3(0, 0, 0);

  m_transform = glm::mat4(1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          m_position.x, m_position.y, m_position.z, 1);
  
  m_scalingMatrix = glm::mat4(m_scale.x, 0, 0, 0,
                              0, m_scale.y, 0, 0,
                              0, 0, m_scale.z, 0,
                              0, 0, 0, 1);
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetPosition(glm::vec3 a_position)
{
  m_position = glm::vec3(a_position);
  m_transform = glm::mat4(1, 0, 0, 0, 
                          0, 1, 0, 0, 
                          0, 0, 1, 0, 
                          m_position.x, m_position.y, m_position.z, 1);

  m_changed = true;
}

void TransformComponent::Translate(glm::vec3 a_position)
{
  m_position += glm::vec3(a_position);
  m_transform = glm::mat4(1, 0, 0, 0, 
                          0, 1, 0, 0, 
                          0, 0, 1, 0, 
                          m_position.x, m_position.y, m_position.z, 1);
  m_changed = true;
}

void TransformComponent::SetScale(glm::vec3 a_scale)
{
  m_scale = a_scale;
  m_scalingMatrix = glm::mat4(m_scale.x, 0, 0, 0, 
                              0, m_scale.y, 0, 0, 
                              0, 0, m_scale.z, 0, 
                              0, 0, 0, 1);
  m_changed = true;
}

void TransformComponent::Scale(glm::vec3 a_scale)
{
  m_scale += a_scale;
  m_scalingMatrix = glm::mat4(m_scale.x, 0, 0, 0, 
                              0, m_scale.y, 0, 0, 
                              0, 0, m_scale.z, 0, 
                              0, 0, 0, 1);
  m_changed = true;
}

void TransformComponent::SetRotation(glm::vec3 a_rotation)
{
  m_rotation = a_rotation;
  UpdateRotation();
}

void TransformComponent::Rotate(glm::vec3 a_rotation)
{
  m_rotation += a_rotation;
  UpdateRotation();
}

void TransformComponent::UpdateRotation()
{
  //calculate rotation for each axis
  float rad = m_rotation.x * PI / 180;
  glm::mat4 rmx = glm::mat4(1, 0, 0, 0, 
                            0, cos(rad), sin(rad), 0, 
                            0, -sin(rad), cos(rad), 0, 
                            0, 0, 0, 1);

  rad = m_rotation.y * PI / 180;
  glm::mat4 rmy = glm::mat4(cos(rad), 0, -sin(rad), 0,
                            0, 1, 0, 0, 
                            sin(rad), 0, cos(rad), 0, 
                            0, 0, 0, 1);

  rad = m_rotation.z * PI / 180;
  glm::mat4 rmz = glm::mat4(cos(rad), sin(rad), 0, 0,
                            -sin(rad), cos(rad), 0, 0, 
                            0, 0, 1, 0, 
                            0, 0, 0, 1);

  m_rotationMatrix = (rmz * rmy * rmx);

  m_changed = true;
}

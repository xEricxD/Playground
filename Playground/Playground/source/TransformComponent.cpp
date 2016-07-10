#include "TransformComponent.h"
#include <iostream>

TransformComponent::TransformComponent() : m_changed(true), m_parent(nullptr)
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

void TransformComponent::SetLocalPosition(glm::vec3 a_position)
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

void TransformComponent::SetLocalScale(glm::vec3 a_scale)
{
  m_scale = a_scale;
  m_scalingMatrix = glm::mat4(m_scale.x, 0, 0, 0, 
                              0, m_scale.y, 0, 0, 
                              0, 0, m_scale.z, 0, 
                              0, 0, 0, 1);
  m_changed = true;
}

void TransformComponent::SetWorldPosition(glm::vec3 a_position)
{
  a_position;
  printf("Called unimplemented SetPosition function in Transformcomponent.cpp\n");
}

void TransformComponent::SetWorldRotation(glm::vec3 a_rotation)
{
  a_rotation;
  printf("Called unimplemented SetRotation function in Transformcomponent.cpp\n");
}

void TransformComponent::SetWorldScale(glm::vec3 a_scale)
{
  a_scale;
  printf("Called unimplemented SetScale function in Transformcomponent.cpp\n");
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

void TransformComponent::SetLocalRotation(glm::vec3 a_rotation)
{
  m_rotation = a_rotation;
  UpdateRotation();
}

void TransformComponent::Rotate(glm::vec3 a_rotation)
{
  m_rotation += a_rotation;
  UpdateRotation();
}

const glm::vec3 TransformComponent::GetPosition(CoordinateSpace a_space)
{
  if (!m_parent || a_space == CoordinateSpace::LOCAL)
    return m_position;
  else // we have a parent and want the world space coordinate
    return m_parent->GetPosition(a_space) + m_position;
}

const glm::vec3 TransformComponent::GetRotation(CoordinateSpace a_space)
{
  if (!m_parent || a_space == CoordinateSpace::LOCAL)
    return m_rotation;
  else // we have a parent and want the world space coordinate
    return m_parent->GetRotation(a_space) + m_rotation;
}

const glm::vec3 TransformComponent::GetScale(CoordinateSpace a_space)
{
  if (!m_parent || a_space == CoordinateSpace::LOCAL)
    return m_scale;
  else // we have a parent and want the world space coordinate
    return m_parent->GetScale(a_space) * m_scale;
}

bool TransformComponent::HasChanged()
{
  if (m_changed)
    return true;
  else if (m_parent)
    return m_parent->HasChanged();
  else
    return false;
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

#pragma once
#include "Includes.h"

class TransformComponent
{
public:
  TransformComponent();
  ~TransformComponent();

  void SetPosition(glm::vec3 a_position);
  void SetRotation(glm::vec3 a_rotation);
  void SetScale(glm::vec3 a_scale);

  void Translate(glm::vec3 a_translation);
  void Rotate(glm::vec3 a_rotation);
  void Scale(glm::vec3 a_scale);

  glm::vec3 &GetPosition() { return m_position; }
  glm::vec3 &GetRotation() { return m_rotation; }
  glm::vec3 &GetScale() { return m_scale; }

  const glm::mat4 &GetTransformationMatrix() { return m_transform; }
  const glm::mat4 &GetScaleMatrix() { return m_scalingMatrix; }
  const glm::mat4 &GetRotationMatrix() { return m_rotationMatrix; }

private:
  void UpdateRotation(); //call when a rotate or setrotation is called

  glm::mat4 m_transform;
  glm::mat4 m_scalingMatrix;
  glm::mat4 m_rotationMatrix;

  glm::vec3 m_position;
  glm::vec3 m_rotation;
  glm::vec3 m_scale;
};


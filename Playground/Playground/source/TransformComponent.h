#pragma once
#include "Includes.h"

class TransformComponent
{
public:

  enum CoordinateSpace
  {
    LOCAL = 0,
    WORLD
  };

  TransformComponent();
  ~TransformComponent();

  void Translate(glm::vec3 a_translation);
  void Rotate(glm::vec3 a_rotation);
  void Scale(glm::vec3 a_scale);

  // Update transform in local space
  void SetLocalPosition(glm::vec3 a_position);
  void SetLocalRotation(glm::vec3 a_rotation);
  void SetLocalScale(glm::vec3 a_scale);

  // Update transform in world space
  void SetWorldPosition(glm::vec3 a_position);
  void SetWorldRotation(glm::vec3 a_rotation);
  void SetWorldScale(glm::vec3 a_scale);

  glm::vec3 GetPosition(CoordinateSpace a_space = CoordinateSpace::WORLD) const;
  glm::vec3 GetRotation(CoordinateSpace a_space = CoordinateSpace::WORLD) const;
  glm::vec3 GetScale(CoordinateSpace a_space = CoordinateSpace::WORLD) const;

  glm::mat4 GetTransformationMatrix(CoordinateSpace a_space = CoordinateSpace::WORLD) const;
  glm::mat4 GetScaleMatrix(CoordinateSpace a_space = CoordinateSpace::WORLD) const;
  glm::mat4 GetRotationMatrix(CoordinateSpace a_space = CoordinateSpace::WORLD) const;

  bool HasChanged();
  // since the transform doesnt have it's own update, we want to resest this bool from the collision system
  void ResetChanged();

  void SetParent(TransformComponent* a_parent) { m_parent = a_parent; }
  TransformComponent* const GetParent() { return m_parent; }

private:
  void UpdateRotation(); //call when a rotate or setrotation is called
  
  // store if the transform has been updated
  bool m_changed;

  glm::mat4 m_transform;
  glm::mat4 m_scalingMatrix;
  glm::mat4 m_rotationMatrix;

  glm::vec3 m_position;
  glm::vec3 m_rotation;
  glm::vec3 m_scale;

  TransformComponent* m_parent;
};


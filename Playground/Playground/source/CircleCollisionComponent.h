#pragma once
#include "CollisionComponent.h"

class CircleCollisionComponent : public CollisionComponent
{
public:
  CircleCollisionComponent();
  virtual ~CircleCollisionComponent() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;
  virtual void ShutDown() override;

  // Generate an AABB collision for the broad phase collision check
  virtual void GenerateAABB() override;

private:
  float m_radius;
};


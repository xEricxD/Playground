#pragma once
#include "CollisionComponent.h"

class CircleCollisionComponent : public CollisionComponent
{
public:
  CircleCollisionComponent(float a_radius = 20);
  virtual ~CircleCollisionComponent() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;
  virtual void ShutDown() override;

  // Generate an AABB collision for the broad phase collision check
  virtual void GenerateAABB() override;

protected:
  virtual void AddVertices() override;

private:
  float m_radius;
};


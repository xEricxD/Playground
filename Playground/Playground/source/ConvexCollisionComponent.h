#pragma once
#include "CollisionComponent.h"

class ConvexCollisionComponent : public CollisionComponent
{
public:
  ConvexCollisionComponent();
  virtual ~ConvexCollisionComponent() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;
  virtual void ShutDown() override;

  // Generate an AABB collision for the broad phase collision check
  virtual void GenerateAABB() override;

protected:
  virtual void AddVertices() override;
};


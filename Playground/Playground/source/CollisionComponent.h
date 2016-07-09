#pragma once
#include "GameobjectComponent.h"

// simple AABB struct used for broad phase collision
struct AABB
{
  glm::vec2 min; // bottom left corner
  glm::vec2 max; // top right corner
};

class CollisionComponent : public GameobjectComponent
{
public:
  CollisionComponent();
  virtual ~CollisionComponent() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;
  virtual void ShutDown() override;

  // Generate an AABB collision for the broad phase collision check
  virtual void GenerateAABB() = 0;

  AABB GetAABB() { return m_AABB; }

  void SetDrawDebug(bool a_value) { m_drawDebug = a_value; }

protected:
  AABB m_AABB;

private:
  void DrawDebug();
  bool m_drawDebug;
};


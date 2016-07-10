#pragma once
#include "GameobjectComponent.h"
#include <vector>

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
  virtual void GenerateAABB();

  AABB* GetAABB() { return &m_AABB; }

  void SetDrawDebug(bool a_value) { m_drawDebug = a_value; }

protected:
  // for concave shapes
  void GenerateConvexHull();
  // Add all the vertices of the object to the vertex array
  virtual void AddVertices() = 0;

  // store the objects AABB
  AABB m_AABB;
  // all vertices of the collider, in vertex space
  std::vector<glm::vec2> m_vertices;
  // if this object hasn't moved, we do not need to test for collision against other sleeping objects, since none of them moved
  bool m_isSleeping;

private:
  void DrawDebug();
  bool m_drawDebug;

  inline glm::vec2 ConvertVertexToViewSpace(glm::vec2 a_vertex);
};


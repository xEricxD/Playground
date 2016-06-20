#pragma once
#include "GameobjectComponent.h"
#include "PathfindingStructs.h"

// TODO - make base class for pathfinder

class AStarPathfinderObject;

class GridNavigationComponent : public GameobjectComponent
{
public:
  GridNavigationComponent();
  virtual ~GridNavigationComponent() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;

  void TraversePath(float a_dt);

  void RequestPath(glm::vec2 a_goalPosition, bool a_stopCurrentPath = false);
  void PathRequestFailed();

  void SetPath(Path* a_path);

  void SetDrawDebug(bool a_value) { m_drawDebug = a_value; }

private:
  void DrawDebug();
  bool m_drawDebug;

  AStarPathfinderObject* m_pathfinder;
  Path* m_activePath;
};


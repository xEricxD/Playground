#pragma once
#include "Scene.h"

class AgentObject;

class PathfindingScene : public Scene
{
public:
  PathfindingScene();
  virtual ~PathfindingScene() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;
  virtual void ShutDown() override;

private:
  AgentObject* m_agent;
};


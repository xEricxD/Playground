#pragma once
#include "Scene.h"

class AgentObject;

class AStarScene : public Scene
{
public:
  AStarScene();
  virtual ~AStarScene() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;
  virtual void ShutDown() override;

private:
  AgentObject* m_agent;
};


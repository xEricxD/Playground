#pragma once
#include "Scene.h"

class AgentObject;

class TestScene : public Scene
{
public:
  TestScene();
  virtual ~TestScene() override;

  virtual void Initialise();
  virtual void Update(float a_dt);
  virtual void ShutDown();

private:
  AgentObject* m_agent; // TODO - temp agent
};


#pragma once
#include "Scene.h"

class TestScene : public Scene
{
public:
  TestScene();
  virtual ~TestScene() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;
  virtual void ShutDown() override;

private:

};


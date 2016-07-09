#pragma once
#include "Scene.h"

class QuadTreeScene : public Scene
{
public:
  QuadTreeScene();
  virtual ~QuadTreeScene();

  virtual void Initialise();
  virtual void Update(float a_dt);
  virtual void ShutDown();


};


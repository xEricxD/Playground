#pragma once
#include "GameObject.h"

class QuadTreeObject : public GameObject
{
public:
  QuadTreeObject();
  virtual ~QuadTreeObject() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;

private:

};


#pragma once
#include "GameObject.h"
#include "GridNavigationComponent.h"

class AgentObject : public GameObject
{
public:
  AgentObject();
  virtual ~AgentObject() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;

  void MoveToLocation(glm::vec2 a_goalLocation);
  GridNavigationComponent* GetNavigationComponent() { return m_navigationComponent; }

private:
  GridNavigationComponent* m_navigationComponent;
};


#pragma once
#include "GameobjectComponent.h"

class GridObject;

class GridEditorComponent : public GameobjectComponent
{
public:
  GridEditorComponent(bool a_allowEditing = false);
  virtual ~GridEditorComponent() override;

  virtual void Initialise() override;
  virtual void Update(float a_dt) override;

private:
  inline void UpdateGridNode();

  GridObject* m_grid;

  bool m_allowEditing;
  // to check if mouse has been released since we did our click event
  bool m_pressed;
};


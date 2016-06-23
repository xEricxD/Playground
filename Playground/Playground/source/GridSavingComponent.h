#pragma once
#include "GameobjectComponent.h"
#include <string>

class GridObject;

class GridSavingComponent : public GameobjectComponent
{
public:
  GridSavingComponent(bool a_loadOnStart = true, bool a_saveOndestroy = true);
  virtual ~GridSavingComponent() override;

  virtual void Initialise() override;
  virtual void ShutDown() override;

  void SaveGridData(std::string a_filepath = "../assets/saved/griddata.txt");
  void LoadGridData(std::string a_filepath = "../assets/saved/griddata.txt");

private:
  GridObject* m_grid; // grid to save
  bool m_saveOnDestroy;
  bool m_loadOnStart;
};


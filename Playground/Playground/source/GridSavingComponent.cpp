#include "GridSavingComponent.h"
#include "GridObject.h"
#include <iostream>
#include <fstream>

GridSavingComponent::GridSavingComponent(bool a_loadOnStart, bool a_saveOndestroy)
  : m_saveOnDestroy(a_saveOndestroy), m_grid(nullptr), m_loadOnStart(a_loadOnStart)
{
}

GridSavingComponent::~GridSavingComponent()
{
}

void GridSavingComponent::Initialise()
{
  GameobjectComponent::Initialise();

  m_componentType = GameobjectComponent::ComponentType::GRIDSAVING;

  // set the grid we want to save as the owner
  if (GetOwner())
    m_grid = (GridObject*)GetOwner();

  if (m_loadOnStart)
    LoadGridData();
}

void GridSavingComponent::ShutDown()
{
  if (m_saveOnDestroy)
    SaveGridData();
}

void GridSavingComponent::SaveGridData(std::string a_filepath)
{
  // open the file
  std::ofstream file;
  file.open(a_filepath);

  // write our data
  glm::vec2 size = m_grid->GetGridSize();
  file << std::to_string(size.x) << "\n";
  file << std::to_string(size.y) << "\n";

  glm::vec2 offset = m_grid->GetOffset();
  file << std::to_string(offset.x) << "\n";
  file << std::to_string(offset.y) << "\n";

  glm::vec2 nodeSize = m_grid->GetNodeSize();
  file << std::to_string(nodeSize.x) << "\n";
  file << std::to_string(nodeSize.y) << "\n";

  // now store the grid
  for (int y = 0; y < size.y; y++)
  {
    for (int x = 0; x < size.x; x++)
    {
      PathfindingNode* n = m_grid->GetNodeByIndex(glm::vec2(x, y));
      (n && n->available) ? file << "1 " : file << "0 ";
    }
    file << "\n";
  }
  // close the file
  file.close();
  printf("Succesfully saved grid data to %s\n", a_filepath.c_str());
}

// TODO - add safety checks for loading
void GridSavingComponent::LoadGridData(std::string a_filepath)
{
  printf("\nloading map: %s\n", a_filepath.c_str());

  std::ifstream file(a_filepath);
  if (file.is_open())
  {
    std::string line; // we will store our read data here
    std::getline(file, line);
    if (!line.length())
    {
      printf("invalid line whilst trying to load grid data, stopping read\n");
      return;
    }
    
    // load in the same order we save
    glm::vec2 size;
    size.x = std::stof(line);
    std::getline(file, line);
    size.y = std::stof(line);
    printf("map size: (%f, %f)\n", size.x, size.y);

    glm::vec2 offset;
    std::getline(file, line);
    offset.x = std::stof(line);
    std::getline(file, line);
    offset.y = std::stof(line);
    printf("map offset: (%f, %f)\n", offset.x, offset.y);

    glm::vec2 nodeSize;
    std::getline(file, line);
    nodeSize.x = std::stof(line);
    std::getline(file, line);
    nodeSize.y = std::stof(line);
    printf("PathfindingNode size: (%f, %f)\n", nodeSize.x, nodeSize.y);

    // generate the grid after loading in all the data
    m_grid->SetNodeSize(nodeSize);
    m_grid->ResizeGrid(size, offset);


    //printf("map data: \n");
    // now find invalid cells and turn them into walls
    bool val;
    for (int y = 0; y < size.y; y++)
    {
      for (int x = 0; x < size.x; x++)
      {
        file >> val;
        //printf("%i ", val);
        PathfindingNode* n = m_grid->GetNodeByIndex(glm::vec2(x, y));
        if (n)
          n->available = val;
      }
      //printf("\n");
    }
    // update all neighbors
    m_grid->UpdateAllNeighbors();

    // close the file
    file.close();
    // some extra newlines for clarity
    printf("\n\n"); 
  }
  else
    printf("Unable to open file: %s\n", a_filepath.c_str());
}
#pragma once
#include "GameObject.h"
#include <map>
#include <SFML/Graphics.hpp>

class GridSavingComponent;

enum ListStatus
{
  NONE = 0,
  ON_OPEN,
  ON_CLOSED
};

// individual nodes for the grid
struct PathfindingNode
{
  PathfindingNode() : neighbors(), parentNode(nullptr), f(0), g(0), h(0), available(true), listStatus(ListStatus::NONE)
  {}

  glm::vec2 position;
  glm::vec2 gridIndex;
  
  bool available;

  // final, current (start - current) and heuristic costs
  short f, g, h;

  ListStatus listStatus;

  PathfindingNode* parentNode;
  PathfindingNode* neighbors[8];
};

// pathfinding grid class
class GridObject : public GameObject
{
public:
  GridObject();
  GridObject(glm::vec2 a_size, glm::vec2 a_offset, glm::vec2 a_nodeSize);
  virtual ~GridObject() override;
  
  virtual void Initialise() override;
  virtual void Update(float a_dt) override;

  void ResizeGrid(glm::vec2 a_newSize);
  void ResizeGrid(glm::vec2 a_newSize, glm::vec2 a_offset);

  PathfindingNode* const GetNodeByPosition(glm::vec2 a_position);
  PathfindingNode* const GetNodeByIndex(glm::vec2 a_index);

  const glm::vec2 GetNodeSize() { return m_nodeSize; }
  const glm::vec2 GetGridSize() { return m_size; }
  const glm::vec2 GetOffset() { return m_offset; }

  void SetNodeSize(glm::vec2 a_nodeSize) { m_nodeSize = a_nodeSize; }
  void SetGridSize(glm::vec2 a_size) { m_size = a_size; }
  void SetGridOffset(glm::vec2 a_offset) { m_offset = a_offset; }

  void SetDrawDebug(bool a_value, bool a_drawSimple = true) { m_drawDebug = a_value, m_drawSimple = a_drawSimple; }
  void ClearGrid();
  void GenerateGrid();
  void UpdateAllNeighbors();
  void UpdateNodeAvailability(PathfindingNode* a_node);

  void LoadGridFromFile();

  glm::vec2 GetRandomPositionOnGrid();

private:
  void DrawDebugGrid();

  void FindNeighbors(PathfindingNode &a_node);
  // TODO -> render differently?
  sf::Sprite m_sprite;
  sf::Texture m_texture;

  // Array with all the grid nodes
  std::vector<PathfindingNode> m_grid;

  glm::vec2 m_nodeSize;
  glm::vec2 m_offset;
  glm::vec2 m_size;

  bool m_drawDebug;
  bool m_drawSimple;

  // saving / loading
  GridSavingComponent* m_savingComp;
};


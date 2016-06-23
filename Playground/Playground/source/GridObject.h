#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

enum ListStatus
{
  NONE = 0,
  ON_OPEN,
  ON_CLOSED
};

// individual nodes for the grid
struct Node
{
  Node() : neighbors(), parentNode(nullptr), f(0), g(0), h(0), available(true), listStatus(ListStatus::NONE)
  {}

  glm::vec2 position;
  glm::vec2 gridIndex;
  
  bool available;

  // final, current (start - current) and heuristic costs
  short f, g, h;

  ListStatus listStatus;

  Node* parentNode;
  Node* neighbors[8];
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

  Node* GetNodeByPosition(glm::vec2 a_position);
  Node* GetNodeByIndex(glm::vec2 a_index);

  glm::vec2 GetNodeSize() { return m_nodeSize; }
  glm::vec2 GetGridSize() { return m_size; }
  glm::vec2 GetOffset() { return m_offset; }

  void SetNodeSize(glm::vec2 a_nodeSize) { m_nodeSize = a_nodeSize; }
  void SetGridSize(glm::vec2 a_size) { m_size = a_size; }
  void SetGridOffset(glm::vec2 a_offset) { m_offset = a_offset; }

  void SetDrawDebug(bool a_value, bool a_drawSimple = true) { m_drawDebug = a_value, m_drawSimple = a_drawSimple; }
  void ClearGrid();
  void GenerateGrid();
  void UpdateNeighbors();

private:
  void DrawDebugGrid();

  void FindNeighbors(Node &a_node);

  // TODO -> render differently?
  sf::Sprite m_sprite;
  sf::Texture m_texture;

  // Array with all the grid nodes
  std::vector<Node> m_grid;

  glm::vec2 m_nodeSize;
  glm::vec2 m_offset;
  glm::vec2 m_size;

  bool m_drawDebug;
  bool m_drawSimple;
};


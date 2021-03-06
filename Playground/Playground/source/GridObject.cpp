#include "GridObject.h"
#include "Engine.h"
#include "GridSavingComponent.h"
#include "GridEditorComponent.h"

// TODO - make automatic grid generator?
#define USING_JPS

GridObject::GridObject() : m_size(glm::vec2(32, 32)), m_nodeSize(glm::vec2(64, 64)), m_offset(glm::vec2(0)),
                           m_drawDebug(false)
{
}

GridObject::GridObject(glm::vec2 a_size, glm::vec2 a_offset, glm::vec2 a_nodeSize)
  : m_size(a_size), m_nodeSize(a_nodeSize), m_offset(a_offset),
  m_drawDebug(false)
{
}

GridObject::~GridObject()
{
}

void GridObject::Initialise()
{
  GameObject::Initialise();
  m_gameObjectType = GameObject::GameObjectType::GRID;

  GenerateGrid();

  m_savingComp = new GridSavingComponent(false, false);
  AddComponent(m_savingComp);

  GridEditorComponent* editorComp = new GridEditorComponent(false);
  AddComponent(editorComp);

  if (m_texture.loadFromFile("../assets/textures/wall.png"))
  {
    m_sprite.setTexture(m_texture);
    m_sprite.setOrigin(m_sprite.getTexture()->getSize().x * 0.5f, m_sprite.getTexture()->getSize().y * 0.5f);
  }
}

void GridObject::Update(float a_dt)
{
  GameObject::Update(a_dt);

  // render the walls
  for (auto PathfindingNode : m_grid)
  {
    if (!PathfindingNode.available)
    {
      glm::mat4 viewMatrix = m_transform->GetTransformationMatrix() * glm::inverse(Engine.GetActiveCamera().GetTransform().GetTransformationMatrix());
      glm::vec2 drawPosition(viewMatrix[3]); // get the x and y component from the vec3
      m_sprite.setPosition(sf::Vector2f(PathfindingNode.position.x + drawPosition.x, PathfindingNode.position.y + drawPosition.y));
      Engine.GetWindow().draw(m_sprite);
    }
  }

  if (m_drawDebug)
    DrawDebugGrid();
}

void GridObject::ResizeGrid(glm::vec2 a_newSize)
{
  m_size = a_newSize;
  GenerateGrid();
}

void GridObject::ResizeGrid(glm::vec2 a_newSize, glm::vec2 a_offset)
{
  m_size = a_newSize;
  m_offset = a_offset;

  GenerateGrid();
}

PathfindingNode* const GridObject::GetNodeByPosition(glm::vec2 a_position)
{
  glm::vec2 startPositionOfGrid(m_offset.x * m_nodeSize.x, m_offset.y * m_nodeSize.y);
  glm::vec2 offset = -startPositionOfGrid + a_position;

  // calculate grid indices
  int xIndex = (int)glm::round(offset.x / m_nodeSize.x);
  int yIndex = (int)glm::round(offset.y / m_nodeSize.y);

  return GetNodeByIndex(glm::vec2(xIndex, yIndex));
}

PathfindingNode* const GridObject::GetNodeByIndex(glm::vec2 a_index)
{
  // make sure we're looking inside grid bounds
  if (a_index.x >= 0 && a_index.y >= 0 && a_index.x < m_size.x && a_index.y < m_size.y)
    return &m_grid[(unsigned int)(a_index.x + a_index.y * m_size.x)]; // return the PathfindingNode we're looking for
  
  return nullptr;
}

void GridObject::GenerateGrid()
{
  if (m_size.x == 0 || m_size.y == 0)
  {
    printf("Error: grid size is 0 on x or y, stopped generation\n");
    return;
  }

  m_grid.clear();

  // calculate number of nodes in grid
  int gridSize = (int)(m_size.x * m_size.y);
  // calcualte our starting position
  glm::vec2 startPosition(m_offset.x * m_nodeSize.x, m_offset.y * m_nodeSize.y);

  glm::vec2 currentNodePos;
  glm::vec2 counter(0);

  for (int i = 0; i < gridSize; i++)
  {
    currentNodePos.x = startPosition.x + counter.x * m_nodeSize.x;
    currentNodePos.y = startPosition.y + counter.y * m_nodeSize.y;

    // generate a new PathfindingNode with all the correct values
    PathfindingNode newNode = PathfindingNode();
    newNode.position = currentNodePos;
    newNode.gridIndex = counter;

    m_grid.push_back(newNode);
    
    if (counter.x < m_size.x - 1)
      counter.x++;
    else if (counter.y < m_size.y - 1)
    {
      counter.y++;
      counter.x = 0;
    }
  }

  // now search for all the neighbors of each PathfindingNode
  for (int i = 0; i < gridSize; i++)
    FindNeighbors(m_grid[i]);
}

void GridObject::UpdateAllNeighbors()
{
  int gridSize = (int)(m_size.x * m_size.y);
  // now search for all the neighbors of each PathfindingNode
  for (int i = 0; i < gridSize; i++)
  {
    for (int j = 0; j < 8; j++)
      m_grid[i].neighbors[j] = nullptr;
    FindNeighbors(m_grid[i]);
  }
}

void GridObject::UpdateNodeAvailability(PathfindingNode* a_node)
{
  // update the nodes availability
  a_node->available = !a_node->available;

  std::vector<PathfindingNode*> currentNeighbors;
  for (auto neighbor : a_node->neighbors)
  {
    if (neighbor)
      currentNeighbors.push_back(neighbor);
  }
  
  // set all current neighbors to null
  for (int i = 0; i < 8; i++)
    a_node->neighbors[i] = nullptr;
  
  FindNeighbors(*a_node);

  for (auto neighbor : currentNeighbors)
  {
    if (neighbor && neighbor->available)
    {
      for (int j = 0; j < 8; j++)
        neighbor->neighbors[j] = nullptr;

      FindNeighbors(*neighbor);
    }
  }
}

void GridObject::LoadGridFromFile()
{
  m_savingComp->LoadGridData();
}

glm::vec2 GridObject::GetRandomPositionOnGrid()
{
  int gridSize = (int)(m_size.x * m_size.y);
  int rnd = rand() % gridSize - 1;

  if (rnd < 0)
    rnd = 0;

  unsigned int spinCount = 0;
  while (!m_grid[rnd].available && spinCount < 100)
  {
    rnd = rand() % gridSize - 1;
    spinCount++;
  }
  
  return m_grid[rnd].position;
}

void GridObject::FindNeighbors(PathfindingNode& a_node)
{
  // if the PathfindingNode isn't available, we will never need it's neighbors, so there's no point setting them
  if (!a_node.available)
    return;

  // set left neighbor
  PathfindingNode* n = GetNodeByIndex(a_node.gridIndex + glm::vec2(-1, 0));
  if (n && n->available)
    a_node.neighbors[0] = n;
  //right
  n = GetNodeByIndex(a_node.gridIndex + glm::vec2(1, 0));
  if (n && n->available)
    a_node.neighbors[1] = n;
  //top
  n = GetNodeByIndex(a_node.gridIndex + glm::vec2(0, -1));
  if (n && n->available)
    a_node.neighbors[2] = n;
  //bottom
  n = GetNodeByIndex(a_node.gridIndex + glm::vec2(0, 1));
  if (n && n->available)
    a_node.neighbors[3] = n;

  //now add diagonals
#ifndef USING_JPS 
  //left - top
  //if top or left PathfindingNode is not a neighbor yet, we can't move to the diagonal because one of those nodes is unavailable, which means we would move into a wall
  if (a_node.neighbors[0] && a_node.neighbors[2])
  {
    n = GetNodeByIndex(a_node.gridIndex + glm::vec2(-1, -1));
    if (n && n->available)
      a_node.neighbors[4] = n;
  }
  //left - bottom
  if (a_node.neighbors[0] && a_node.neighbors[3])
  {
    n = GetNodeByIndex(a_node.gridIndex + glm::vec2(-1, 1));
    if (n && n->available)
      a_node.neighbors[5] = n;
  }
  //right - top
  if (a_node.neighbors[1] && a_node.neighbors[2])
  {
    n = GetNodeByIndex(a_node.gridIndex + glm::vec2(1, -1));
    if (n && n->available)
      a_node.neighbors[6] = n;
  }
  //right - bottom
  if (a_node.neighbors[1] && a_node.neighbors[3])
  {
    n = GetNodeByIndex(a_node.gridIndex + glm::vec2(1, 1));
    if (n && n->available)
      a_node.neighbors[7] = n;
  }
#else
  //left - top
  n = GetNodeByIndex(a_node.gridIndex + glm::vec2(-1, -1));
  if (n && n->available)
    a_node.neighbors[4] = n;
  //left - bottom
  n = GetNodeByIndex(a_node.gridIndex + glm::vec2(-1, 1));
  if (n && n->available)
    a_node.neighbors[5] = n;
  //right - top
  n = GetNodeByIndex(a_node.gridIndex + glm::vec2(1, -1));
  if (n && n->available)
    a_node.neighbors[6] = n;
  //right - bottom
  n = GetNodeByIndex(a_node.gridIndex + glm::vec2(1, 1));
  if (n && n->available)
    a_node.neighbors[7] = n;
#endif
}

void GridObject::ClearGrid()
{
  for (PathfindingNode &PathfindingNode : m_grid)
  {
    PathfindingNode.f = PathfindingNode.h = PathfindingNode.g = 0;
    PathfindingNode.listStatus = ListStatus::NONE;
    PathfindingNode.parentNode = nullptr;
  }
}

void GridObject::DrawDebugGrid()
{
  // get the view matrix to get the correct draw position
  glm::mat4 viewMatrix = m_transform->GetTransformationMatrix() * glm::inverse(Engine.GetActiveCamera().GetTransform().GetTransformationMatrix());
  glm::vec2 drawPosition(viewMatrix[3]); // get the x and y component from the vec3

  if (m_drawSimple)
  {
    glm::vec2 startPosition(m_offset.x * m_nodeSize.x, m_offset.y * m_nodeSize.y);

    // simply draw the line of the grid, not individual nodes
    // draw x lines
    for (int i = 0; i <= m_size.x; i++)
    {
      float xStart = startPosition.x + (i * m_nodeSize.x) - (m_nodeSize.x * 0.5f) + drawPosition.x;
      float yStart = startPosition.y - (m_nodeSize.y * 0.5f) + drawPosition.y;
      sf::Vertex line[]
      {
        sf::Vertex(sf::Vector2f(xStart, yStart)),
        sf::Vertex(sf::Vector2f(xStart, yStart + (m_size.y * m_nodeSize.y)))
      };

      Engine.GetWindow().draw(line, 2, sf::Lines);
    }

    // draw y lines
    for (int i = 0; i <= m_size.y; i++)
    {
      float xStart = startPosition.x - (m_nodeSize.x * 0.5f) + drawPosition.x;
      float yStart = startPosition.y + (i * m_nodeSize.y) - (m_nodeSize.y * 0.5f) + drawPosition.y;
      sf::Vertex line[]
      {
        sf::Vertex(sf::Vector2f(xStart, yStart)),
        sf::Vertex(sf::Vector2f(xStart + (m_size.x * m_nodeSize.x), yStart))
      };

      Engine.GetWindow().draw(line, 2, sf::Lines);
    }
  }
  else
  {
    // draw a box for each PathfindingNode
    sf::RectangleShape box(sf::Vector2f(m_nodeSize.x, m_nodeSize.y));
    box.setOrigin(sf::Vector2f(m_nodeSize.x, m_nodeSize.y) * 0.5f);
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineThickness(1.f);

    for (PathfindingNode &PathfindingNode : m_grid)
    {
      box.setPosition(sf::Vector2f(PathfindingNode.position.x + drawPosition.x, PathfindingNode.position.y + drawPosition.y));

      if (PathfindingNode.available)
        box.setOutlineColor(sf::Color::White);
      else
        box.setOutlineColor(sf::Color::Red);

      Engine.GetWindow().draw(box);
    }
  }
}
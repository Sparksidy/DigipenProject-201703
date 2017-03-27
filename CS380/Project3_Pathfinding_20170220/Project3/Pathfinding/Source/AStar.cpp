#include <Stdafx.h>

static Terrain* terrain = nullptr;
#define getCoordinate(argument1,argument2) terrain->GetCoordinates(argument2,argument1)
static float boxDistance;
GarbageCollector* g_garbage = nullptr;

void Node::operator=(const Node & rhs)
{
  g = rhs.g;
  h = rhs.h;
  parent = rhs.parent;
}
void AStar::PreComputeOctile()
{
  for (int i = 0; i < MAX_MAPSIZE; ++i)
  {
    for (int j = 0; j < MAX_MAPSIZE; ++j)
    {
      precomputedHeuristic[i + (j * MAX_MAPSIZE)] = HeuristicOctile(g_garbage->GetGridPos(i), g_garbage->GetGridPos(j));
    }
  }
}

AStar::AStar()
{
  terrain = &g_terrain;
  g_garbage = new GarbageCollector;
  boxDistance = D3DXVec3Length(&(terrain->GetCoordinates(0, 0) - terrain->GetCoordinates(1, 0))) * 1.5f;
  PreComputeOctile();
}

AStar::~AStar()
{
  delete g_garbage;
}

void AStar::Init()
{
  if (initialized)
    return;

  //assign heuristic calculating method
  heuristicFn[0] = HeuristicEucledean;
  heuristicFn[1] = HeuristicOctile;
  heuristicFn[2] = HeuristicChebyshev;
  heuristicFn[3] = HeuristicManhattan;

  initialized = true;
}

void AStar::Reset(int x, int y,int endX, int endY)
{
  Init();

  if (x == endX && y == endY)
    return;

  g_garbage->Clear();
  ClearOpenList();

  start = g_garbage->Allocate(x, y);
  end = g_garbage->Allocate(endX, endY);
  endIdx = g_garbage->index(endX, endY);

  *start = CalculateCost(nullptr, g_garbage->index(x, y));
  AddToOpenCloseList(x, y, NODE_OPEN);
  currentStep = 0;
  terrain->SetColor(y, x, DEBUG_COLOR_YELLOW);
}

Node AStar::CalculateCost(Node* parent, int idx)
{
  Node newNode;
  gridPos pos = g_garbage->GetGridPos(idx);

  if (parent)
  {
    //Distance vector from parent to current node
    int x = parent->x - pos.x;
    int y = parent->y - pos.y;

    float dist = 1;
    if (x == 0 || y == 0)
      dist = 1;
    else
      dist = 1.41421356237f;
    newNode.g = parent->g + dist;

    if (m_aStarUsesAnalysis)
    {
      newNode.g += g_terrain.GetInfluenceMapValue(pos.y, pos.x) * 20;
    }

  }
  else
  {
    newNode.g = 0;
  }

  newNode.parent = parent;
  if (m_heuristicCalc != 1)
    newNode.h = heuristicFn[m_heuristicCalc](pos, *end) * m_heuristicWeight;
  else
    newNode.h = precomputedHeuristic[idx * MAX_MAPSIZE + endIdx] * m_heuristicWeight;

  return newNode;
}

bool AStar::Step(std::list<D3DXVECTOR3>& waypoint)
{
  while (openListSize != 0)
  {
    Node* parent = nullptr;
    ////////////////////////////////////////////////////
    //pop cheapest node in open list
    ///////////////////////////////////////////////////
    float cheapest = std::numeric_limits<float>::infinity();
    int idx = 0;
    int selected;
    for (; idx < openListSize; idx++)
    {
      auto it = openList[idx];
      //get the node data from the garbage collector
      Node* curNode = g_garbage->Get(it);

      //devensive coding to prevent nullptr operations
      Debug::DebugPrint(curNode == nullptr,"Step: Pop cheapest node, garbage array size : %d, error access index %d",g_garbage->size,it);
      
      //calculate current cost
      float currentCost = curNode->g + curNode->h;
      //replace cheapset node  if current cost is cheaper
      if (cheapest > currentCost)
      {
        cheapest = currentCost;
        parent = curNode;
        selected = idx;
      }
    }
    //pop from list
    g_garbage->Get(openList[selected])->openClose = NODE_UNLISTED;
    openList[selected] = openList[openListSize - 1];
    openListSize--;
    ////////////////////////////////////////////////////
    //If node is the goal node, Get path
    ///////////////////////////////////////////////////
    if (parent == end)
    {
      GetPath(parent, waypoint);
      return true;
    }

    ////////////////////////////////////////////////////
    //For all childs
    ///////////////////////////////////////////////////
    for (auto i : child)
    {
      gridPos pos = (*parent + i);

      //Check if out of range
      if (pos.x < 0 || pos.y < 0 || pos.x > g_garbage->size - 1 || pos.y > g_garbage->size - 1)
        continue;

      //Check if diagonals
      if (std::abs(i.x) == std::abs(i.y))
      {
        bool invalidMove = false;
        //it is diagonal, check top bot left right if is wall
        invalidMove |= terrain->IsWall(parent->y + i.y, parent->x);
        invalidMove |= terrain->IsWall(parent->y, parent->x + i.x);

        if (invalidMove)
          continue;
      }

      if (terrain->IsWall(pos.y, pos.x))
        continue;

      int childIndex = g_garbage->index(pos.x,pos.y);
      Node cost = CalculateCost(parent, childIndex);

      //if unallocated
      Node* node = g_garbage->Get(childIndex);

      //if it is not even allocated, allocate, push to openlist
      if (node == nullptr)
      {
        Node* newAllocate = g_garbage->Allocate(pos.x, pos.y);
        *newAllocate = cost;
        AddToOpenCloseList(pos.x, pos.y, NODE_OPEN);
        continue;
      }

      if (node->openClose == NODE_UNLISTED)
      {
        *node = cost;
        AddToOpenCloseList(pos.x, pos.y, NODE_OPEN);
      }
      else
      {
        if (cost.h + cost.g < node->g + node->h)
        {
          *node = cost;
        }
      }
    }
    ////////////////////////////////////////////////////
    //place parent node on closed list
    ///////////////////////////////////////////////////
    AddToOpenCloseList(parent->x, parent->y, NODE_CLOSE);

    if (m_singleStep)
    {
      break;
    }
  }
  if (openListSize == 0)
    return true;
  else
    return false;
}

bool AStar::StraightLineOptimize(std::list<D3DXVECTOR3>& waypoint)
{
  gridPos* s = start;
  gridPos* e = end;
  bool wallExist = false;
  int minX, maxX, minY, maxY;
  minX = min(start->x, end->x);
  maxX = max(start->x, end->x);
  minY = min(start->y, end->y);
  maxY = max(start->y, end->y);

  for (int x = minX; x <= maxX; x++)
  {
    for (int y = minY; y <= maxY; y++)
    {
      wallExist |= terrain->IsWall(y,x);
    }
  }

  if (!wallExist)
  {
    waypoint.clear();
    waypoint.push_back(terrain->GetCoordinates(s->y, s->x));
    waypoint.push_back(terrain->GetCoordinates(e->y, e->x));
    return true;
  }
  return false;
}

void AStar::RubberBanding(Node* end)
{
  Node* pp = end->parent->parent;
  Node* p = end->parent;
  Node* c = end;

  while (pp)
  {
    int minX, maxX, minY, maxY;
    bool wallExist = false;

    minX = min(pp->x, c->x);
    maxX = max(pp->x, c->x);
    minY = min(pp->y, c->y);
    maxY = max(pp->y, c->y);

    for (int x = minX; x <= maxX; x++)
    {
      for (int y = minY; y <= maxY; y++)
      {
        wallExist |= terrain->IsWall(y, x);
      }
    }

    if (!wallExist)
    {
      c->parent = pp;
    }
    else
    {
      c = p;
    }
    p = pp;
    pp = pp->parent;
  }
}
#include <iterator>

void AStar::Smoothing(std::list<D3DXVECTOR3>& l)
{
  //add points if bigger than 1.5 (grid space -> world space)
  std::vector <Vec3> waypoints;
  std::move(l.begin(), l.end(), std::back_inserter(waypoints));

  for (unsigned i = 0; i < waypoints.size() - 1; ++i)
  {
    auto a = waypoints[i];
    auto b = waypoints[i + 1];
    float d = (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z);

    if (d > boxDistance * boxDistance)
    {
      waypoints.insert(waypoints.begin() + i + 1, (a + b) / 2);
      i = -1;
    }
  }

  l.clear(); //since the speed is not tested here, and the original waypoint using list, soooo.. pls don't mind me for this sucky code
  std::move(waypoints.begin(), waypoints.end(), std::back_inserter(l));

  std::list <Vec3> newList;
  typedef std::list<Vec3>::iterator listIterator;
  int i = 0;
  int listSize = l.size();
  for (listIterator it = l.begin(); it != l.end(); it++)
  {
    newList.push_back(*it);
    if (it == l.end())
      continue;

    if (i == listSize - 1)
    {
      break;
    }

    Vec3 p1, p2, p3, p4;
    listIterator copyit = it;
    if (listSize > 2)
    {
      if (i == 0)
      {
        p1 = *copyit;
        p2 = *copyit;
        p3 = *++copyit;
        p4 = *++copyit;
      }
      else if (i == listSize - 2)
      {
        p1 = *--copyit;
        p2 = *++copyit;
        p3 = *++copyit;
        p4 = *copyit;
      }
      else
      {
        p1 = *--copyit;
        p2 = *++copyit;
        p3 = *++copyit;
        p4 = *++copyit;
      }
    }
    else
    {
      p1 = *copyit;
      p2 = *copyit;
      p3 = *++copyit;
      p4 = *copyit;
    }
    for (int i = 1; i <= 3; ++i)
    {
      float s = 0.25f * i;
      Vec3 newPoint;
      newPoint = p1 * (-0.5f * s * s * s    +           s * s    - 0.5f * s ) +
                 p2 * (1.5f  * s * s * s    + (-2.5f) * s * s    + 1.0f     ) +
                 p3 * (-1.5f * s * s * s    + 2       * s * s    + 0.5f * s ) +
                 p4 * (0.5f * s * s * s     - 0.5f    * s * s               );

      newList.push_back(newPoint);
    }

    ++i;
  }

  l = newList;
}

void AStar::GetPath(Node* end, std::list<D3DXVECTOR3>& l)
{
  if (m_rubberband)
  {
    RubberBanding(end);

  }

  Node* cur = end;
  l.clear();
  while (cur)
  {
    l.push_front(terrain->GetCoordinates(cur->y, cur->x));
    cur = cur->parent;
  }

  if (m_smooth)
  {
    Smoothing(l);
  }
}

//void AStar::RemoveFromList(int x, int y)
//{
//
//  Node* addThis = g_garbage->Get(x, y);
//  ////addThis != nullptr
//  //Debug::DebugPrint(addThis == nullptr, "RemoveFromList: invalid x y index");
//  ////current node is not part of open or close list
//  //Debug::DebugPrint(addThis->openClose == NODE_UNLISTED,"RemoveFromList: doesnt exist in anylist");
//
//  ///////////////////////
//  //Removing From list
//  //////////////////////
//  if (addThis->openClose == NODE_OPEN)
//    openList.erase(g_garbage->index(x, y));
//  //else
//  //  closeList.erase(g_garbage->index(x, y));
//
//  //Changing current status
//  addThis->openClose = NODE_UNLISTED;
//  g_garbage->DumpOpenClose();
//}

void AStar::ClearOpenList()
{
  openListSize = 0;
}

bool AStar::AddToOpenCloseList(int x, int y, NodeOpeClose stat)
{
  bool Error = false; 
  DebugDrawingColor color;
  //std::unordered_set<int>* list = nullptr;
  //stat == NODE_OPEN ? list = &openList : list = nullptr;
  stat == NODE_OPEN ? color = DEBUG_COLOR_BLUE : color = DEBUG_COLOR_YELLOW;
  //if (list)
  //{
  //  //Check this node existed in open/close list
  //  std::unordered_set<int>::iterator it;
  //  it = list->find(GarbageCollector::index(x, y));
  //  Debug::DebugPrint(Error = (it != list->end()), "AddToOpenCloseList: this node is already existed in List!");
  //  if (Error) return false;
  //}


  //Check if already added to any list
  Node* addThis = g_garbage->Get(x, y);
  Debug::DebugPrint(Error = (addThis == nullptr), "AddToOpenCloseList: Existed in list!");
  if(Error) return false;

  //Check if already added to any list
  Debug::DebugPrint(Error = (addThis->openClose == NODE_CLOSE || addThis->openClose == NODE_OPEN) , "AddToOpenCloseList: Existed in list!");
  if (Error) return false;
  
  if(!g_terrain.IsWall(y,x))
    terrain->SetColor(y, x, color);

  if (stat == NODE_OPEN)
  {
    //list->insert(GarbageCollector::index(x, y));
    openList[openListSize] = GarbageCollector::index(x, y);
    openListSize++;
  }

  addThis->openClose = stat;
  g_garbage->DumpOpenClose();
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// GARBAGE COLLECTOR
////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned GarbageCollector::size;

GarbageCollector::GarbageCollector()
{
  size = terrain->GetWidth();
  //size = 20 ;

  nodes = new Node[size * size];
  DumpMemory();
  for (int y = 0; y < size; ++y)
  {
    for (int x = 0; x < size; ++x)
    {
      nodes[index(x, y)].x = x;
      nodes[index(x, y)].y = y;
    }
  }
}

GarbageCollector::~GarbageCollector()
{
  delete [] nodes;
}

void GarbageCollector::Reset()
{
  size = terrain->GetWidth();
  delete [] nodes;
  nodes = new Node[size * size];
  DumpMemory();
  for (int y = 0; y < size; ++y)
  {
    for (int x = 0; x < size; ++x)
    {
      nodes[index(x, y)].x = x;
      nodes[index(x, y)].y = y;
    }
  }

}

void GarbageCollector::Clear()
{
  for (int i = 0; i < size * size; ++i)
  {
    nodes[i].openClose = NODE_UNLISTED;
    nodes[i].stat = NODE_UNUSED;
    nodes[i].parent = nullptr;
  }
  DumpMemory();
}

Node* GarbageCollector::Allocate(int x, int y)
{
  //Check index out of range error
  bool Error = x > (int)size - 1 || y > (int)size - 1 || x < 0 || y < 0;
  Debug::DebugPrint(Error, "Allocate: indexOut of range xy :%d %d,size: %d", x, y,size);

  if (Error)
    return nullptr;

  Node* allocate = &nodes[index(x,y)];
  Debug::DebugPrint(Error = (allocate->stat == NODE_USED), "Allocate: index is allocated", x, y, size);
  if (Error)
    return nullptr;

  ///////////////////////
  //Allocating
  //////////////////////
  allocate->stat = NODE_USED;
  //DumpMemory();
  return allocate;

}

bool GarbageCollector::Deallocate(int x, int y)
{
  if (x > (int)size || y > (int)size || x < 0 || y < 0)
    return false;

  Node* deallocate = &nodes[index(x, y)];
  if (deallocate->stat == NODE_USED)
  {
    deallocate->stat = NODE_UNUSED;
    //DumpMemory();
    return true;
  }
  else
  {
    return false;
  }
}

Node* GarbageCollector::Get(int x, int y)
{
  if (x > (int)size-1 || y >(int)size-1 || x < 0 || y < 0)
    return nullptr;

  Node* getThis = &nodes[index(x,y)];
  if (getThis->stat == NODE_USED)
  {
    return getThis;
  }
  else
  {
    return nullptr;
  }
}

Node* GarbageCollector::Get(int index)
{
  if (index > (unsigned)(size * size) - 1 || index < 0)
  {
    Debug::DebugPrint(true, "invalid index %d",index);
    return nullptr;
  }

  Node* getThis = &nodes[index];
  if (getThis->stat == NODE_USED)
  {
    return getThis;
  }
  else
  {
    return nullptr;
  }
}

gridPos GarbageCollector::GetGridPos(int index)
{
  /*if (index > (unsigned)(size * size) - 1 || index < 0)
  {
    Debug::DebugPrint(true, "invalid index %d", index);
    return gridPos(-1,-1);
  }

  int x, y;
  y = index / g_garbage->size;
  x = index % g_garbage->size;

  return gridPos(x, y);*/

  return *&nodes[index];
}

Node* GarbageCollector::GetDetails(int x, int y)
{
  return &nodes[index(x, y)];
}

int GarbageCollector::index(int _x, int _y)
{
  unsigned x, y;
  x = _x;
  y = _y;

  if (x > (unsigned)size || y > (unsigned)size || x < 0 || y < 0)
    return -1;

  if (x > size)
    x = size;
  if (x < 0)
    x = 0;

  if (y > size)
    y = size;
  if (y < 0)
    y = size;


  return y * size + x;
}

void GarbageCollector::DumpMemory()
{
#ifdef DEBUG_GARBAGEDUMP
  for (int y = 0; y < size; ++y)
  {
    std::cout << std::endl;
    for (int x = 0; x < size; ++x)
    {
      std::string dumpThis;
      if(nodes[index(x, y)].stat == NODE_USED)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x04);
      else
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x08);


      nodes[index(x, y)].stat == NODE_USED ? dumpThis = "1" : dumpThis = "0";
      std::cout << dumpThis << "    ";
    }
  }
  std::cout << std::endl;
#endif
}

void GarbageCollector::DumpOpenClose()
{
#ifdef DEBUG_OPENCLOSEDUMP
  for (unsigned y = 0; y < size; ++y)
  {
    std::cout << std::endl;
    for (unsigned x = 0; x < size; ++x)
    {
      std::cout << nodes[index(x, y)].openClose << "   ";
    }
  }
  std::cout << std::endl;
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
// HELPER
////////////////////////////////////////////////////////////////////////////////////////////////////////
float HeuristicEucledean(gridPos cur, gridPos target)
{
  int x = (cur.x - target.x);
  int y = (cur.y - target.y);
  return std::sqrt(x*x + y*y);
}

float HeuristicOctile(gridPos cur, gridPos target)
{
  float xDiff = abs(target.x - cur.x);
  float yDiff = abs(target.y - cur.y);
  float cmin = min(xDiff, yDiff);
  float cmax = max(xDiff, yDiff);

  return cmin * 1.41421356237f + cmax - cmin;
}

float HeuristicChebyshev(gridPos cur, gridPos target)
{
  return max(abs(target.x - cur.x), abs(target.y - cur.y));
}

float HeuristicManhattan(gridPos cur, gridPos target)
{
  return abs(target.x - cur.x) + abs(target.y - cur.y);
}


bool operator<(const gridPos & lhs, const gridPos & rhs)// lhs = left-hand side
                                                         // rhs = right-hand side
{
  if (lhs.x != rhs.x)
  {
    return lhs.x < rhs.x;
  }
  else
  {
    return lhs.y < rhs.y;
  }
}

gridPos operator+(const gridPos & lhs, const gridPos & rhs)
{
  gridPos returnThis;

  returnThis.x = lhs.x + rhs.x;
  returnThis.y = lhs.y + rhs.y;

  if (returnThis.x > (unsigned)g_garbage->size - 1 || returnThis.x < 0)
    returnThis.x = -1;

  if (returnThis.y >(unsigned)g_garbage->size - 1 || returnThis.y < 0)
    returnThis.y = -1;

  return returnThis;
}



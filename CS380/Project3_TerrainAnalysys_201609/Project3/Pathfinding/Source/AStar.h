#pragma once
#include <unordered_set>
#include <stdio.h>
#include <stdarg.h>
#include <cmath> 

//#define TEST
//#define DEBUG_GARBAGEDUMP
//#define DEBUG_OPENCLOSEDUMP
//#define DEBUG_BREAKWARNING
struct gridPos;

typedef D3DXVECTOR3 Vec3;
typedef float(*HeuristicCost)(gridPos cur, gridPos target);
float HeuristicEucledean(gridPos cur, gridPos target);
float HeuristicOctile(gridPos cur, gridPos target);
float HeuristicChebyshev(gridPos cur, gridPos target);
float HeuristicManhattan(gridPos cur, gridPos target);

#define MAX_MAPSIZE 1600
////////////////////////////////////////////////////////////////////////////////////////////////////////
// AStar
////////////////////////////////////////////////////////////////////////////////////////////////////////

enum NodeStatus
{
  NODE_UNUSED,
  NODE_USED
};

enum NodeOpeClose
{
  NODE_UNLISTED,
  NODE_OPEN,
  NODE_CLOSE
};

struct gridPos
{
  gridPos() : x(), y() {}
  gridPos(int x, int y) : x(x), y(y) {}
  int x, y;
};

bool operator<(const gridPos & lhs, const gridPos & rhs);
gridPos operator+(const gridPos & lhs, const gridPos & rhs);


struct Node : gridPos
{
  NodeStatus stat = NODE_UNUSED;
  NodeOpeClose openClose = NODE_UNLISTED;
  float g, h;
  Node* parent;
  void operator=( const Node & rhs);

};

class AStar
{
public:
  AStar();
  ~AStar();

  void Init();
  void Reset(int x, int y, int endX, int endY);
  bool Step(std::list<D3DXVECTOR3>& waypoint);

  bool m_smooth;
  bool m_rubberband; //done
  //bool m_straightline; //done
  bool m_singleStep; //done
  bool m_extracredit;
  bool m_aStarUsesAnalysis; // done
  bool m_fogOfWar;
  float m_heuristicWeight;//done
  int m_heuristicCalc; //done

  bool StraightLineOptimize(std::list<D3DXVECTOR3>& waypoint);
private:
  void RubberBanding(Node* end);
  void Smoothing(std::list<D3DXVECTOR3>& l);
  Node CalculateCost(Node* parent, int pos);
  void PreComputeOctile();
  bool AddToOpenCloseList(int x, int y, NodeOpeClose stat);
  void ClearOpenList();
  void GetPath(Node* end, std::list<D3DXVECTOR3>& l);


  bool initialized = false;
  int openList[MAX_MAPSIZE];
  float precomputedHeuristic[MAX_MAPSIZE * MAX_MAPSIZE];

  unsigned openListSize;
  //std::unordered_set<int> closeList;

  unsigned currentStep = 0;
  Node* start;
  Node* end;
  int endIdx;
  const gridPos child[8] = { gridPos(-1,1),gridPos(0,1),gridPos(1,1),
                             gridPos(-1,0),             gridPos(1,0),
                             gridPos(-1,-1),gridPos(0,-1),gridPos(1,-1) };
  HeuristicCost heuristicFn[4];

};

////////////////////////////////////////////////////////////////////////////////////////////////////////
// GARBAGE COLLECTOR
////////////////////////////////////////////////////////////////////////////////////////////////////////

class GarbageCollector
{
public:
  GarbageCollector();
  ~GarbageCollector();
  static int index(int x, int y);

  Node* Allocate(int x, int y);
  bool Deallocate(int x, int y);
  Node* Get(int x, int y);
  Node* Get(int index);
  Node* GetDetails(int x, int y);
  gridPos GetGridPos(int index);
  void Clear();

  void Reset();
  void DumpMemory();
  void DumpOpenClose();

  static unsigned size;
  Node* nodes;

private:

  bool initialized = false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Debugging
////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Debug
{
  static void DebugPrint(bool error, const char *fmt, ...)
  {
    if (error)
    {
#ifdef _DEBUG 
      va_list args;
      va_start(args, fmt);
      vprintf(fmt, args);
      va_end(args);
      std::cout << std::endl;
  #ifdef DEBUG_BREAKWARNING
      __debugbreak();
  #endif

#endif 
    }
  }

};

extern GarbageCollector* g_garbage;

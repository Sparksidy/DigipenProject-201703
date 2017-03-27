// Author: Chi-Hao Kuo
// Updated: 2/20/2017

/* Copyright Steve Rabin, 2012.
* All rights reserved worldwide.
*
* This software is provided "as is" without express or implied
* warranties. You may freely copy and compile this source into
* applications you distribute provided that the copyright text
* below is included in the resulting source code, for example:
* "Portions Copyright Steve Rabin, 2012"
*/

#include <Stdafx.h>
/* Copyright Steve Rabin, 2012.
* All rights reserved worldwide.
*
* This software is provided "as is" without express or implied
* warranties. You may freely copy and compile this source into
* applications you distribute provided that the copyright text
* below is included in the resulting source code, for example:
* "Portions Copyright Steve Rabin, 2012"
*/
#include <fstream>

#include <Stdafx.h>
#include <limits>
#include <cstddef>

void printVec2(int x, int y)
{
  std::cout << x << " " << y << std::endl;
}

//bool Terrain::Test(int r0, int c0, int r1, int c1)
//{
//  return false;
//}
//
//
//void Terrain::Update()
//{
//  std::ifstream infile("testInput.txt");
//  int a, b;
//  int counter = 0;
//  while (infile >> a >> b)
//  {
//    std::cout << m_terrainInfluenceMap[b][a] << std::endl;
//  }
//}

float Terrain::ClosestWall(int y, int x)
{

  //m_terrain[row][col]
  bool wallFound = false;
  int maxIndex = 0;
  float closest = std::numeric_limits<float>::infinity();

  //while wall is not found
  while (!wallFound)
  {
    //check outer circle
    for (int i = 0; i <= maxIndex; ++i)
    {
      D3DXVECTOR2 cur = D3DXVECTOR2(i, maxIndex);
      float dist = D3DXVec2LengthSq(&cur);
      if (IsWall(y + i, x + maxIndex) ||
        IsWall(y - i, x - maxIndex) ||
        IsWall(y + maxIndex, x - i) ||
        IsWall(y - maxIndex, x + i))
      {
        if (closest > dist)
          closest = dist;
        wallFound = true;
      }

    }

    maxIndex++;
  }

  return (closest);
}

unsigned int bitCount(unsigned int value) {
  unsigned int count = 0;
  while (value > 0) {           // until all bits are zero
    if ((value & 1) == 1)     // check lower bit
      count++;
    value >>= 1;              // shift bits, removing lower bit
  }
  return count;
}
//
//float Terrain::RearCoverValue(int y, int x)
//{
//  //TODO: Implement this for the Terrain Analysis project.
//  //Note: A cardinal wall is a wall to the East, West, North, or South of (row, col).
//  //Note: The sides of the map are considered walls.
//  //Being surrounded by 0 cardinal and 0 diagonal walls -> return 0.0f.
//  //Being surrounded by 0 cardinal walls and 1 or more diagonal walls -> return 0.05f.
//  //Being surrounded by 1 cardinal wall and any number of diagonal walls -> return 0.10f.
//  //Being surrounded by 2 non-adjacent cardinal walls and less than 2 diagonal walls -> return 0.20f.
//  //Being surrounded by 2 non-adjacent cardinal walls and 2 or more diagonal walls -> return 0.25f.
//  //Being surrounded by 2 adjacent cardinal walls and no adjacent diagonal walls (disregard between cardinal walls) -> return 0.25f.
//  //Being surrounded by 2 adjacent cardinal walls and 1 adjacent diagonal wall (not between cardinal walls) -> return 0.40f.
//  //Being surrounded by 2 adjacent cardinal walls and 2 adjacent diagonal walls (not between cardinal walls) -> return 0.60f.
//  //Being surrounded by 3 of more cardinal walls -> return 1.0f.
//  //WRITE YOUR CODE HERE
//  const unsigned topLeft = 128;
//  const unsigned top = 64;
//  const unsigned topRight = 32;
//  const unsigned left = 16;
//  const unsigned right = 8;
//  const unsigned botLeft = 4;
//  const unsigned bot = 2;
//  const unsigned botRight = 1;
//
//  const unsigned cardinalWalls = top | bot | left | right;
//  const unsigned diagonalWalls = topLeft | topRight | botLeft | botRight;
//  const unsigned nonAdjacentVertical = top | bot;
//  const unsigned nonAdjacentHorizontal = left | right;
//  const unsigned AdjacentTopRight = top | right;
//  const unsigned AdjacentTopLeft = top | left;
//  const unsigned AdjacentBotRight = bot | right;
//  const unsigned AdjacentBotLeft = bot | left;
//  const unsigned AdjacentDiagonal1 = topLeft | topRight;
//  const unsigned AdjacentDiagonal2 = topLeft | botLeft;
//  const unsigned AdjacentDiagonal3 = botRight | topRight;
//  const unsigned AdjacentDiagonal4 = botRight | botLeft;
//
//  unsigned walls = 0;
//
//  //index 0 as nonDiagonal, index 1 as diagonal
//  int wallCount[2] = { 0,0 };
//
//  if (IsWall(y, x + 1))
//  {
//    walls |= right;
//  }
//  if (IsWall(y, x - 1))
//  {
//    walls |= left;
//  }
//  if (IsWall(y + 1, x))
//  {
//    walls |= top;
//  }
//  if (IsWall(y - 1, x))
//  {
//    walls |= bot;
//  }
//
//  if (IsWall(y + 1, x + 1))
//  {
//    walls |= topRight;
//  }
//
//  if (IsWall(y - 1, x - 1))
//  {
//    walls |= botLeft;
//  }
//  if (IsWall(y + 1, x - 1))
//  {
//    walls |= topLeft;
//  }
//  if (IsWall(y - 1, x + 1))
//  {
//    walls |= botRight;
//  }
//
//  //Done//Being surrounded by 0 cardinal and 0 diagonal walls -> return 0.0f.
//  //done//Being surrounded by 0 cardinal walls and 1 or more diagonal walls -> return 0.05f.
//  //done//Being surrounded by 1 cardinal wall and any number of diagonal walls -> return 0.10f.
//  //Being surrounded by 2 non-adjacent cardinal walls and less than 2 diagonal walls -> return 0.20f.
//  //Being surrounded by 2 non-adjacent cardinal walls and 2 or more diagonal walls -> return 0.25f.
//  //Being surrounded by 3 of more cardinal walls -> return 1.0f.
//  if (bitCount(walls & cardinalWalls) >= 3)
//    return 1.0f;
//
//  if (bitCount(walls) == 0)
//    return 0;
//  if (bitCount(walls & cardinalWalls) == 0 && bitCount(walls & diagonalWalls) > 0)
//    return 0.05f;
//  if (bitCount(walls & cardinalWalls) == 1)
//    return 0.1f;
//  unsigned test = walls & nonAdjacentVertical;
//  if (bitCount(walls & cardinalWalls) == 2 && //2 walls
//    ((walls & nonAdjacentVertical) == nonAdjacentVertical || (walls & nonAdjacentHorizontal) == nonAdjacentHorizontal) // it's non adjacent 2 walls
//    )
//  {
//    unsigned diagonalCount = bitCount(walls & diagonalWalls);
//    if (diagonalCount < 2)
//      return 0.2f;
//    else
//      return 0.25f;
//  }
//  //Being surrounded by 2 adjacent cardinal walls 
//  if (bitCount(walls & cardinalWalls) == 2 &&
//    (walls & AdjacentTopRight) == AdjacentTopRight ||
//    (walls & AdjacentTopLeft) == AdjacentTopLeft ||
//    (walls & AdjacentBotRight) == AdjacentBotRight ||
//    (walls & AdjacentBotLeft) == AdjacentBotLeft)
//  {
//    //  and no adjacent diagonal walls(disregard between cardinal walls) -> return 0.25f.
//    if (bitCount(walls & diagonalWalls) == 0)
//    {
//      return 0.25f;
//    }
//    int adjacentDiagonal = 0;
//    if ((walls & AdjacentTopRight) == AdjacentTopRight)
//    {
//      adjacentDiagonal = walls & (topLeft | botRight);
//    }
//    if ((walls & AdjacentTopLeft) == AdjacentTopLeft)
//    {
//      adjacentDiagonal = walls & (topRight | botLeft);
//    }
//    if ((walls & AdjacentBotRight) == AdjacentBotRight)
//    {
//      adjacentDiagonal = walls & (topRight | botLeft);
//    }
//    if ((walls & AdjacentBotLeft) == AdjacentBotLeft)
//    {
//      adjacentDiagonal = walls & (topLeft | botRight);
//    }
//    if (bitCount(adjacentDiagonal) == 0)
//      return 0.25f;
//    //  and 1 adjacent diagonal wall(not between cardinal walls) -> return 0.40f. 
//    if (bitCount(adjacentDiagonal) == 1)
//      return 0.4f;
//    //  and 2 adjacent diagonal walls(not between cardinal walls) -> return 0.60f. 
//    if (bitCount(adjacentDiagonal) == 2)
//      return 0.6f;
//  }
//
//  return 0.0f;	//REPLACE THIS
//}

void Terrain::AnalyzeOpennessClosestWall(void)
{
  //TODO: Implement this for the Terrain Analysis project.

  //Mark every square on the terrain (m_terrainInfluenceMap[r][c]) with
  //the value 1/(d*d), where d is the distance to the closest wall in 
  //row/column grid space.
  //Edges of the map count as walls!
  for (int i = 0; i < m_width; ++i)
    for (int j = 0; j < m_width; ++j)
    {
      m_terrainInfluenceMap[i][j] = 1 / ClosestWall(i, j);
    }

  //WRITE YOUR CODE HERE

}

void Terrain::AnalyzeVisibility(void)
{
  //TODO: Implement this for the Terrain Analysis project.

  //Mark every square on the terrain (m_terrainInfluenceMap[r][c]) with
  //the number of grid squares (that are visible to the square in question)
  //divided by 160 (a magic number that looks good). Cap the value at 1.0.

  //Two grid squares are visible to each other if a line between 
  //their centerpoints doesn't intersect the four boundary lines
  //of every walled grid square. Put this code in IsClearPath().

  //WRITE YOUR CODE HERE
  for (int curY = 0; curY < GetWidth(); ++curY)
  {
    for (int curX = 0; curX < GetWidth(); ++curX)
    {
      int visibleGrid = 0;
      float visibility = 0;

      //for every single grid, check visibleGrid Count
      for (int y = 0; y < GetWidth(); ++y)
        for (int x = 0; x < GetWidth(); ++x)
          if (IsClearPath(curY, curX, y, x) && !IsWall(y, x))
            visibleGrid++;

      //set visibility as 1/160 * visibleGrid;
      visibility = visibleGrid / 160.0f;
      m_terrainInfluenceMap[curY][curX] = visibility > 1.0f ? 1.0f : visibility;
    }
  }
}
//
//void Terrain::AnalyzeRearCoverWithHighVisibility(void)
//{
//  //TODO: Implement this for the Terrain Analysis project.
//
//  //Multiply the results from the AnalyzeOpennessRearCover function
//  //and the AnalyzeVisibility function.
//
//  //WRITE YOUR CODE HERE
//  int** rearCover = new int*[m_width];
//
//  for (int i = 0; i < m_width; ++i)
//    rearCover[i] = new int[m_width];
//
//  for (int r = 0; r<m_width; r++)
//  {
//    for (int c = 0; c<m_width; c++)
//    {
//      if (!IsWall(r, c))
//      {
//        rearCover[r][c] = RearCoverValue(r, c);
//      }
//    }
//  }
//  AnalyzeVisibility();
//  for (int r = 0; r < m_width; r++)
//  {
//    for (int c = 0; c < m_width; c++)
//    {
//      m_terrainInfluenceMap[r][c] *= rearCover[r][c];
//    }
//  }
//}

void Terrain::AnalyzeVisibleToPlayer(void)
{
  //TODO: Implement this for the Terrain Analysis project.

  //For every square on the terrain (m_terrainInfluenceMap[r][c])
  //that is visible to the player square, mark it with 1.0.
  //For all non 1.0 squares that are visible and next to 1.0 squares,
  //mark them with 0.5. Otherwise, the square should be marked with 0.0.

  //Two grid squares are visible to each other if a line between 
  //their centerpoints doesn't intersect the four boundary lines
  //of every walled grid square. Put this code in IsClearPath().
  for (int y = 0; y < m_width; ++y)
  {
    for (int x = 0; x < m_width; ++x)
    {
      m_terrainInfluenceMap[y][x] = 0.0f;
    }
  }

  for (int y = 0; y < m_width; ++y)
  {
    for (int x = 0; x < m_width; ++x)
    {
      m_terrainInfluenceMap[y][x] = 0.0f;

      if (IsClearPath(y, x, m_rPlayer, m_cPlayer))
      {
        m_terrainInfluenceMap[y][x] = 1.0f;
      }
    }
  }

  for (int y = 0; y < m_width; ++y)
  {
    for (int x = 0; x < m_width; ++x)
    {
      int maxX = x + 1;
      int minX = x - 1;
      int maxY = y + 1;
      int minY = y - 1;

      maxX >= m_width ? maxX = x : maxX = maxX;
      maxY >= m_width ? maxY = y : maxY = maxY;
      minX < 0 ? minX = x : maxX = maxX;
      minY < 0 ? minY = y : minY = minY;

      unsigned left = 1;
      unsigned top = 2;
      unsigned right = 4;
      unsigned bot = 8;
      unsigned leftTop = left | top;
      unsigned leftBot = left | bot;
      unsigned rightTop = right | top;
      unsigned rightBot = right | bot;

      int cardinalWalls = 0;
      int influenceMap = 0;
      if (IsWall(y + 1, x))
        cardinalWalls |= top;
      if (IsWall(y - 1, x))
        cardinalWalls |= bot;
      if (IsWall(y, x + 1))
        cardinalWalls |= right;
      if (IsWall(y, x - 1))
        cardinalWalls |= left;

      int visibleCount = 0;
      if (m_terrainInfluenceMap[maxY][minX] == 1.0f)
      {
        visibleCount++;
      }
      if (m_terrainInfluenceMap[maxY][x] == 1.0f)
      {
        visibleCount++;
      }
      if (m_terrainInfluenceMap[maxY][maxX] == 1.0f)
      {
        visibleCount++;
      }
      if (m_terrainInfluenceMap[y][minX] == 1.0f)
      {
        visibleCount++;
      }
      if (m_terrainInfluenceMap[y][x + 1] == 1.0f)
      {
        visibleCount++;
      }
      if (m_terrainInfluenceMap[minY][minX] == 1.0f)
      {
        visibleCount++;
      }
      if (m_terrainInfluenceMap[minY][x] == 1.0f)
      {
        visibleCount++;
      }
      if (m_terrainInfluenceMap[minY][maxX] == 1.0f)
      {
        visibleCount++;
      }


      if (
        visibleCount > 0 && m_terrainInfluenceMap[y][x] == 0
        )
      {
        m_terrainInfluenceMap[y][x] = 0.5f;
        if ((cardinalWalls & leftTop) == leftTop)
        {
          if (m_terrainInfluenceMap[maxY][minX] == 1.0 && visibleCount == 1)
          {
            m_terrainInfluenceMap[y][x] = 0.f;
          }
        }
        if ((cardinalWalls & rightTop) == rightTop)
        {
          if (m_terrainInfluenceMap[maxY][maxX] == 1.0 && visibleCount == 1)
          {
            m_terrainInfluenceMap[y][x] = 0.f;
          }
        }
        if ((cardinalWalls & leftBot) == leftBot)
        {
          if (m_terrainInfluenceMap[minY][minX] == 1.0 && visibleCount == 1)
          {
            m_terrainInfluenceMap[y][x] = 0.f;
          }
        }
        if ((cardinalWalls & rightBot) == rightBot)
        {
          if (m_terrainInfluenceMap[minY][maxX] == 1.0 && visibleCount == 1)
          {
            m_terrainInfluenceMap[y][x] = 0.f;
          }
        }
      }
    }
  }

}

void Terrain::AnalyzeSearch(void)
{
  //TODO: Implement this for the Terrain Analysis project.

  //For every square on the terrain (m_terrainInfluenceMap[r][c])
  //that is visible by the player square, mark it with 1.0.
  //Otherwise, don't change the value (because it will get
  //decremented with time in the update call).
  //You must consider the direction the player is facing:
  //D3DXVECTOR2 playerDir = D3DXVECTOR2(m_dirPlayer.x, m_dirPlayer.z)
  //Give the player a field of view a tad greater than 180 degrees.

  //Two grid squares are visible to each other if a line between 
  //their centerpoints doesn't intersect the four boundary lines
  //of every walled grid square. Put this code in IsClearPath().

  //WRITE YOUR CODE HERE
  D3DXVECTOR2 playerDir = D3DXVECTOR2(m_dirPlayer.x, m_dirPlayer.z);
  D3DXVec2Normalize(&playerDir, &playerDir);
  float viewPlaneDistance = D3DXVec2Dot(&playerDir, &D3DXVECTOR2(m_cPlayer, m_rPlayer));

  for (int y = 0; y < m_width; ++y)
  {
    for (int x = 0; x < m_width; ++x)
    {
      //using point plane projection theory
      float DistToPlane = D3DXVec2Dot(&playerDir, &D3DXVECTOR2(x, y)) - viewPlaneDistance;
      if (DistToPlane > -0.01f) // if it's = 0, its on the plane, > 0 out side the plane, < 0 inside the plane(out of the vision)
      {
        if (IsClearPath(y, x, m_rPlayer, m_cPlayer))
        {
          m_terrainInfluenceMap[y][x] = 1.0f;
        }
      }
    }
  }
}

bool Terrain::intersectGrid(int y, int x, D3DXVECTOR2 rayStart, D3DXVECTOR2 rayEnd)
{
  D3DXVECTOR2 AABBmin = D3DXVECTOR2(x - 0.025f, y - 0.025f);
  D3DXVECTOR2 AABBmax = D3DXVECTOR2(x + 1 + 0.025f, y + 1 + +0.025f);
  D3DXVECTOR2 gridEdge[4][2] =
  {
    { D3DXVECTOR2(AABBmin.x,AABBmin.y),D3DXVECTOR2(AABBmax.x,AABBmin.y) },
    { D3DXVECTOR2(AABBmin.x,AABBmin.y),D3DXVECTOR2(AABBmin.x,AABBmax.y) },
    { D3DXVECTOR2(AABBmax.x,AABBmax.y),D3DXVECTOR2(AABBmin.x,AABBmax.y) },
    { D3DXVECTOR2(AABBmax.x,AABBmax.y),D3DXVECTOR2(AABBmax.x,AABBmin.y) },
  };

  bool line1 = LineIntersect(gridEdge[0][0].x, gridEdge[0][0].y, gridEdge[0][1].x, gridEdge[0][1].y, rayStart.x + 0.5f, rayStart.y + 0.5f, rayEnd.x + 0.5f, rayEnd.y + 0.5f);
  bool line2 = LineIntersect(gridEdge[1][0].x, gridEdge[1][0].y, gridEdge[1][1].x, gridEdge[1][1].y, rayStart.x + 0.5f, rayStart.y + 0.5f, rayEnd.x + 0.5f, rayEnd.y + 0.5f);
  bool line3 = LineIntersect(gridEdge[2][0].x, gridEdge[2][0].y, gridEdge[2][1].x, gridEdge[2][1].y, rayStart.x + 0.5f, rayStart.y + 0.5f, rayEnd.x + 0.5f, rayEnd.y + 0.5f);
  bool line4 = LineIntersect(gridEdge[3][0].x, gridEdge[3][0].y, gridEdge[3][1].x, gridEdge[3][1].y, rayStart.x + 0.5f, rayStart.y + 0.5f, rayEnd.x + 0.5f, rayEnd.y + 0.5f);

  return line1 || line2 || line3 || line4;
}

bool Terrain::IsClearPath(int y0, int x0, int y1, int x1)
{
  //TODO: Implement this for the Terrain Analysis project.

  //Two grid squares (r0,c0) and (r1,c1) are visible to each other 
  //if a line between their centerpoints doesn't intersect the four 
  //boundary lines of every walled grid square. Make use of the
  //function LineIntersect(). You should also puff out the four
  //boundary lines by a very tiny bit so that a diagonal line passing
  //by the corner will intersect it.

  bool isIntersect = false;
  D3DXVECTOR2 rayStart = D3DXVECTOR2(x0, y0);
  D3DXVECTOR2 rayEnd = D3DXVECTOR2(x1, y1);
  int startY = min(y0, y1);
  int startX = min(x0, x1);
  int endY = max(y0, y1);
  int endX = max(x0, x1);

  startY = max(0, startY);
  startX = max(0, startX);
  endX = min(m_width - 1, endX);
  endY = min(m_width - 1, endY);

  //WRITE YOUR CODE HERE
  for (int y = startY; y <= endY; ++y)
  {
    for (int x = startX; x <= endX; ++x)
    {
      if (m_terrain[y][x] == TILE_WALL || m_terrain[y][x] == TILE_WALL_INVISIBLE)
      {
        //std::cout << x << " " << y << std::endl;

        if (intersectGrid(y, x, rayStart, rayEnd))
          return false;
      }

    }
  }

  return true;
}

bool Terrain::UpdateFogOfWar(D3DXVECTOR3& pos, D3DXVECTOR3& dir)
{
  if (!isFogOfWar)
    return false;

  int r, c;
  GetRowColumn(&pos, &r, &c);

  if (m_rPlayer < 0 || m_cPlayer < 0)
  {
    m_rPlayer = r;
    m_cPlayer = c;
    m_dirPlayer = dir;
  }
  else if ((m_rPlayer != r || m_cPlayer != c))
  {
    m_rPlayer = r;
    m_cPlayer = c;
    m_dirPlayer = dir;
  }



  int prevSeenWall = curSeenWall;
  if (counter == 0)
  {
    counter++;
  }

  AnalyzeVisibleToPlayer();
  for (int y = 0; y < m_width; ++y)
  {
    for (int x = 0; x < m_width; ++x)
    {
      if (m_terrain[y][x] == TILE_WALL_INVISIBLE && m_terrainInfluenceMap[y][x] >= .5f)
      {
        m_terrain[y][x] = TILE_WALL;
        curSeenWall++;
      }
    }
  }

  return prevSeenWall != curSeenWall;
}

//Helper Function
std::vector<int> FindValidNeighbour(int x, int y, int width)
{
  //Left, Right, Top, Bottom, TopRight, TopLeft, BottomRight, BottomLeft
  std::vector<int> returnThis;

  for (int i = -1; i <= 1; ++i)
  {
    for (int j = -1; j <= 1; ++j)
    {
      if (i == 0 && j == 0)
        continue;

      int newX = x + j;
      int newY = y + i;
      if (newX < width && newX >= 0 && newY < width && newY >= 0)
      {
        //std::cout << "i = " << i << " ,j = " << j << std::endl;
        //std::cout << "x = " << newX << " ,y = " << newY << std::endl;

        returnThis.push_back(newY * width + newX);

      }
    }
  }


  return std::vector<int>();
}

void Terrain::Propagation(float decay, float growing, bool computeNegativeInfluence)
{
	// TODO: Implement this for the Occupancy Map project.

	// For this function, write result to m_terrainOccupancyMap

	// Pseudo code:
	//
	// For each tile on the map
	//
	//   Get the influence value of each neighbor after decay
	//
	//   If old influence is negative, keep the minimum neighbor influence. 
	//   If old influence is positive, keep the maximum neighbor influence. 
	//   If old influence is 0, keep the decayed influence WITH THE HIGHEST ABSOLUTE.
	//
	//   Apply linear interpolation to the influence value of the tile, 
	//   and the maximum decayed influence value from all neighbors, with growing 
	//   factor as coefficient
	//
	//   Store the result to the temp layer
	//
	// Store influence value from temp layer

	// computeNegativeInfluence flag is true if we need to handle two agents
	// (have both positive and negative influence)
	// computeNegativeInfluence flag is false if we only deal with positive
	// influence, ignore negative influence

	// WRITE YOUR CODE HERE
  // For each tile on the map
  for (int y = 0; y < m_width; ++y)
  {
    for (int x = 0; x < m_width; ++x)
    {
      if (m_terrain[y][x] != TILE_WALL)
      {
        //   Get the influence value of each neighbor after decay
        std::vector<int> neighbourIdx = FindValidNeighbour(x,y,m_width);
      }
    }
  }

}

void Terrain::NormalizeOccupancyMap(bool computeNegativeInfluence)
{
	// TODO: Implement this for the Occupancy Map project.

	// For this function, write result to m_terrainOccupancyMap

	// divide all tiles with maximum influence value, so the range of the
	// influence is kept in [0,1]
	// if we need to handle negative influence value, divide all positive
	// tiles with maximum influence value, and all negative tiles with
	// minimum influence value * -1, so the range of the influence is kept
	// at [-1,1]
	
	// computeNegativeInfluence flag is true if we need to handle two agents
	// (have both positive and negative influence)
	// computeNegativeInfluence flag is false if we only deal with positive
	// influence, ignore negative influence 

	// WRITE YOUR CODE HERE


}

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
#include <vector>

float Terrain::ClosestWall(int row, int col)
{
	//TODO: Helper function for the Terrain Analysis project (you'll likely need it).

	//Return the distance to the closest wall or side of the map.

	return 0.0f;	//REPLACE THIS
}

float Terrain::RearCoverValue(int row, int col)
{
	//TODO: Implement this for the Terrain Analysis project.

	//Note: A cardinal wall is a wall to the East, West, North, or South of (row, col).
	//Note: The sides of the map are considered walls.
	//Being surrounded by 0 cardinal and 0 diagonal walls -> return 0.0f.
	//Being surrounded by 0 cardinal walls and 1 or more diagonal walls -> return 0.05f.
	//Being surrounded by 1 cardinal wall and any number of diagonal walls -> return 0.10f.
	//Being surrounded by 2 non-adjacent cardinal walls and less than 2 diagonal walls -> return 0.20f.
	//Being surrounded by 2 non-adjacent cardinal walls and 2 or more diagonal walls -> return 0.25f.
	//Being surrounded by 2 adjacent cardinal walls and no adjacent diagonal walls (disregard between cardinal walls) -> return 0.25f.
	//Being surrounded by 2 adjacent cardinal walls and 1 adjacent diagonal wall (not between cardinal walls) -> return 0.40f.
	//Being surrounded by 2 adjacent cardinal walls and 2 adjacent diagonal walls (not between cardinal walls) -> return 0.60f.
	//Being surrounded by 3 of more cardinal walls -> return 1.0f.


	//WRITE YOUR CODE HERE


	return 0.0f;	//REPLACE THIS
}

void Terrain::AnalyzeOpennessClosestWall(void)
{
	//TODO: Implement this for the Terrain Analysis project.

	//Mark every square on the terrain (m_terrainInfluenceMap[r][c]) with
	//the value 1/(d*d), where d is the distance to the closest wall in 
	//row/column grid space.
	//Edges of the map count as walls!

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

}

void Terrain::AnalyzeRearCoverWithHighVisibility(void)
{
	//TODO: Implement this for the Terrain Analysis project.

	//Multiply the results from the AnalyzeOpennessRearCover function
	//and the AnalyzeVisibility function.


	//WRITE YOUR CODE HERE

}

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


	//WRITE YOUR CODE HERE

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

}

bool Terrain::IsClearPath(int r0, int c0, int r1, int c1)
{
	//TODO: Implement this for the Terrain Analysis project.

	//Two grid squares (r0,c0) and (r1,c1) are visible to each other 
	//if a line between their centerpoints doesn't intersect the four 
	//boundary lines of every walled grid square. Make use of the
	//function LineIntersect(). You should also puff out the four
	//boundary lines by a very tiny bit so that a diagonal line passing
	//by the corner will intersect it.


	//WRITE YOUR CODE HERE


	return true;	//REPLACE THIS
}

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

bool Movement::ComputePath( int r, int c, bool newRequest )
{
  AStar* test = &g_astar;
	m_goal = g_terrain.GetCoordinates( r, c );
	m_movementMode = MOVEMENT_WAYPOINT_LIST;
  m_waypointList.clear();
  m_waypointList.push_back(m_owner->GetBody().GetPos());

	if(true)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////
		//INSERT YOUR A* CODE HERE
		//1. You should probably make your own A* class.
		//2. You will need to make this function remember the current progress if it preemptively exits.
		//3. Return "true" if the path is complete, otherwise "false".
		///////////////////////////////////////////////////////////////////////////////////////////////////
    if (newRequest)
    {
      int x, y;
      g_terrain.GetRowColumn(&m_owner->GetBody().GetPos(), &y, &x);
      m_waypointList.clear();
      m_waypointList.push_back(m_owner->GetBody().GetPos());
      if (x == c && y == r)
        return true;

      if (g_terrain.IsWall(r, c))
        return true;

      g_astar.Reset(x, y, c, r);
      if (m_straightline)
        if (g_astar.StraightLineOptimize(m_waypointList))
          return true;
    }

    return  g_astar.Step(m_waypointList);
	}
	else
	{	
		//Randomly meander toward goal (might get stuck at wall)
		int curR, curC;
		D3DXVECTOR3 cur = m_owner->GetBody().GetPos();
		g_terrain.GetRowColumn( &cur, &curR, &curC );

		m_waypointList.clear();
		m_waypointList.push_back( cur );

		int countdown = 100;
		while( curR != r || curC != c )
		{
			if( countdown-- < 0 ) { break; }

			if( curC == c || (curR != r && rand()%2 == 0) )
			{	//Go in row direction
				int last = curR;
				if( curR < r ) { curR++; }
				else { curR--; }

				if( g_terrain.IsWall( curR, curC ) )
				{
					curR = last;
					continue;
				}
			}
			else
			{	//Go in column direction
				int last = curC;
				if( curC < c ) { curC++; }
				else { curC--; }

				if( g_terrain.IsWall( curR, curC ) )
				{
					curC = last;
					continue;
				}
			}

			D3DXVECTOR3 spot = g_terrain.GetCoordinates( curR, curC );
			m_waypointList.push_back( spot );
			g_terrain.SetColor( curR, curC, DEBUG_COLOR_YELLOW );
		}
		return true;
	}
}

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

extern D3DXVECTOR3 g_click3D;


//Add new states here
enum StateName {
	STATE_Initialize,	//Note: the first enum is the starting state
	STATE_CalcPath,
	STATE_MoveToGoal,
	STATE_Idle,
	STATE_TimingTestShort,
	STATE_TimingTestLong,
  STATE_CustomTimingTest
};

//Add new substates here
enum SubstateName {
	//empty
};

void Agent::InitFogOfWar(bool enabled)
{
	if (enabled)
		g_terrain.InitFogOfWar();
	else
		g_terrain.ClearFogOfWar();
}

bool Agent::States( State_Machine_Event event, MSG_Object * msg, int state, int substate )
{
BeginStateMachine

	//Global message responses

	OnMsg( MSG_MapChange )
		//////////////////////////////////////
		// Add code here to handle map changes
		//////////////////////////////////////
		InitFogOfWar(m_owner->GetMovement().GetFogOfWar());
    g_garbage->Reset();

	OnMsg( MSG_Reset )
		ResetStateMachine();

	OnMsg( MSG_SetHeuristicWeight )
		m_owner->GetMovement().SetHeuristicWeight( msg->GetFloatData() );

	OnMsg( MSG_SetHeuristicCalc )
		m_owner->GetMovement().SetHeuristicCalc( msg->GetIntData() );

	OnMsg( MSG_SetSmoothing )
		m_owner->GetMovement().SetSmoothPath( msg->GetBoolData() );

	OnMsg( MSG_SetRubberbanding )
		m_owner->GetMovement().SetRubberbandPath( msg->GetBoolData() );

	OnMsg( MSG_SetStraightline )
		m_owner->GetMovement().SetStraightlinePath( msg->GetBoolData() );

	OnMsg( MSG_SetAgentSpeed )
		m_animStyle = msg->GetIntData();

	OnMsg( MSG_SetSingleStep )
		m_owner->GetMovement().SetSingleStep( msg->GetBoolData() );

	OnMsg( MSG_ExtraCredit )
		m_owner->GetMovement().SetExtraCredit( msg->GetBoolData() );

	OnMsg( MSG_SetAStarUsesAnalysis )
		m_owner->GetMovement().AStarUsesAnalysis( msg->GetBoolData() );

	OnMsg( MSG_SetMoving )
		m_moving = msg->GetBoolData();

	OnMsg( MSG_RunTimingsShort )
		ChangeState( STATE_TimingTestShort );

	OnMsg( MSG_RunTimingsLong )
		ChangeState( STATE_TimingTestLong );

  OnMsg(MSG_CustomTimingTest)
    ChangeState(STATE_CustomTimingTest);

	OnMsg( MSG_SetFogOfWar )
		m_owner->GetMovement().SetFogOfWar(msg->GetBoolData());
		InitFogOfWar(msg->GetBoolData());

	OnMsg( MSG_SetGoal )
		int row = -1;
		int col = -1;
		if(g_terrain.GetRowColumn( &g_click3D, &row, &col ))
		{
			if( !g_terrain.IsWall( row, col ) )
			{ 
				g_terrain.ResetColors();
				g_terrain.SetColor( row, col, DEBUG_COLOR_BLUE );
				m_rowGoal = row;
				m_colGoal = col;
				bool foundPath = m_owner->GetMovement().ComputePathWithTiming( m_rowGoal, m_colGoal, true ); 
				if( foundPath ) 
				{
					ChangeState( STATE_MoveToGoal );
				}
				else
				{
					ChangeState( STATE_CalcPath );
				}
			}
		}


	///////////////////////////////////////////////////////////////
	DeclareState( STATE_Initialize )

		OnEnter
			m_animStyle = 1;
			m_owner->GetMovement().SetIdleSpeed();
			

	///////////////////////////////////////////////////////////////
	DeclareState(STATE_CalcPath)

		OnEnter
			m_owner->GetMovement().SetIdleSpeed();

		OnUpdate
			Movement& move = m_owner->GetMovement();
			bool foundPath = m_owner->GetMovement().ComputePathWithTiming( m_rowGoal, m_colGoal, false );
			if(foundPath)
			{
				ChangeState( STATE_MoveToGoal );
			}


	///////////////////////////////////////////////////////////////
	DeclareState( STATE_MoveToGoal )

		OnEnter
			if(!m_moving) { ChangeState( STATE_Idle ); }
			if(m_animStyle == 0) { m_owner->GetMovement().SetWalkSpeed(); }
			else if(m_animStyle == 1) { m_owner->GetMovement().SetJogSpeed(); }
			else { m_owner->GetMovement().SetWalkSpeed(); }

		OnMsg( MSG_Arrived )
			ChangeState( STATE_Idle );

		OnMsg( MSG_SetAgentSpeed )
			m_animStyle = msg->GetIntData();
			if(m_animStyle == 0) { m_owner->GetMovement().SetWalkSpeed(); }
			else if(m_animStyle == 1) { m_owner->GetMovement().SetJogSpeed(); }
			else { m_owner->GetMovement().SetWalkSpeed(); }


	///////////////////////////////////////////////////////////////
	DeclareState( STATE_Idle )

		OnEnter
			m_owner->GetMovement().SetIdleSpeed();


	///////////////////////////////////////////////////////////////
	DeclareState( STATE_TimingTestShort )

		OnEnter
			RunTimingTest(4, 18, 16, 1, 0, "TimingResultsShort.txt");
			ChangeState( STATE_Idle );


	///////////////////////////////////////////////////////////////
	DeclareState( STATE_TimingTestLong )

		OnEnter
			RunTimingTest(1, 38, 36, 1, 0, "TimingResultsLong.txt");
			ChangeState( STATE_Idle );

  ///////////////////////////////////////////////////////////////
      DeclareState(STATE_CustomTimingTest)

        OnEnter
        CustomTimingTest1();
       ChangeState(STATE_Idle);

EndStateMachine
}
void Agent::RunTimingTest(int mapIndex, int xStart, int yStart, int xGoal, int yGoal, std::string filename)
{
  Movement& movement = m_owner->GetMovement();
  int heuristic = movement.GetHeuristicCalc();
  float weight = movement.GetHeuristicWeight();
  bool step = movement.GetSingleStep();
  bool smoothing = movement.GetSmoothPath();
  bool rubberband = movement.GetRubberbandPath();
  bool straightline = movement.GetStraightlinePath();
  bool analysis = movement.GetAnalysis();
  bool fow = movement.GetFogOfWar();

  movement.SetHeuristicCalc(1); // Should be Octile
  movement.SetHeuristicWeight(1.01f);
  movement.SetSingleStep(false);
  movement.SetSmoothPath(false);
  movement.SetRubberbandPath(false);
  movement.SetStraightlinePath(false);
  movement.AStarUsesAnalysis(false);
  movement.SetFogOfWar(false);

  while (g_terrain.GetMapIndex() != mapIndex)
  {
    g_terrain.NextMap();
  }

  m_owner->GetBody().SetPos(g_terrain.GetCoordinates(xStart, yStart));

#ifdef _DEBUG
  const unsigned NUMTESTS = 100;
#else
  const unsigned NUMTESTS = 1000;
#endif
  double results[NUMTESTS];
  double resultsTime[NUMTESTS];

  g_clock.UpdateQPCFrequency();

  // Run tests
  for (int i = 0; i < NUMTESTS; ++i)
  {
    movement.ComputePathWithTiming(xGoal, yGoal, true);
    results[i] = g_clock.GetStopwatchPathfinding();
    resultsTime[i] = g_clock.GetStopwatchPathfindingTime();
  }

  // Find minimum
  double min = results[0];
  double minTime = resultsTime[0];
  for (int i = 1; i < NUMTESTS; ++i)
  {
    if (min > results[i])
    {
      min = results[i];
      minTime = resultsTime[i];
    }
  }

  // Output results
  std::ofstream out(filename.c_str());

  out << std::endl << "Minimum time:  " << min << "\t(" << minTime << " ms)" << std::endl << std::endl;
  out << "Test #\t\t" << "Time" << std::endl;

  for (int i = 0; i < NUMTESTS; ++i)
    out << i + 1 << ":\t\t" << results[i] << "\t(" << resultsTime[i] << " ms)" << std::endl;

  std::cout << std::endl << "Minimum time:  " << min << "\t(" << minTime << " ms)" << std::endl << std::endl;


  out.close();

  movement.SetHeuristicCalc(heuristic);
  movement.SetHeuristicWeight(weight);
  movement.SetSingleStep(step);
  movement.SetSmoothPath(smoothing);
  movement.SetRubberbandPath(rubberband);
  movement.AStarUsesAnalysis(analysis);
  movement.SetStraightlinePath(straightline);
  movement.SetFogOfWar(fow);
}


void Agent::CustomTimingTest()
{
  Movement& movement = m_owner->GetMovement();

  movement.SetHeuristicCalc(1); // Should be Octile
  movement.SetHeuristicWeight(1.01f);
  movement.SetSingleStep(false);
  movement.SetSmoothPath(false);
  movement.SetRubberbandPath(true);
  movement.SetStraightlinePath(true);
  movement.AStarUsesAnalysis(false);
  movement.SetFogOfWar(false);

  //Test all available path
  int mapsCount = g_terrain.GetMapSize();
  Vec3 initialPosition = m_owner->GetBody().GetPos();

  Terrain& a = g_terrain;

  for (int i = 0; i < mapsCount - 1; ++i)
  {
    g_terrain.NextMap();
    if (i < 3)
      continue;
    std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;
    std::cout << "mapID :" << i << std::endl;
    std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;

    //check all initial point
    for (int cy = 0; cy < g_terrain.GetWidth(); ++cy)
    {
      for (int cx = 0; cx < g_terrain.GetWidth(); ++cx)
      {
        std::cout << "startPos: " << cx << "," << cy << std::endl;
        if (g_terrain.IsWall(cy, cx))
          continue;
        m_owner->GetBody().SetPos(g_terrain.GetCoordinates(cy, cx));

        //Check all target
        for (int y = 0; y < g_terrain.GetWidth(); ++y)
        {
          for (int x = 0; x < g_terrain.GetWidth(); ++x)
          {
            movement.ComputePathWithTiming(y, x, true);
          }
        }//check all target end
      }
    }//check all startpoint end
  }//check all map end

  RunTimingTest(1, 38, 36, 1, 0, "TimingResultsLong.txt");
}

void Agent::CustomTimingTest1()
{
  for (int i = 0; i < 10; ++i)
  {
    RunTimingTest(1, 38, 36, 1, 0, "TimingResultsLong.txt");
  }

  RunTimingTest(1, 38, 36, 1, 0, "TimingResultsLong.txt");

}






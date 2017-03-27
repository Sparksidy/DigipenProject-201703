/******************************************************************************/
/*!
\file		Blackboard.cpp
\project	CS380/CS580 AI Framework
\author		Chi-Hao Kuo
\summary	Global blackboard.

Copyright (C) 2016 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <Stdafx.h>

BlackBoard::BlackBoard()
	: m_rPlayer(-1), m_cPlayer(-1), m_rStart(-1), m_cStart(-1), m_rGoal(-1), m_cGoal(-1),
	m_terrainAnalysisFlag(false), m_terrainAnalysisType(0), m_occupancyMapFlag(false), m_occupancyMapType(0),
	m_decayFactor(DEFAULT_DECAY), m_growFactor(DEFAULT_GROWING), m_updateFrequency(DEFAULT_UPDATEFREQUENCY),
	m_singleStep(true), m_previousSingleStep(true), m_fovAngle(DEFAULT_FOVANGLE), 
	m_fovCloseDistance(DEFAULT_FOVCLOSEDISTANCE), m_seekPlayer(false)
{

}

void BlackBoard::UpdatePlayerPos(int r, int c)
{
	m_rPlayer = r;
	m_cPlayer = c;
}

void BlackBoard::UpdateStartPos(void)
{
	m_rStart = m_rPlayer;
	m_cStart = m_cPlayer;
}

void BlackBoard::UpdateGoalPos(int r, int c)
{
	m_rGoal = r;
	m_cGoal = c;
}

void BlackBoard::SetTerrainAnalysisType(int type)
{
#if defined(_DEBUG)
	ASSERTMSG(type >= 0 && type < TerrainAnalysis_Count, "Invalid Type");
#endif

	m_terrainAnalysisType = type;
}

void BlackBoard::IncTerrainAnalysisType(void)
{
	if (++m_terrainAnalysisType >= TerrainAnalysis_Count)
		m_terrainAnalysisType = 0;

	if (g_blackboard.GetTerrainAnalysisFlag())
	{
		g_terrain.SetTerrainAnalysis();
	}
}

void BlackBoard::SetOccupancyMapType(int type)
{
#if defined(_DEBUG)
	ASSERTMSG(type >= 0 && type < OccupancyMap_Count, "Invalid Type");
#endif

	m_occupancyMapType = type;
}

void BlackBoard::IncOccupancyMapType(void)
{
	if (++m_occupancyMapType >= OccupancyMap_Count)
		m_occupancyMapType = 0;
}

void BlackBoard::SetDecayFactor(float factor)
{
#if defined(_DEBUG)
	ASSERTMSG(factor >= 0.0f && factor <= 1.0f, "Invalid Decay Factor");
#endif

	m_decayFactor = factor;
}

void BlackBoard::SetGrowFactor(float factor)
{
#if defined(_DEBUG)
	ASSERTMSG(factor >= 0.0f && factor <= 1.0f, "Invalid Grow Factor");
#endif

	m_growFactor = factor;
}

void BlackBoard::SetUpdateFrequency(float frequency)
{
#if defined(_DEBUG)
	ASSERTMSG(frequency >= 0.0f, "Invalid Update Frequency");
#endif

	m_updateFrequency = frequency;
}
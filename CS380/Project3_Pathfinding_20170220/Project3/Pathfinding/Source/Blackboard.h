/******************************************************************************/
/*!
\file		Blackboard.h
\project	CS380/CS580 AI Framework
\author		Chi-Hao Kuo
\summary	Global blackboard.

Copyright (C) 2016 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

class BlackBoard
{
public:
	BlackBoard();

	void UpdatePlayerPos(int r, int c);
	void UpdateStartPos(void);
	void UpdateGoalPos(int r, int c);

	int GetRowPlayer(void)		{ return m_rPlayer; }
	int GetColPlayer(void)		{ return m_cPlayer; }
	int GetRowStart(void)		{ return m_rStart; }
	int GetColStart(void)		{ return m_cStart; }
	int GetRowGoal(void)		{ return m_rGoal; }
	int GetColGoal(void)		{ return m_cGoal; }

	void ToggleTerrainAnalysisFlag(void)		{ m_terrainAnalysisFlag = !m_terrainAnalysisFlag; }
	void SetTerrainAnalysisType(int type);
	void IncTerrainAnalysisType(void);
	void ToggleOccupancyMapFlag(void)			{ m_occupancyMapFlag = !m_occupancyMapFlag; }
	void SetOccupancyMapType(int type);
	void IncOccupancyMapType(void);
	void SetDecayFactor(float factor);
	void SetGrowFactor(float factor);
	void SetUpdateFrequency(float frequency);
	void SetSingleStepFlag(bool flag)			{ m_previousSingleStep = m_singleStep; m_singleStep = flag; }
	void ToggleSingleStepFlag(void)				{ m_previousSingleStep = m_singleStep; m_singleStep = !m_singleStep; }
	void RestoreSingleStepFlag(void)			{ m_singleStep = m_previousSingleStep; }
	void SetFOVAngle(float angle)				{ m_fovAngle = angle; }
	void SetFOVCloseDistance(float distance)	{ m_fovCloseDistance = distance; }
	void SetSeekPlayerFlag(bool flag)			{ m_seekPlayer = flag; }

	bool GetTerrainAnalysisFlag(void)			{ return m_terrainAnalysisFlag; }
	int GetTerrainAnalysisType(void)			{ return m_terrainAnalysisType; }
	bool GetOccupancyMapFlag(void)				{ return m_occupancyMapFlag; }
	int GetOccupancyMapType(void)				{ return m_occupancyMapType; }
	float GetDecayFactor(void)					{ return m_decayFactor; }
	float GetGrowFactor(void)					{ return m_growFactor; }
	float GetUpdateFrequency(void)				{ return m_updateFrequency; }
	bool GetSingleStep(void)					{ return m_singleStep; }
	float GetFOVAngle(void)						{ return m_fovAngle; }
	float GetFOVCloseDistance(void)				{ return m_fovCloseDistance; }
	bool GetSeekPlayerFlag(void)				{ return m_seekPlayer; }

private:
	int	m_rPlayer;		// player row position
	int	m_cPlayer;		// player col position
	int	m_rStart;		// start row in A* (player)
	int	m_cStart;		// start col in A* (player)
	int	m_rGoal;		// goal row in A* (player)
	int	m_cGoal;		// goal col in A* (player)

	bool m_terrainAnalysisFlag;		// toggle terrain analysis on/off
	int m_terrainAnalysisType;		// terrain analysis type
	bool m_occupancyMapFlag;		// toggle occupancy on/off
	int m_occupancyMapType;			// occupancy map type
	float m_decayFactor;			// decay factor
	float m_growFactor;				// grow factor
	float m_updateFrequency;		// update frequency for propagation
	bool m_singleStep;				// toggle single step
	bool m_previousSingleStep;		// store previous single step value
	float m_fovAngle;				// FOV angle (hide and seek)
	float m_fovCloseDistance;		// FOV close distance (hide and seek)
	bool m_seekPlayer;				// flag: is enemy seeking player (hide and seek)
};
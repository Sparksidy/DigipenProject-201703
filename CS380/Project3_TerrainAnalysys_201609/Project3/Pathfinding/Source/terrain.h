/* Copyright Steve Rabin, 2012. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright Steve Rabin, 2012"
 */

#pragma once

#include "singleton.h"
#include "debugdrawing.h"
#include "Map.h"
#include <vector>

enum TerrainAnalysisType
{
	TerrainAnalysis_None = 0,
	TerrainAnalysis_OpennessClosestWall,
	TerrainAnalysis_OpennessRearCover,
	TerrainAnalysis_Visibility,
	TerrainAnalysis_RearCoverWithHighVisibility,
	TerrainAnalysis_VisibleToPlayer,
	TerrainAnalysis_Search,
	TerrainAnalysis_Count
};

class Terrain
{
public:
	
	TerrainAnalysisType m_analysis;

	Terrain( void );
	~Terrain( void );

  void Update();

	void Create( void );
	void NextMap( void );
	int GetMapIndex( void )					{ return( m_nextMap ); }

	D3DXVECTOR3 GetCoordinates( int r, int c );
	bool GetRowColumn( D3DXVECTOR3* pos, int* r, int* c );
	inline bool IsWall( int r, int c )		
  {
    if (r < 0 || c < 0 || r >= m_width || c >= m_width)
      return true;
    return( m_terrain[r][c] == TILE_WALL ); 
  }

	bool IsClearPath( int r0, int c0, int r1, int c1 );

	void SetColor( int r, int c, DebugDrawingColor color );
	void ResetColors( void );

	void UpdatePlayerPos( D3DXVECTOR3& pos, D3DXVECTOR3& dir );

	void Analyze( void );
	void IncTerrainAnalysis( void );
	inline void SetInfluenceMapValue( int r, int c, float value) { m_terrainInfluenceMap[r][c] = value; }
	inline float GetInfluenceMapValue( int r, int c ) { return m_terrainInfluenceMap[r][c]; }
	void ResetInfluenceMap( void );

	void DrawDebugVisualization( IDirect3DDevice9* pd3dDevice );
	int GetWidth() const;
  int GetMapSize(void)const { return m_maps.size(); } ;
  std::vector<Map>* GetMaps(void)const { return &(std::vector<Map>)m_maps; };

  unsigned curSeenWall = 0;
  bool isFogOfWar = false;
  int counter = 0;
	void InitFogOfWar();
	void ClearFogOfWar();
  bool UpdateFogOfWar(D3DXVECTOR3& a, D3DXVECTOR3& b); //return true if need to recalculate path
protected:

	// Map List parameters
	int m_nextMap;
	typedef std::vector<Map> MapList;
	MapList m_maps;

	// Parameters for current map
	int m_width;
	Tile** m_terrain;
	DebugDrawingColor** m_terrainColor;
	float** m_terrainInfluenceMap;

	bool m_reevaluateAnalysis;
	int m_rPlayer, m_cPlayer;
	D3DXVECTOR3 m_dirPlayer;

	void AnalyzeOpennessClosestWall( void );
	void AnalyzeOpennessRearCover( void );
	void AnalyzeVisibility( void );
	void AnalyzeRearCoverWithHighVisibility( void );
	void AnalyzeVisibleToPlayer( void );
	void AnalyzeSearch( void );
	void ClearTerrainAnalysis( void );
  bool intersectGrid(int y, int x, D3DXVECTOR2 rayStart, D3DXVECTOR2 rayEnd);
  bool Test(int r0, int c0, int r1, int c1);
	float ClosestWall(int row, int col);
	float RearCoverValue( int row, int col );
	bool LineIntersect( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 );
};

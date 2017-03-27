/******************************************************************************/
/*!
\file		TinyCustomData.h
\project	CS380/CS580 AI Framework
\author		Chi-Hao Kuo
\summary	Custom data for tiny.

Copyright (C) 2016 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <BehaviorTrees/BehaviorTreesShared.h>

namespace BT
{
  struct FlockCustomData : public NodeAbstractData
  {
  public:
    static float separationFactor;
    static float cohesionFactor;
    static float alignmentFactor;
    static float detectionDistance;
    static float separationDistance;

    D3DXVECTOR3 velocity = D3DXVECTOR3(0,0,0);
    D3DXVECTOR3 acceleration = D3DXVECTOR3(0, 0, 0);;
    bool initialized = false;

    static void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
  };
}
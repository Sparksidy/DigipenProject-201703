/******************************************************************************/
/*!
\file		L_MoveToFurthestTarget.h
\project	CS380/CS580 AI Framework
\author		Chi-Hao Kuo
\summary	Action: Move to the furthest AI agent.

Copyright (C) 2016 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <BehaviorTrees/BehaviorTreesShared.h>

namespace BT
{
  // selector node
  class L_FlockNatural : public LeafNode
  {
  protected:
    // Only run when initializng the node
    virtual void OnInitial(NodeData *nodedata_ptr) override;
    // Only run when entering the node
    virtual Status OnEnter(NodeData *nodedata_ptr) override;
    // Only run when exiting the node
    virtual void OnExit(NodeData *nodedata_ptr) override;
    // Run every frame
    virtual Status OnUpdate(float dt, NodeData *nodedata_ptr) override;
    // Only run when node is in suspended
    virtual Status OnSuspend(NodeData *nodedata_ptr) override;
    //Initialize custom data type
    void InitialCustomData(NodeData *nodedata_ptr);


    void GetAllNeighbour(dbCompositionList& allGo, NodeData* nodedata_ptr, dbCompositionList& returnedNeighbour);
    D3DXVECTOR3 CalculateCohesion(dbCompositionList& allGO, NodeData *nodedata_ptr);
    D3DXVECTOR3 CalculateAlignment(dbCompositionList& neighbour, NodeData *nodedata_ptr);
    D3DXVECTOR3 CalculateSeparation(dbCompositionList& neighbour, NodeData *nodedata_ptr);
    float randomAlignment;
    float randomCohesion;
    float randomSeparation;
    float randomCentering;
  };
}
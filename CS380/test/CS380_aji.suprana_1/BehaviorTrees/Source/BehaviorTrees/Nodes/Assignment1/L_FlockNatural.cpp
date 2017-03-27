/******************************************************************************/
/*!
\file		L_MoveToFurthestTarget.cpp
\project	CS380/CS580 AI Framework
\author		Chi-Hao Kuo
\summary	Action: Move to the furthest AI agent.

Copyright (C) 2016 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <Stdafx.h>

using namespace BT;

/*--------------------------------------------------------------------------*
Name:           InitialCustomData

Description:    Initializing customdata type

Arguments:      nodedata_ptr:	current node data pointer.

Returns:        None.
*---------------------------------------------------------------------------*/
void L_FlockNatural::InitialCustomData(NodeData *nodedata_ptr)
{
  nodedata_ptr->InitialCustomData<FlockCustomData>();
}

/*--------------------------------------------------------------------------*
Name:           OnInitial

Description:    Only run when initializng the node.

Arguments:      nodedata_ptr:	current node data pointer.

Returns:        None.
*---------------------------------------------------------------------------*/
void L_FlockNatural::OnInitial(NodeData *nodedata_ptr)
{
  LeafNode::OnInitial(nodedata_ptr);
}

/*--------------------------------------------------------------------------*
Name:           OnEnter

Description:    Only run when entering the node.

Arguments:      nodedata_ptr:	current node data pointer.

Returns:        Status:			return status.
*---------------------------------------------------------------------------*/

Status L_FlockNatural::OnEnter(NodeData *nodedata_ptr)
{
  LeafNode::OnEnter(nodedata_ptr);

  AgentBTData &agentdata = nodedata_ptr->GetAgentData();
  GameObject *self = agentdata.GetGameObject();
  InitialCustomData(nodedata_ptr);
  FlockCustomData* data = nodedata_ptr->GetCustomData<FlockCustomData>();

  data->velocity = D3DXVECTOR3(0, 0, 0);
  if (!nodedata_ptr->GetCustomData<FlockCustomData>()->initialized)
  {
    self->SetSpeedStatus(TinySpeedStatus::TS_JOG);
    SetTinySpeed(self);
    nodedata_ptr->GetCustomData<FlockCustomData>()->initialized = true;
  }
  randomAlignment  = g_random.RangeFloat(1.0f,2.3f);
  randomCohesion   = g_random.RangeFloat(1.0f,2.3f);
  randomSeparation = g_random.RangeFloat(1.0f,10.3f);
  randomCentering  = g_random.RangeFloat(1.0f,2.3f);
  

  return Status::BT_RUNNING;
}

/*--------------------------------------------------------------------------*
Name:           OnExit

Description:    Only run when exiting the node.

Arguments:      nodedata_ptr:	current node data pointer.

Returns:        None.
*---------------------------------------------------------------------------*/
void L_FlockNatural::OnExit(NodeData *nodedata_ptr)
{
  LeafNode::OnExit(nodedata_ptr);
}
void SpeedCap(D3DXVECTOR3& vec, float cap)
{
  if (D3DXVec3Length(&vec) > cap)
  {
    D3DXVec3Normalize(&vec, &vec);
    vec *= cap;
  }
}
/*--------------------------------------------------------------------------*
Name:           OnUpdate

Description:    Run every frame.

Arguments:      dt:				delta time.
nodedata_ptr:	current node data pointer.

Returns:        Status:			return status.
*---------------------------------------------------------------------------*/
Status L_FlockNatural::OnUpdate(float dt, NodeData *nodedata_ptr)
{
  LeafNode::OnUpdate(dt, nodedata_ptr);

  GameObject *self = nodedata_ptr->GetAgentData().GetGameObject();
  
  dbCompositionList allGo;
  dbCompositionList neighbour;
  g_database.ComposeList(allGo);

  GetAllNeighbour(allGo, nodedata_ptr, neighbour);

  D3DXVECTOR3 alignment;
  D3DXVECTOR3 cohesion;
  D3DXVECTOR3 separation;

  float curSpeed = self->GetTiny().GetSpeedScale();
  D3DXVECTOR3 curVelocity = (self->GetTargetPOS() - self->GetBody().GetPos());
  D3DXVec3Normalize(&curVelocity, &curVelocity);
  curVelocity *= curSpeed;

  FlockCustomData* data = nodedata_ptr->GetCustomData<FlockCustomData>();

  alignment = CalculateAlignment(neighbour, nodedata_ptr);
  cohesion = CalculateCohesion(neighbour, nodedata_ptr);
  separation = CalculateSeparation(neighbour, nodedata_ptr);

  D3DXVECTOR3 addThis = cohesion * data->cohesionFactor * randomCohesion + separation * data->separationFactor  * randomSeparation + alignment * data->alignmentFactor * randomAlignment * 3;
  data->acceleration += addThis;
  SpeedCap(data->acceleration, 5);

  data->velocity += data->acceleration * 5;
  SpeedCap(data->velocity, 5);

  float speed = D3DXVec3Length(&data->velocity);
  self->SetTargetPOS(self->GetBody().GetPos() + data->velocity);

  if(speed < 10)
    self->SetSpeedStatus(TinySpeedStatus::TS_WALK);
  else
    self->SetSpeedStatus(TinySpeedStatus::TS_JOG);

  SetTinySpeed(self);

  return Status::BT_RUNNING;
}



/*--------------------------------------------------------------------------*
Name:           OnSuspend

Description:    Only run when node is in suspended.

Arguments:      nodedata_ptr:	current node data pointer.

Returns:        Status:			return status.
*---------------------------------------------------------------------------*/
Status L_FlockNatural::OnSuspend(NodeData *nodedata_ptr)
{
  return LeafNode::OnSuspend(nodedata_ptr);
}

D3DXVECTOR3 L_FlockNatural::CalculateCohesion(dbCompositionList& neighbour, NodeData *nodedata_ptr)
{
  D3DXVECTOR3 cohesion(0,0,0);
  if (neighbour.size() == 0)
    return cohesion;

  GameObject *self = nodedata_ptr->GetAgentData().GetGameObject();

  for (auto curObj : neighbour)
  {
    cohesion += curObj->GetBody().GetPos();
  }

  cohesion /= neighbour.size();

  cohesion -= self->GetBody().GetPos();
  D3DXVec3Normalize(&cohesion, &cohesion);

  return cohesion;
}

D3DXVECTOR3 L_FlockNatural::CalculateAlignment(dbCompositionList& neighbour, NodeData *nodedata_ptr)
{
  D3DXVECTOR3 alignment(0, 0, 0);

  if (neighbour.size() == 0)
    return alignment;

  GameObject *self = nodedata_ptr->GetAgentData().GetGameObject();

  for (auto curObj : neighbour)
  {
    D3DXVECTOR3 velocity = (curObj->GetTargetPOS() - curObj->GetBody().GetPos());
   // D3DXVec3Normalize(&velocity, &velocity);
    alignment += velocity * self->GetTiny().GetSpeedScale();
  }

  alignment /= neighbour.size();
  D3DXVec3Normalize(&alignment, &alignment);

  return alignment;
}

D3DXVECTOR3 L_FlockNatural::CalculateSeparation(dbCompositionList& neighbour, NodeData *nodedata_ptr)
{
  D3DXVECTOR3 separation(0, 0, 0);
  if (neighbour.size() == 0)
    return separation;

  GameObject *self = nodedata_ptr->GetAgentData().GetGameObject();
  FlockCustomData* data = nodedata_ptr->GetCustomData<FlockCustomData>();
  float desiredDist = data->separationDistance;
  for (auto curObj : neighbour)
  {
    D3DXVECTOR3 curSeparation = curObj->GetBody().GetPos() - self->GetBody().GetPos();
    float curDist = D3DXVec3Length(&curSeparation);
    float t = desiredDist - curDist;
    D3DXVec3Normalize(&curSeparation, &curSeparation);
    curSeparation *= t;

    separation += curSeparation;
  }
  
  separation *= -1;
  //D3DXVec3Normalize(&separation, &separation);

  return separation;
}


void L_FlockNatural::GetAllNeighbour(dbCompositionList& allGo, NodeData* nodedata_ptr,dbCompositionList& returnedNeighbour)
{
  dbCompositionList neighbour;
  GameObject *self = nodedata_ptr->GetAgentData().GetGameObject();
  D3DXVECTOR3 curPos = self->GetBody().GetPos();

  for (auto go : allGo)
  {
    if (go == self)
      continue;

    D3DXVECTOR3 neigbourPos = go->GetBody().GetPos();
    float distance = D3DXVec3Length(&(neigbourPos - curPos));

    //only get all child that is on the neighbour
    if (distance < nodedata_ptr->GetCustomData<FlockCustomData>()->detectionDistance * 10)
    {
      returnedNeighbour.push_back(go);
    }
  }

  if (returnedNeighbour.size() < 10)
  {
    returnedNeighbour == allGo;
  }
}

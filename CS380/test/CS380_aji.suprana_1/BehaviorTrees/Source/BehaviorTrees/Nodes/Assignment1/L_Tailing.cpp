/******************************************************************************/
/*!
\file		L_MoveToMouseTarget.cpp
\project	CS380/CS580 AI Framework
\author		Chi-Hao Kuo
\summary	Action: Move to mouse click point.

Copyright (C) 2016 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <Stdafx.h>

using namespace BT;

///* protected methods */
//Status LeaderBehave(GameObject* obj, NodeData *nodedata_ptr)
//{
//  GameObject *self = nodedata_ptr->GetAgentData().GetGameObject();
//  D3DXVECTOR3 mousePos = nodedata_ptr->GetAgentData().GetCustomData<TinyCustomData>()->m_mousePos;
//  D3DXVECTOR3 objPos = self->GetBody().GetPos();
//  D3DXVECTOR3 distVec = mousePos - objPos;
//  float dist = D3DXVec3Length(&distVec);
//
//  D3DXVECTOR3 target = mousePos ;
//
//  self->SetTargetPOS(target);
//  self->GetBody().SetSpeed(dist);
//
//  if (IsNear(self->GetBody().GetPos(), target))
//    return Status::BT_SUCCESS;
//
//  return Status::BT_RUNNING;
//}
//
//Status tailingBehave(GameObject* curObj, GameObject* prevObj, NodeData* nodedata_ptr)
//{
//  GameObject *self = nodedata_ptr->GetAgentData().GetGameObject();
//  D3DXVECTOR3 prevPos = prevObj->GetBody().GetPos();
//  D3DXVECTOR3 objPos = self->GetBody().GetPos();
//  D3DXVECTOR3 distVec = (prevPos - objPos);
//  float dist = D3DXVec3Length(&distVec);
//
//  D3DXVECTOR3 target = prevPos - distVec * 0.01;
//
//  self->SetTargetPOS(target);
//  self->GetBody().SetSpeed(dist);
//
//  if (IsNear(self->GetBody().GetPos(), target))
//    return Status::BT_SUCCESS;
//
//  return Status::BT_RUNNING;
//
//}
/*--------------------------------------------------------------------------*
Name:           OnInitial

Description:    Only run when initializng the node.

Arguments:      nodedata_ptr:	current node data pointer.

Returns:        None.
*---------------------------------------------------------------------------*/
void L_Tailing::OnInitial(NodeData *nodedata_ptr)
{
  LeafNode::OnInitial(nodedata_ptr);
}

/*--------------------------------------------------------------------------*
Name:           OnEnter

Description:    Only run when entering the node.

Arguments:      nodedata_ptr:	current node data pointer.

Returns:        Status:			return status.
*---------------------------------------------------------------------------*/
D3DXVECTOR3 distVec = D3DXVECTOR3(0, 0, 0);
D3DXVECTOR3 target = D3DXVECTOR3(0, 0, 0);
Status L_Tailing::OnEnter(NodeData *nodedata_ptr)
{
  LeafNode::OnEnter(nodedata_ptr);
  dbCompositionList allGo;
  g_database.ComposeList(allGo);
  AgentBTData &agentdata = nodedata_ptr->GetAgentData();
  GameObject *self = agentdata.GetGameObject();
  TinyCustomData *customdata = agentdata.GetCustomData<TinyCustomData>();

  D3DXVECTOR3 mousePos = nodedata_ptr->GetAgentData().GetCustomData<TinyCustomData>()->m_mousePos;

  if (customdata->m_mouseClick)
  {
    customdata->m_mouseClick = false;

    for (int i = 0; i < allGo.size(); ++i)
    {
      if (allGo[i] == self)
      {
        if (i == 0)
        {
          distVec = self->GetBody().GetPos() - mousePos;
          target = mousePos;
        }
        else
        {
          target = mousePos - 0.1f* i * distVec;
        }
      }
    }

    self->SetTargetPOS(target);
    self->SetSpeedStatus(TinySpeedStatus::TS_JOG);
    SetTinySpeed(self);

    return Status::BT_RUNNING;
  }

  return Status::BT_FAILURE;

}

/*--------------------------------------------------------------------------*
Name:           OnExit

Description:    Only run when exiting the node.

Arguments:      nodedata_ptr:	current node data pointer.

Returns:        None.
*---------------------------------------------------------------------------*/
void L_Tailing::OnExit(NodeData *nodedata_ptr)
{
  LeafNode::OnExit(nodedata_ptr);
}

/*--------------------------------------------------------------------------*
Name:           OnUpdate

Description:    Run every frame.

Arguments:      dt:				delta time.
nodedata_ptr:	current node data pointer.

Returns:        Status:			return status.
*---------------------------------------------------------------------------*/
Status L_Tailing::OnUpdate(float dt, NodeData *nodedata_ptr)
{
  LeafNode::OnUpdate(dt, nodedata_ptr);

  GameObject *self = nodedata_ptr->GetAgentData().GetGameObject();

  if (IsNear(self->GetBody().GetPos(), self->GetTargetPOS()))
    return Status::BT_SUCCESS;

  return Status::BT_RUNNING;
}


/*--------------------------------------------------------------------------*
Name:           OnSuspend

Description:    Only run when node is in suspended.

Arguments:      nodedata_ptr:	current node data pointer.

Returns:        Status:			return status.
*---------------------------------------------------------------------------*/
Status L_Tailing::OnSuspend(NodeData *nodedata_ptr)
{
  return LeafNode::OnSuspend(nodedata_ptr);
}
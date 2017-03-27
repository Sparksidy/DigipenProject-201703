/******************************************************************************/
/*!
\file		C_ParallelSequencer.cpp
\project	CS380/CS580 AI Framework
\author		Chi-Hao Kuo
\summary	Parallel sequencer node logic.

Copyright (C) 2016 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <Stdafx.h>

using namespace BT;

/* protected methods */

/*--------------------------------------------------------------------------*
Name:           OnInitial

Description:    Only run when initializng the node.

Arguments:      nodedata_ptr:	current node data pointer.

Returns:        None.
*---------------------------------------------------------------------------*/
void C_Flocking::OnInitial(NodeData *nodedata_ptr)
{
  ParallelNode::OnInitial(nodedata_ptr);
}

/*--------------------------------------------------------------------------*
Name:           OnEnter

Description:    Only run when entering the node.

Arguments:      nodedata_ptr:	current node data pointer.

Returns:        Status:			return status.
*---------------------------------------------------------------------------*/
Status C_Flocking::OnEnter(NodeData *nodedata_ptr)
{
  return ParallelNode::OnEnter(nodedata_ptr);
}

/*--------------------------------------------------------------------------*
Name:           OnExit

Description:    Only run when exiting the node.

Arguments:      nodedata_ptr:	current node data pointer.

Returns:        None.
*---------------------------------------------------------------------------*/
void C_Flocking::OnExit(NodeData *nodedata_ptr)
{
  ParallelNode::OnExit(nodedata_ptr);
}

/*--------------------------------------------------------------------------*
Name:           OnUpdate

Description:    Run every frame.

Arguments:      dt:				delta time.
nodedata_ptr:	current node data pointer.

Returns:        Status:			return status.
*---------------------------------------------------------------------------*/
Status C_Flocking::OnUpdate(float dt, NodeData *nodedata_ptr)
{
  UNREFERENCED_PARAMETER(dt);

  // return SUCCESS when all children return SUCCESS

    return Status::BT_RUNNING;
}

/*--------------------------------------------------------------------------*
Name:           OnSuspend

Description:    Only run when node is in suspended.

Arguments:      nodedata_ptr:	current node data pointer.

Returns:        Status:			return status.
*---------------------------------------------------------------------------*/
Status C_Flocking::OnSuspend(NodeData *nodedata_ptr)
{
  return ParallelNode::OnSuspend(nodedata_ptr);
}
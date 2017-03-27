/******************************************************************************/
/*!
\file		TinyCustomData.cpp
\project	CS380/CS580 AI Framework
\author		Chi-Hao Kuo
\summary	Custom data for tiny.

Copyright (C) 2016 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <Stdafx.h>

using namespace BT;

/* public methods */

/*--------------------------------------------------------------------------*
Name:           TinyCustomData

Description:    Default constructor.

Arguments:      None.

Returns:        None.
*---------------------------------------------------------------------------*/
TinyCustomData::TinyCustomData()
	: m_npc(nullptr)
{
	m_mouseClick = false;
  m_mouseDown = false;
}

/*--------------------------------------------------------------------------*
Name:           OnMessage

Description:    Handle messages.

Arguments:      None.

Returns:        None.
*---------------------------------------------------------------------------*/
void TinyCustomData::OnMessage(void)
{
	// default behavior is to drop all messages

	while (m_msgqueue.size())
	{
		MSG_Object &msg = m_msgqueue.front();
		m_msgqueue.pop();

		switch (msg.GetName())
		{
		case MSG_MouseClick:
		{
			m_mouseClick = true;
			m_mousePos = msg.GetVector3Data();
		}
			break;
    case MSG_MouseDown:
    {
      m_mouseDown = true;
      m_mousePos = msg.GetVector3Data();
    }
    break;
    case MSG_MouseUp:
    {
      m_mouseDown = false;
    }
    break;
		default:
			break;
		}
	}
}
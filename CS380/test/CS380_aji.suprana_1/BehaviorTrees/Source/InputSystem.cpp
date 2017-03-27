#include <Stdafx.h>
#include "InputSystem.h"

// Initilize AI reasoner class
bool InputSystem::Initialize(void) 
{
  m_Lmouse = false;
  mouse2DPos = D3DXVECTOR3(0,0,0);
  mouse3DPos = D3DXVECTOR3(0, 0, 0);
  return ISystem::Initialize(); 
}

// Execute each agent's reasoner
void InputSystem::Update(float dt) 
{
  if(m_Lmouse)
    g_database.SendMsgFromSystem(MSG_MouseDown, MSG_Data(mouse3DPos));
  else
    g_database.SendMsgFromSystem(MSG_MouseUp, MSG_Data(mouse3DPos));
};

// Shutdown reasoner
void InputSystem::Shutdown(void)
{

};
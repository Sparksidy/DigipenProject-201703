#pragma once

class InputSystem : public ISystem
{
public:
  /*Getter Setter*/
  void SetLMouse(bool b) { m_Lmouse = b; }
  void Set2DMouse(D3DXVECTOR3 pos) { mouse2DPos = pos; }
  void Set3DMouse(D3DXVECTOR3 pos) { mouse3DPos = pos; }


  /*Method*/
public:
  // Initilize AI reasoner class
  bool Initialize(void);

  // Execute each agent's reasoner
  void Update(float dt);

  // Shutdown reasoner
  void Shutdown(void);

  /*Variables*/
private:
  bool m_Lmouse = false;
  D3DXVECTOR3 mouse2DPos;
  D3DXVECTOR3 mouse3DPos;
};
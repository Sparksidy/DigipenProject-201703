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
float FlockCustomData::separationFactor = 0.5f;
float FlockCustomData::cohesionFactor = 0.5f;
float FlockCustomData::alignmentFactor = 0.5f;

float FlockCustomData::separationDistance = 1;
float FlockCustomData::detectionDistance = 3;
void CALLBACK FlockCustomData::OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
  switch (nControlID)
  {
  case IDC_COHESION:
    cohesionFactor = reinterpret_cast<CDXUTSlider*>(pControl)->GetValue() / 100.f;
    break;

  case IDC_ALIGNMENT:
    alignmentFactor = reinterpret_cast<CDXUTSlider*>(pControl)->GetValue() / 100.f;
    break;

  case  IDC_SEPARATION:
    separationFactor = reinterpret_cast<CDXUTSlider*>(pControl)->GetValue() / 100.f;
    break;

  case IDC_DETECTIONDISTANCE:
    detectionDistance = reinterpret_cast<CDXUTSlider*>(pControl)->GetValue() / 50.f;
    break;

  case IDC_SEPARATIONDISTANCE:
    separationDistance = reinterpret_cast<CDXUTSlider*>(pControl)->GetValue() / 50.f;
    break;

  }

}

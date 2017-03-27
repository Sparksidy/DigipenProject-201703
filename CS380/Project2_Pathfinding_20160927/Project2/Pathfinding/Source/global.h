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

#include <assert.h>
#include "DXUT\DXUT.h"

#define ASSERTMSG(eval, message) assert(eval && message)
#define COMPILE_TIME_ASSERT(expression, message) { typedef int ASSERT__##message[1][(expression)]; }


#define g_clock Singleton<Clock>::GetInstance()
#define g_database Singleton<Database>::GetInstance()
#define g_msgroute Singleton<MsgRoute>::GetInstance()
#define g_sound Singleton<Sound>::GetInstance()
#define g_debuglog Singleton<DebugLog>::GetInstance()
#define g_debugdrawing Singleton<DebugDrawing>::GetInstance()
#define g_terrain Singleton<Terrain>::GetInstance()
#define g_trigger Singleton<TriggerSystem>::GetInstance()
#define g_astar Singleton<AStar>::GetInstance()

#define INVALID_OBJECT_ID 0
#define SYSTEM_OBJECT_ID 1


typedef unsigned int objectID;


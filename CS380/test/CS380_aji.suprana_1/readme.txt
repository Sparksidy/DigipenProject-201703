Student Name: Aji Suprana

Project Name: Flocking and Formation

What I implemented:
All in folder Nodes/Assignment1/*
InputSystem.cpp for the messaging

Directions (if needed):
1.Flocking:
-it automatically flocks
-click,hold and drag to let all character following mouse

2.FlockingTailing:
-Flocking as default state
-Click to let character form a line

3.Formations:
-Random movement as default
-click to let character change formation, line and circle

What I liked about the project and framework:
- Behavior is reusable if designed well
- Easy to make a new behavior with the tool provided and existing framework

What I disliked about the project and framework:
- The character Tiny structure is too deep without 
any documentation, which make it hard to do 
things like slerp the speed, change velocity, 
accelartion deceleration

Any difficulties I experienced while doing the project:

Hours spent:
13hours

Agent Communication (file and line number):
InputSystem.cpp

New selector node (name):
C_FlockSelect

10 total nodes (names):
C_FlockNatural
C_FLockSelect
C_FlockToMouse
L_CheckMouseDown
L_CheckMouseUp
L_CircleFormation
L_FlockNatural
L_FLockToMouse
L_Idle
L_Tailing

3 Behavior trees (names):
Flocking
FlockTailing
FormationChanging

Extra credit:
2D Flocking
2 formation
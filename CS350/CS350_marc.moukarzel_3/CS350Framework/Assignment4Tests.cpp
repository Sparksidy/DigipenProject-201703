#include "Application.hpp"
#include "Shapes.hpp"
#include "Geometry.hpp"
#include "Math/Utilities.hpp"
#include "DebugDraw.hpp"
#include "Components.hpp"
#include "SimpleNSquared.hpp"
#include "UnitTests.hpp"
#include "DynamicAabbTree.hpp"
#include "Helpers/Source/Simplification.hpp"
#include "BspTree.hpp"
#include "Helpers/Source/ObjReader.hpp"

void InitializeAssignment4Tests()
{
  mTestFns.push_back(AssignmentUnitTestList());
  AssignmentUnitTestList& list = mTestFns.back();
}

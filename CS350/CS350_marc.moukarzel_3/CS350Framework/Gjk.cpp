///////////////////////////////////////////////////////////////////////////////
///
/// Authors: Joshua Davis
/// Copyright 2015, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#include "Gjk.hpp"

#include "Model.hpp"
#include "Mesh.hpp"
#include "DebugDraw.hpp"
#include "Math/Utilities.hpp"
#include "Geometry.hpp"


//-----------------------------------------------------------------------------SupportShape
Vector3 SupportShape::GetCenter(const std::vector<Vector3>& points, const Matrix4& transform) const
{
  Vector3 center = Vector3::cZero;
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return center;
}

Vector3 SupportShape::Support(const Vector3& direction, const std::vector<Vector3>& points, const Matrix4& transform) const
{
  Vector3 result = Vector3::cZero;
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return result;
}

void SupportShape::DebugDraw(const std::vector<Vector3>& points, const Matrix4& transform, Vector4 color) const
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
}

//-----------------------------------------------------------------------------ModelSupportShape
Vector3 ModelSupportShape::GetCenter() const
{
  return SupportShape::GetCenter(mModel->mMesh->mVertices, mModel->mOwner->has(Transform)->GetTransform());
}

Vector3 ModelSupportShape::Support(const Vector3& direction) const
{
  return SupportShape::Support(direction, mModel->mMesh->mVertices, mModel->mOwner->has(Transform)->GetTransform());
}

void ModelSupportShape::DebugDraw(Vector4 color) const
{
  SupportShape::DebugDraw(mModel->mMesh->mVertices, mModel->mOwner->has(Transform)->GetTransform());
}

//-----------------------------------------------------------------------------PointsSupportShape
PointsSupportShape::PointsSupportShape()
{
  mScale = Vector3(1);
  mRotation = Matrix3::cIdentity;
  mTranslation = Vector3::cZero;
}

Vector3 PointsSupportShape::GetCenter() const
{
  Matrix4 transform = Math::BuildTransform(mTranslation, mRotation, mScale);
  return SupportShape::GetCenter(mLocalSpacePoints, transform);
}

Vector3 PointsSupportShape::Support(const Vector3& direction) const
{
  Matrix4 transform = Math::BuildTransform(mTranslation, mRotation, mScale);
  return SupportShape::Support(direction, mLocalSpacePoints, transform);
}

void PointsSupportShape::DebugDraw(Vector4 color) const
{
  Matrix4 transform = Math::BuildTransform(mTranslation, mRotation, mScale);
  SupportShape::DebugDraw(mLocalSpacePoints, transform, color);
}

//-----------------------------------------------------------------------------SphereSupportShape
Vector3 SphereSupportShape::GetCenter() const
{
  return mSphere.mCenter;
}

Vector3 SphereSupportShape::Support(const Vector3& direction) const
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return Vector3::cZero;
}

void SphereSupportShape::DebugDraw(Vector4 color) const
{
  DebugShape& shape = gDebugDrawer->DrawSphere(mSphere);
  shape.Color(color);
}

//-----------------------------------------------------------------------------ObbSupportShape
Vector3 ObbSupportShape::GetCenter() const
{
  return mTranslation;
}

Vector3 ObbSupportShape::Support(const Vector3& direction) const
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return Vector3::cZero;
}

void ObbSupportShape::DebugDraw(Vector4 color) const
{
  Matrix4 transform = Math::BuildTransform(mTranslation, mRotation, mScale);
  DebugShape& shape = gDebugDrawer->DrawAabb(Aabb(Vector3(-0.5f), Vector3(0.5f)));
  shape.Color(color);
  shape.SetTransform(transform);
}

//-----------------------------------------------------------------------------Gjk
VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0,
                                               size_t& newSize, int newIndices[4])
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return VoronoiRegion::Unknown;
}

Vector3 Gjk::FindClosestPoint(const Vector3& q, const Vector3& p0)
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return Vector3(Math::PositiveMax());
}

Vector3 Gjk::ComputeSearchDirection(const Vector3& q, const Vector3& p0)
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return Vector3(Math::PositiveMax());
}

// Edge Tests
VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1,
                                                 size_t& newSize, int newIndices[4])
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return VoronoiRegion::Unknown;
}

Vector3 Gjk::FindClosestPoint(const Vector3& q, const Vector3& p0, const Vector3& p1)
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return Vector3(Math::PositiveMax());
}

Vector3 Gjk::ComputeSearchDirection(const Vector3& q, const Vector3& p0, const Vector3& p1)
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return Vector3(Math::PositiveMax());
}

// Triangle Tests
VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2,
                                                 size_t& newSize, int newIndices[4])
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return VoronoiRegion::Unknown;
}

Vector3 Gjk::FindClosestPoint(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return Vector3(Math::PositiveMax());
}

Vector3 Gjk::ComputeSearchDirection(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return Vector3(Math::PositiveMax());
}

// Tetrahedron Tests
VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3,
                                                 size_t& newSize, int newIndices[4])
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return VoronoiRegion::Unknown;
}

Vector3 Gjk::FindClosestPoint(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return Vector3(Math::PositiveMax());
}

Vector3 Gjk::ComputeSearchDirection(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
  /******Student:Assignment5******/
  Warn("Assignment5: Required function un-implemented");
  return Vector3(Math::PositiveMax());
}

Gjk::Gjk()
{
}

bool Gjk::Intersect(const SupportShape* shapeA, const SupportShape* shapeB, unsigned int maxIterations, int debuggingIndex, CsoPoint& closestPoint, float epsilon, bool debugDraw)
{
  Warn("Assignment5: Required function un-implemented");
  return false;
}

Gjk::CsoPoint Gjk::ComputeSupport(const Vector3& direction)
{
  /******Student:Assignment5******/
  CsoPoint result;
  Warn("Assignment5: Required function un-implemented");

  return result;
}

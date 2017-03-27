///////////////////////////////////////////////////////////////////////////////
///
/// Authors: Joshua Davis
/// Copyright 2015, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Math/Math.hpp"
#include "Shapes.hpp"
#include "DebugDraw.hpp"

class Model;

//-----------------------------------------------------------------------------SupportShape
class SupportShape
{
public:

  virtual Vector3 GetCenter() const = 0;
  virtual Vector3 Support(const Vector3& direction) const = 0;
  virtual void DebugDraw(Vector4 color = Vector4::cZero) const = 0;

  //Vector3 SupportPoints(const std::vector<Vector3>& points)
  Vector3 GetCenter(const std::vector<Vector3>& points, const Matrix4& transform) const;
  Vector3 Support(const Vector3& direction, const std::vector<Vector3>& points, const Matrix4& transform) const;
  void DebugDraw(const std::vector<Vector3>& points, const Matrix4& transform, Vector4 color = Vector4::cZero) const;
};

//-----------------------------------------------------------------------------ModelSupportShape
class ModelSupportShape : public SupportShape
{
public:

  Vector3 GetCenter() const override;
  Vector3 Support(const Vector3& direction) const override;
  void DebugDraw(Vector4 color = Vector4::cZero) const override;

  Model* mModel;
};

//-----------------------------------------------------------------------------PointsSupportShape
class PointsSupportShape : public SupportShape
{
public:
  PointsSupportShape();
  Vector3 GetCenter() const override;
  Vector3 Support(const Vector3& direction) const override;
  void DebugDraw(Vector4 color = Vector4::cZero) const override;

  std::vector<Vector3> mLocalSpacePoints;
  Vector3 mScale;
  Matrix3 mRotation;
  Vector3 mTranslation;
};

//-----------------------------------------------------------------------------SphereSupportShape
class SphereSupportShape : public SupportShape
{
public:
  Vector3 GetCenter() const override;
  Vector3 Support(const Vector3& direction) const override;
  void DebugDraw(Vector4 color = Vector4::cZero) const override;

  Sphere mSphere;
};

//-----------------------------------------------------------------------------ObbSupportShape
class ObbSupportShape : public SupportShape
{
public:

  Vector3 GetCenter() const override;
  Vector3 Support(const Vector3& direction) const override;
  void DebugDraw(Vector4 color = Vector4::cZero) const override;

  Vector3 mScale;
  Matrix3 mRotation;
  Vector3 mTranslation;
};

namespace VoronoiRegion
{
  enum Type { Point0, Point1, Point2, Point3, 
              Edge01, Edge02, Edge03, Edge12, Edge13, Edge23,
              Triangle012, Triangle013, Triangle023, Triangle123,
              Tetrahedra0123,
              Unknown};
  static const char* Names[] = {"Point0", "Point1", "Point2", "Point3",
                                "Edge01", "Edge02", "Edge03", "Edge12", "Edge13", "Edge23",
                                "Triangle012", "Triangle013", "Triangle023", "Triangle123",
                                "Tetrahedra0123",
                                "Unknown"};
}

/******Student:Assignment5******/
// Implement gjk
//-----------------------------------------------------------------------------Gjk
class Gjk
{
public:

  // Point Tests
  static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0,
                                                   size_t& newSize, int newIndices[4]);
  static Vector3 FindClosestPoint(const Vector3& q, const Vector3& p0);
  static Vector3 ComputeSearchDirection(const Vector3& q, const Vector3& p0);

  // Edge Tests
  static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1,
                                                   size_t& newSize, int newIndices[4]);
  static Vector3 FindClosestPoint(const Vector3& q, const Vector3& p0, const Vector3& p1);
  static Vector3 ComputeSearchDirection(const Vector3& q, const Vector3& p0, const Vector3& p1);

  // Triangle Tests
  static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2,
                                                   size_t& newSize, int newIndices[4]);
  static Vector3 FindClosestPoint(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2);
  static Vector3 ComputeSearchDirection(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2);

  // Tetrahedron Tests
  static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3,
                                                   size_t& newSize, int newIndices[4]);
  static Vector3 FindClosestPoint(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3);
  static Vector3 ComputeSearchDirection(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3);

  // Simple structure that contains all information for a point in Gjk.
  struct CsoPoint
  {
    Vector3 mPointA;
    Vector3 mPointB;
    Vector3 mCsoPoint;
  };

  Gjk();

  // Returns true if the shapes intersect.
  bool Intersect(const SupportShape* shapeA, const SupportShape* shapeB, unsigned int maxIterations, int debuggingIndex, CsoPoint& closestPoint, float epsilon, bool debugDraw);
  // Finds the point furthest in the given direction on the CSO (and the relevant points from each object)
  CsoPoint ComputeSupport(const Vector3& direction);

  

  // Add your implementation here
};

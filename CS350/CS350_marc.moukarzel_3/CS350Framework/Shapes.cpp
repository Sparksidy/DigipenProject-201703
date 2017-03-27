///////////////////////////////////////////////////////////////////////////////
///
/// Authors: Joshua Davis
/// Copyright 2015, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#include "Shapes.hpp"
#include "DebugDraw.hpp"
#include "Geometry.hpp"
#include "Math/Utilities.hpp"
#define MAX_ITERATIONS 50

//-----------------------------------------------------------------------------LineSegment
LineSegment::LineSegment()
{
  mStart = mEnd = Vector3::cZero;
}

LineSegment::LineSegment(Math::Vec3Param start, Math::Vec3Param end)
{
  mStart = start;
  mEnd = end;
}

DebugShape& LineSegment::DebugDraw() const
{
  return gDebugDrawer->DrawLine(*this);
}

//-----------------------------------------------------------------------------Ray
Ray::Ray()
{
  mStart = mDirection = Vector3::cZero;
}

Ray::Ray(Math::Vec3Param start, Math::Vec3Param dir)
{
  mStart = start;
  mDirection = dir;
}

Ray Ray::Transform(const Math::Matrix4& transform) const
{
  Ray transformedRay;
  transformedRay.mStart = Math::TransformPoint(transform, mStart);
  transformedRay.mDirection = Math::TransformNormal(transform, mDirection);
  return transformedRay;
}

Vector3 Ray::GetPoint(float t) const
{
  return mStart + mDirection * t;
}

DebugShape& Ray::DebugDraw(float t) const
{
  return gDebugDrawer->DrawRay(*this, t);
}

//-----------------------------------------------------------------------------PCA Helpers
// DONE
Matrix3 ComputeCovarianceMatrix(const std::vector<Vector3>& points)
{
  /******Student:Assignment2******/
  Matrix3 CovarianceMatrix;
  CovarianceMatrix.ZeroOut();

  float oon = 1.0f / (float)points.size();
  Vector3 c = Vector3(0.0f, 0.0f, 0.0f);
  float e00 = 0, e11 = 0, e22 = 0, e01 = 0, e02 = 0, e12 = 0;

  for (unsigned i = 0; i < points.size(); ++i)
    c += points[i];

    c *= oon;

  for (unsigned i = 0; i < points.size(); ++i)
  {
    Vector3 p = points[i] - c;
    e00 += p.x * p.x;
    e11 += p.y * p.y;
    e22 += p.z * p.z;
    e01 += p.x * p.y;
    e02 += p.x * p.z;
    e12 += p.y * p.z;
  }

  CovarianceMatrix[0][0] = e00 * oon;
  CovarianceMatrix[1][1] = e11 *oon;
  CovarianceMatrix[2][2] = e22 *oon;
  CovarianceMatrix[0][1] = CovarianceMatrix[1][0] = e01 *oon;
  CovarianceMatrix[0][2] = CovarianceMatrix[2][0] = e02 *oon;
  CovarianceMatrix[1][2] = CovarianceMatrix[2][1] = e12 *oon;

  return CovarianceMatrix;
}

// DONE
Matrix3 ComputeJacobiRotation(const Matrix3& matrix)
{
  float absM01, absM02, absM12;
  //float cosValue = 0.0f;
  //float sinValue = 0.0f;
  Matrix3 jac;
  jac.ZeroOut();

  absM01 = Math::Abs(matrix.m01);
  absM02 = Math::Abs(matrix.m02);
  absM12 = Math::Abs(matrix.m12);

  if (absM01 > absM02 && absM01 > absM12)
  {
    float theta = atan((2.0f * matrix.m01) / (matrix.m00 - matrix.m11)) / 2.0f;
    float cosValue = cos(theta);
    float sinValue = sin(theta);
    jac.m00 = cosValue;
    jac.m01 = -sinValue;
    jac.m10 = sinValue;
    jac.m11 = cosValue;
    jac.m22 = 1.f;
  }
  else if (absM02 > absM01 && absM02 > absM12)
  {
    float theta = atan((2.0f * matrix.m02) / (matrix.m22 - matrix.m00)) / 2.0f;
    float cosValue = cos(theta);
    float sinValue = sin(theta);
    jac.m00 = cosValue;
    jac.m02 = sinValue;
    jac.m20 = -sinValue;
    jac.m22 = cosValue;
    jac.m11 = 1.f;
  }
  else //if (absM12 > absM01 && absM12 > absM02)
  {
    float theta = atan((2.0f * matrix.m12) / (matrix.m11 - matrix.m22)) / 2.0f;
    float cosValue = cos(theta);
    float sinValue = sin(theta);

    jac.m11 = cosValue;
    jac.m12 = -sinValue;
    jac.m21 = sinValue;
    jac.m22 = cosValue;
    jac.m00 = 1.f;
  }
  return jac;
}

void ComputeEigenValuesAndVectors(const Matrix3& covariance, Vector3& eigenValues, Matrix3& eigenVectors, int maxIterations)
{
  Matrix3 covmatrix = covariance;
  Matrix3 jacobiMatrix;
  Matrix3 covMatSum;
  std::vector<Matrix3> jacobiMatrixVector;
  int itCount = 0;
  while (1)
  {
    if (Math::Abs(covmatrix.m01) <= 0.00001f && Math::Abs(covmatrix.m02) <= 0.00001f && Math::Abs(covmatrix.m12) <= 0.00001f)
      break;
    if (itCount == MAX_ITERATIONS)
      break;

    jacobiMatrix = ComputeJacobiRotation(covmatrix);
    jacobiMatrixVector.emplace_back(jacobiMatrix);
    covMatSum = jacobiMatrix.Transposed() * covmatrix * jacobiMatrix;
    covmatrix = covMatSum;
  }

  // get the egein values
  eigenValues.x = covMatSum[0][0];
  eigenValues.y = covMatSum[1][1];
  eigenValues.z = covMatSum[2][2];

  eigenVectors.SetIdentity();

  for (auto matrix : jacobiMatrixVector)
  {
    eigenVectors = eigenVectors * matrix;
  }
}

//-----------------------------------------------------------------------------Sphere
Sphere::Sphere()
{
  mCenter = Vector3::cZero;
  mRadius = 0;
}

Sphere::Sphere(const Vector3& center, float radius)
{
  mCenter = center;
  mRadius = radius;
}

// DONE
void Sphere::ComputeCentroid(const std::vector<Vector3>& points)
{
  /******Student:Assignment2******/
 // Vector3 centroid = Vector3(0, 0, 0);
 // double signedArea = 0.0;
 // double x0 = 0.0; // Current vertex X
 // double y0 = 0.0; // Current vertex Y
 // double z0 = 0.0;
 // double x1 = 0.0; // Next vertex X
 // double y1 = 0.0; // Next vertex Y
 // double z1 = 0.0;
 // double a = 0.0;  // Partial signed area

 // int i = 0;
 // for (; i < points.size() -1 ; ++i)
 // {
 //   x0 = points[i].x;
 //   y0 = points[i].y;
 //   z0 = points[i].z;
 //   x1 = points[i + 1].x;
 //   y1 = points[i + 1].y;
 //   z1 = points[i + 1].z;
 //   a = x0*y1*z0 - x1*y0*z1;
 //   signedArea += a;
 //   centroid.x += (x0 + x1)*a;
 //   centroid.y += (y0 + y1)*a;
 //   centroid.z += (z0 + z1)*a;

 // }

 // x0 = points[i].x;
 // y0 = points[i].y;
 // z0 = points[i].z;

 // x1 = points[0].x;
 // y1 = points[0].y;
 // z1 = points[0].z;

 //// a = x0*y1 - x1*y0;
 // a = x0*y1*z0 - x1*y0*z1;

 // signedArea += a;
 // centroid.x += (x0 + x1)*a;
 // centroid.y += (y0 + y1)*a;
 // centroid.z += (z0 + z1)*a;

 // signedArea *= 0.5;
 // centroid.x /= (6.0*signedArea);
 // centroid.y /= (6.0*signedArea);
 // centroid.z /= (6.0*signedArea);


 // mCenter = centroid;
  float minX, maxX, minY, maxY, minZ, maxZ;
  minX = maxX = points[0].x;
  minY = maxY = points[0].y;
  minZ = maxZ = points[0].z;
  for (unsigned i = 0; i < points.size(); ++i)
  {
    if (points[i].x < minX)
      minX = points[i].x;
    if (points[i].y < minY)
      minY = points[i].y;
    if (points[i].z < minZ)
      minZ = points[i].z;
    if (points[i].x > maxX)
      maxX = points[i].x;
    if (points[i].y > maxY)
      maxY = points[i].y;
    if (points[i].z > maxZ)
      maxZ = points[i].z;
  }

  // We construct an AABB with these min max value and get its center
  Aabb AABB(Vector3(minX,minY,minZ), Vector3(maxX, maxY,maxZ));
  mCenter = AABB.GetCenter();
  // initialize the radius
  float radius = Math::LengthSq(mCenter - points[0]);
  for (unsigned i = 0; i < points.size(); ++i)
  {
    float r = Math::LengthSq(mCenter - points[i]);
    if (r > radius)
      radius = r;
  }

  mRadius = Math::Sqrt(radius);
}

// DONE 1 Test Wrong
void Sphere::ComputeRitter(const std::vector<Vector3>& points)
{
  // REFERED to orange book
  /******Student:Assignment2******/
  // Comput the two most separted points
  int minX = 0, maxX = 0, minY = 0, maxY = 0, minZ = 0, maxZ = 0;
  int min, max;
  for (unsigned i = 0; i < points.size(); ++i)
  {
    if (points[i].x <= points[minX].x) minX = i;
    if (points[i].x >= points[maxX].x) maxX = i;
    if (points[i].y <= points[minY].y) minY = i;
    if (points[i].y >= points[maxY].y) maxY = i;
    if (points[i].z <= points[minZ].z) minZ = i;
    if (points[i].z >= points[maxZ].z) maxZ = i;
  }

  float dist2x = Math::Dot(points[maxX] - points[minX], points[maxX] - points[minX]);
  float dist2y = Math::Dot(points[maxY] - points[minY], points[maxY] - points[minY]);
  float dist2z = Math::Dot(points[maxZ] - points[minZ], points[maxZ] - points[minZ]);
 // float min, max, radius;


  //if (dist2x > dist2y && dist2x > dist2z)
  //{
  //  max = maxX;
  //  min = minX;
  //  //center = (points[maxX] + points[minX])* 0.5f;
  //  //radius = (points[maxX] - points[minX]).Length() * 0.5f;
  //}

  min = minX;
  max = maxX;

  if (dist2y >= dist2x && dist2y >= dist2z)
  {
    max = maxY;
    min = minY;
  }
  if (dist2z >= dist2x && dist2z >= dist2y)
  {
    max = maxZ;
    min = minZ;
  }
  

  mCenter =  (points[min] + points[max]) * 0.5f;
  float rad = Math::Dot(points[max] - mCenter, points[max] - mCenter);
  mRadius = Math::Sqrt(rad);

  for (unsigned i = 0; i < points.size(); ++i)
  {
    Vector3 d = points[i] - mCenter;
    float dist2 = Math::Dot(d, d);
    if (dist2 > mRadius * mRadius)
    {
      float dist = Math::Sqrt(dist2);
      float newRadius = (mRadius + dist) * 0.5f;
      float k = (newRadius - mRadius) / dist;
      mRadius = newRadius;
      mCenter += d * k;
    }
  }
}

// DONE
void Sphere::ComputePCA(const std::vector<Vector3>& points)
{
  /******Student:Assignment2******/
  Matrix3 covarianceMatrix = ComputeCovarianceMatrix(points);
  Vector3 eigenValues;
  Matrix3 eigenVectors;
  ComputeEigenValuesAndVectors(covarianceMatrix, eigenValues, eigenVectors, MAX_ITERATIONS);

  Vector3 vect;
  Vector3 v1, v2, v3;
  v1 = eigenVectors.BasisX();
  v2 = eigenVectors.BasisY();
  v3 = eigenVectors.BasisZ();
  // find the biggest eigbn value
  Vector3 spreadingAxis;

  if (eigenValues[0] > eigenValues[1] && eigenValues[0] > eigenValues[2])
    spreadingAxis = v1;
  else if (eigenValues[1] > eigenValues[0] && eigenValues[1] > eigenValues[2])
    spreadingAxis = v2;
  else if (eigenValues[2] > eigenValues[1] && eigenValues[2] > eigenValues[0])
    spreadingAxis = v3;

  // find two furtherest pointss on the axis
  Vector3 min, max;
  min =  max = points[0];
  float dotProduct;
  float maxdist = Dot(max, spreadingAxis);
  float mindist = maxdist;
 

  for (auto point :points)
  {
    dotProduct = Dot(point, spreadingAxis);

    if (dotProduct > maxdist)
    {
      max = point;
      maxdist = dotProduct;
    }

    else if (dotProduct < mindist)
    {
      min = point;
      mindist = dotProduct;
    }
  }

  mCenter = (min + max) / 2.0f;
  mRadius = Length(min - max) / 2.0f;

  for (auto point : points)
  {
    if (!ContainsPoint(point))
    {
      Vector3 c;
      Vector3 b, pc;
      float r;
      pc = (point - mCenter).Normalized();
      b = mCenter - (pc * mRadius);
      c = (point + b) * 0.5f;
      r = (point - b).Length() * 0.5f;
      mCenter = c;
      mRadius = r;
    }
  }
}

bool Sphere::ContainsPoint(const Vector3& point)
{
  return PointSphere(point, mCenter, mRadius);
}

Vector3 Sphere::GetCenter() const
{
  return mCenter;
}

float Sphere::GetRadius() const
{
  return mRadius;
}

bool Sphere::Compare(const Sphere& rhs, float epsilon) const
{
  float posDiff = Math::Length(mCenter - rhs.mCenter);
  float radiusDiff = Math::Abs(mRadius - rhs.mRadius);

  return posDiff < epsilon && radiusDiff < epsilon;
}

DebugShape& Sphere::DebugDraw() const
{
  return gDebugDrawer->DrawSphere(*this);
}

//-----------------------------------------------------------------------------Aabb
Aabb::Aabb()
{
  //set the aabb to an initial bad value (where the min is smaller than the max)
  mMin.Splat(Math::PositiveMax());
  mMax.Splat(-Math::PositiveMax());
}

Aabb::Aabb(const Vector3& min, const Vector3& max)
{
  mMin = min;
  mMax = max;
}

Aabb Aabb::BuildFromCenterAndHalfExtents(const Vector3& center, const Vector3& halfExtents)
{
  return Aabb(center - halfExtents, center + halfExtents);
}

// DONE
float Aabb::GetVolume() const
{
  /******Student:Assignment2******/
  // V = W*H*D
  float w = Math::Abs(GetHalfSize().x)* 2.0f;
  float h = Math::Abs(GetHalfSize().y)* 2.0f;
  float d = Math::Abs(GetHalfSize().z)* 2.0f;

  return w * h* d;
}

float Aabb::GetSurfaceArea() const
{
  /******Student:Assignment2******/
  float w = Math::Abs(GetHalfSize().x)* 2.0f;
  float h = Math::Abs(GetHalfSize().y)* 2.0f;
  float d = Math::Abs(GetHalfSize().z)* 2.0f;

  return (w * h *2.0f) + (w * d * 2.0f) + (h * d * 2.0f);
}

// DONE
bool Aabb::Contains(const Aabb& aabb) const
{
  /******Student:Assignment2******/
  if (GetMin().x > aabb.GetMin().x || GetMax().x < aabb.GetMax().x)
    return false;
  if (GetMin().y > aabb.GetMin().y || GetMax().y < aabb.GetMax().y)
    return false;
  if (GetMin().z > aabb.GetMin().z || GetMax().z < aabb.GetMax().z)
    return false;

  return true;
}

void Aabb::Expand(const Vector3& point)
{
  for(size_t i = 0; i < 3; ++i)
  {
    mMin[i] = Math::Min(mMin[i], point[i]);
    mMax[i] = Math::Max(mMax[i], point[i]);
  }
}

Aabb Aabb::Combine(const Aabb& lhs, const Aabb& rhs)
{
  Aabb result;
  for(size_t i = 0; i < 3; ++i)
  {
    result.mMin[i] = Math::Min(lhs.mMin[i], rhs.mMin[i]);
    result.mMax[i] = Math::Max(lhs.mMax[i], rhs.mMax[i]);
  }
  return result;
}

bool Aabb::Compare(const Aabb& rhs, float epsilon) const
{
  float pos1Diff = Math::Length(mMin - rhs.mMin);
  float pos2Diff = Math::Length(mMax - rhs.mMax);

  return pos1Diff < epsilon && pos2Diff < epsilon;
}

// DONE
void Aabb::Transform(const Vector3& scale, const Matrix3& rotation, const Vector3& translation)
{
  /******Student:Assignment2******/
  // Scale Rot Trans
  // get min max from half extents and center
  Vector3 c = GetCenter() * scale;
  Vector3 center = Math::Transform(rotation, c);
  center += translation;

  Vector3 hfextent = GetHalfSize() * scale;
  Vector3 halfExtents;
  halfExtents.x = Math::Abs(rotation.m00 * hfextent.x) +
    Math::Abs(rotation.m01 * hfextent.y) + Math::Abs(rotation.m02 * hfextent.z);
  halfExtents.y = Math::Abs(rotation.m10 * hfextent.x) +
    Math::Abs(rotation.m11 * hfextent.y) + Math::Abs(rotation.m12 * hfextent.z);
  halfExtents.z = Math::Abs(rotation.m20 * hfextent.x) +
    Math::Abs(rotation.m21 * hfextent.y) + Math::Abs(rotation.m22 * hfextent.z);
 
  mMax = center + halfExtents;
  mMin = center - halfExtents;
}

Vector3 Aabb::GetMin() const
{
  return mMin;
}

Vector3 Aabb::GetMax() const
{
  return mMax;
}

Vector3 Aabb::GetCenter() const
{
  return (mMin + mMax) * 0.5f;
}

Vector3 Aabb::GetHalfSize() const
{
  return (mMax - mMin) * 0.5f;
}

DebugShape& Aabb::DebugDraw() const
{
  return gDebugDrawer->DrawAabb(*this);
}

//-----------------------------------------------------------------------------Triangle
Triangle::Triangle()
{
  mPoints[0] = mPoints[1] = mPoints[2] = Vector3::cZero;
}

Triangle::Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
  mPoints[0] = p0;
  mPoints[1] = p1;
  mPoints[2] = p2;
}

DebugShape& Triangle::DebugDraw() const
{
  return gDebugDrawer->DrawTriangle(*this);
}

//-----------------------------------------------------------------------------Plane
Plane::Plane()
{
  mData = Vector4::cZero;
}

Plane::Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
  Set(p0, p1, p2);
}

Plane::Plane(const Vector3& normal, const Vector3& point)
{
  Set(normal, point);
}

void Plane::Set(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
  /******Student:Assignment1******/
  // createt the plance
  Vector3 v1 = p1 - p0;
  Vector3 v2 = p2 - p0;
  Vector3 cross = v1.Cross(v2);
  cross.Normalize();
  mData.x = cross.x;
  mData.y = cross.y;
  mData.z = cross.z;
  mData.w = cross.Dot(p0);
}

void Plane::Set(const Vector3& normal, const Vector3& point)
{
  /******Student:Assignment1******/
  Vector3 n = normal.Normalized();
  mData.x = n.x;
  mData.y = n.y;
  mData.z = n.z;
  mData.w = n.Dot(point);
}

Vector3 Plane::GetNormal() const
{
  return Vector3(mData.x, mData.y, mData.z);
}

float Plane::GetDistance() const
{
  return mData.w;
}

DebugShape& Plane::DebugDraw(float size) const
{
  return DebugDraw(size, size);
}

DebugShape& Plane::DebugDraw(float sizeX, float sizeY) const
{
  return gDebugDrawer->DrawPlane(*this, sizeX, sizeY);
}

//-----------------------------------------------------------------------------Frustum
void Frustum::Set(const Vector3& lbn, const Vector3& rbn, const Vector3& rtn, const Vector3& ltn,
                  const Vector3& lbf, const Vector3& rbf, const Vector3& rtf, const Vector3& ltf)
{
  mPoints[0] = lbn;
  mPoints[1] = rbn;
  mPoints[2] = rtn;
  mPoints[3] = ltn;
  mPoints[4] = lbf;
  mPoints[5] = rbf;
  mPoints[6] = rtf;
  mPoints[7] = ltf;

  //left
  mPlanes[0].Set(lbf, ltf, lbn);
  //right
  mPlanes[1].Set(rbn, rtf, rbf);
  //top
  mPlanes[2].Set(ltn, ltf, rtn);
  //bot
  mPlanes[3].Set(rbn, lbf, lbn);
  //near
  mPlanes[4].Set(lbn, ltn, rbn);
  //far
  mPlanes[5].Set(rbf, rtf, lbf);
}

Math::Vector4* Frustum::GetPlanes() const
{
  return (Vector4*)mPlanes;
}

DebugShape& Frustum::DebugDraw() const
{
  return gDebugDrawer->DrawFrustum(*this);
}

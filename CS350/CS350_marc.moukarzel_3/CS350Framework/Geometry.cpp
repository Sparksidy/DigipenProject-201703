///////////////////////////////////////////////////////////////////////////////
///
/// Authors: Joshua Davis
/// Copyright 2015, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#include "Geometry.hpp"
#include "DebugDraw.hpp"
#include "Application.hpp"

// DONE Passed TEST
Vector3 ProjectPointOnPlane(const Vector3& point, const Vector3& normal, float planeDistance)
{
  /******Student:Assignment1******/
  // Make a vector from origin to point of interest 
  float v = Vector4(normal.x, normal.y, normal.z, planeDistance).Dot(Vector4(point.x, point.y, point.z, -1.f));
  return Vector3(point - (v * normal.Normalized()));
}
// DONE Passed TEST
bool BarycentricCoordinates(const Vector3& point, const Vector3& a, const Vector3& b,
                            float& u, float& v, float epsilon)
{
  /******Student:Assignment1******/
  // mP = uA + vB + wC  -> u + v + w = 1 -> (u,v,w) bary coord
  // using method 2 for barycentric coordinates 
  // we find u as fraction
  float n = (point - b).Dot(a - b);
  float d = (a - b).Dot(a - b);
  // Check for 0 denominator
  if (d == 0.0f -epsilon)
    return false;
  else
  {
    u = n / d;
    v = 1.0f - u;
    if ((u >= 0.0f - epsilon && u <= 1.0f + epsilon) && (v >= 0.0f - epsilon && v <= 1.0f + epsilon))
    {
      return true;
    }
  }

  return false;
}

// DONE Passed TEST
bool BarycentricCoordinates(const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c,
                            float& u, float& v, float& w, float epsilon)
{
  /******Student:Assignment1******/
  // ORANGE BOOK refer
  // Use signed triangle method
  //float SignedAreaPBC = Math::Dot(Cross(b - point, c - point), Normalized((Cross(b - a, c - a))));
  //float SignedAreaPCA = Math::Dot(Cross(c - point, a - point), Normalized((Cross(c - b, a - b))));
  //float SignedAreaABC = Math::Dot(Cross(b - a, c - a), Normalized((Cross(b - point, c - point))));
  //  // Compute u,v,w
  //u = SignedAreaPBC / SignedAreaABC;
  //v = SignedAreaPCA / SignedAreaABC;
  //w = 1.0f - u - v;

  Vector3 normalabc = (b - a).Cross(c - a);
  Vector3 normalpbc = (b - point).Cross(c - point);
  Vector3 normalpca = (c - point).Cross(a - point);

  float SignedAreaABC = normalabc.Dot(normalabc.Normalized());
  float SignedAreaPBC = normalpbc.Dot(normalabc.Normalized());
  float SignedAreaPCA = normalpca.Dot(normalabc.Normalized());

  if (SignedAreaABC == 0.0f - epsilon|| SignedAreaPBC == 0.0f - epsilon
    || SignedAreaPCA == 0.0f - epsilon)
    return false;
    

  u = SignedAreaPBC / SignedAreaABC ;
  v = SignedAreaPCA / SignedAreaABC ;
  w = 1 - u - v;

  if ((u >= 0.0f-epsilon && u <= 1.0f +epsilon) &&
    (v >= 0.0f-epsilon && v <= 1.0f +epsilon) &&
    (w >= 0.0f-epsilon && w <= 1.0f+epsilon))
    return true;
  else
    return false;

  return false;
}

// DONE Passed TEST
IntersectionType::Type PointPlane(const Vector3& point, const Vector4& plane, float epsilon)
{
  /******Student:Assignment1******/
  Vector4 ptemp;
  Vector4 planetemp;

  ptemp = Vector4(point.x, point.y, point.z, -1.0f);
  planetemp = plane;
  // find w and compare it with epsilon to see where the point is
  float w = planetemp.Dot(ptemp);

  if (w > epsilon)
    return IntersectionType::Type::Inside;
  else if (w < -epsilon)
    return IntersectionType::Type::Outside;
  else if (w <= epsilon)
    return IntersectionType::Type::Coplanar;

  return IntersectionType::Overlaps;

}

// DONE Passed TEST
bool PointSphere(const Vector3& point, const Vector3& sphereCenter, float sphereRadius)
{
  /******Student:Assignment1******/
  Vector3 v = sphereCenter - point;
  float dist = v.Length();

  if (dist > sphereRadius)
    return false;
  else
    return true;
}

// DONE Passed TEST
bool PointAabb(const Vector3& point, const Vector3& aabbMin, const Vector3& aabbMax)
{
  /******Student:Assignment1******/
  if ((point.x >= aabbMin.x && point.x <= aabbMax.x) 
    && (point.y <= aabbMax.y && point.y >= aabbMin.y)
    && (point.z >= aabbMin.z && point.z <= aabbMax.z))
    return true;
  else
    return false;
}

// CLOSE 2 Tests Wrong
bool RayPlane(const Vector3& rayStart, const Vector3& rayDir,
              const Vector4& plane, float& t, float epsilon)
{
  // TODO: Fix something with epsilon boundaries
  ++Application::mStatistics.mRayPlaneTests;
  /******Student:Assignment1******/
  Vector3 planeNormal = Vector3(plane.x, plane.y, plane.z);
  float n = plane.w - Dot(rayStart,planeNormal);
  float d = Dot(rayDir,planeNormal);

  if (d >= 0.0f - epsilon && d <= epsilon)
    return false;
  else
    t = n / d;

  if (t >= epsilon )
    return true;
  else
    return false;
}

// DONE Passed TEST
bool RayTriangle(const Vector3& rayStart, const Vector3& rayDir,
                 const Vector3& triP0, const Vector3& triP1, const Vector3& triP2,
                 float& t, float triExpansionEpsilon)
{

  ++Application::mStatistics.mRayTriangleTests;
  /******Student:Assignment1******/
  // Perform Ray vs Plane
  // Create the plane from the triagle points
  Plane plane;
  plane.Set(triP0, triP1, triP2);
  Vector4 planevec(plane.mData);

  bool result = RayPlane(rayStart, rayDir, planevec, t);
  // if there is intersection we then we use bary coord to check point
  if (result == true)
  {
    // we use the barycentric function
    // get the pos of the point
    Vector3 point = rayStart + t * rayDir;
    Vector3 a, b, c;
    a = triP0;
    b = triP1;
    c = triP2;

    a -= point;
    b -= point;
    c -= point;

    Vector3 u = Cross(b, c);
    Vector3 v = Cross(c, a);
    if (Dot(u, v) < 0.0f - triExpansionEpsilon)
      return false;

    Vector3 w = Cross(a, b);
    if (Dot(u, w) < 0.0f - triExpansionEpsilon)
      return false;

    return true;
  }
  return false;
}

// DONE Passed TEST
bool RaySphere(const Vector3& rayStart, const Vector3& rayDir,
               const Vector3& sphereCenter, float sphereRadius,
               float& t)
{
  ++Application::mStatistics.mRaySphereTests;
  /******Student:Assignment1******/
  //Vector3 m = rayStart - sphereCenter;
  //float len = m.Length();

  //if (len <= sphereRadius)
  //{
  //  t = 0.0f;
  //  return true;
  //}

  //float b = Dot(m, rayDir);
  //float c = Dot(m, m) - sphereRadius * sphereRadius;

  //float a = Dot(rayDir, rayDir);

  //if (c > 0.0f && b > 0.0f)
  //  return false;

  //float disc = (b * b) - (a*c);
  //// No intersection between ray and sphere

  //if (c > 0.0f  && b > 0.0f)
  //  return false;

  //if (disc < 0.0f)
  //  return false;

  //else if (disc == 0.0f)
  //{
  //  t = (-b - Math::Sqrt(disc)) / a;
  //  if (t >= 0.0f)
  //    return true;
  //  else
  //    return false;
  //}
  //else
  //{
  //  // we have two intersections
  //  float intersect_1;
  //  float intersect_2;

  //  intersect_1 = (-b + Math::Sqrt(disc)) / a;
  //  intersect_2 = (-b - Math::Sqrt(disc)) / a;

  //  float smallerval = intersect_1;
  //  if (intersect_1 < intersect_2)
  //  {
  //    smallerval = intersect_1;
  //  }
  //  else
  //  {
  //    smallerval = intersect_2;
  //  }

  //  if (t < 0.0f)
  //  {
  //    t = 0.0f; // clamp its inside
  //    return false;
  //  }
  //  else
  //  {
  //    t = smallerval;
  //    return true;
  //  }
  //}
  //return false;

// ray is starting inside the sphere
  float distanceCenterStart = (sphereCenter - rayStart).Length();
  if (distanceCenterStart <= sphereRadius)
  {
    t = 0.f;
    return true;
  }
  // discriminant term
  float a = rayDir.Dot(rayDir);
  float b = -2.f * (sphereCenter - rayStart).Dot(rayDir);
  float c = (sphereCenter - rayStart).Dot(sphereCenter - rayStart) - (sphereRadius * sphereRadius);

  float discriminat = (b * b) - (4.f * a * c);
  if (discriminat < 0)
  {
    return false;
  }
  else if (discriminat == 0)
  {
    // one intersection
    t = (-b + Math::Sqrt(discriminat)) / (2.0f * a);

    if (t >= 0.f)
      return true;
    else
      return false;
  }
  else if (discriminat > 0)
  {
    // two intersection
    float t1, t2;
    t1 = (-b - Math::Sqrt(discriminat)) / (2.0f * a);
    t2 = (-b + Math::Sqrt(discriminat)) / (2.0f * a);

    float lesser = t1;
    if (t2 < t1)
      lesser = t2;

    if (lesser >= 0.f)
    {
      t = lesser;
      return true;
    }
    else
      return false;
  }

  return false;
}

// DONE Passed TEST
bool RayAabb(const Vector3& rayStart, const Vector3& rayDir,
             const Vector3& aabbMin, const Vector3& aabbMax, float& t)
{
  ++Application::mStatistics.mRayAabbTests;
  /******Student:Assignment1******/
  // Case ray is inside
  if ((aabbMin.x <= rayStart.x && rayStart.x <= aabbMax.x) &&
    (aabbMin.y <= rayStart.y && rayStart.y <= aabbMax.y) &&
    (aabbMin.z <= rayStart.z && rayStart.z <= aabbMax.z))
  {
    t = 0.0f;
    return true;
  }

  float xMin = (aabbMin.x - rayStart.x) / rayDir.x;
  float xMax = (aabbMax.x - rayStart.x) / rayDir.x;
  // Check for swaps
  if (rayDir.x < 0.0f)
  {
    float tmp;
tmp = xMin;
xMin = xMax;
xMax = tmp;
  }
  else if (rayDir.x == 0.0f)
  {
    if (aabbMin.x > rayStart.x)
      return false;

    if (rayStart.x > aabbMax.x)
      return false;
  }


  float yMin = (aabbMin.y - rayStart.y) / rayDir.y;
  float yMax = (aabbMax.y - rayStart.y) / rayDir.y;
  if (rayDir.y < 0.0f)
  {
    float tmp;
    tmp = yMin;
    yMin = yMax;
    yMax = tmp;
  }
  else if (rayDir.y == 0.0f)
  {
    if (aabbMin.y > rayStart.y)
      return false;

    if (rayStart.y > aabbMax.y)
      return false;
  }

  float zMin = (aabbMin.z - rayStart.z) / rayDir.z;
  float zMax = (aabbMax.z - rayStart.z) / rayDir.z;
  if (rayDir.z < 0.0f)
  {
    float tmp;
    tmp = zMin;
    zMin = zMax;
    zMax = tmp;
  }
  else if (rayDir.z == 0.0f)
  {
    if (aabbMin.z > rayStart.z)
      return false;

    if (rayStart.z > aabbMax.z)
      return false;
  }

  float tMin = Math::Max(Math::Max(xMin, yMin), zMin);
  float tMax = Math::Min(Math::Min(xMax, yMax), zMax);

  // no collision
  if (tMin > tMax)
    return false;
  else if (tMin <= tMax)
  {
    t = tMin;
    if (tMin >= 0.0f)
      return true;
  }

  return false;
}

// DONE Passed TEST
IntersectionType::Type PlaneTriangle(const Vector4& plane,
  const Vector3& triP0, const Vector3& triP1, const Vector3& triP2,
  float epsilon)
{
  ++Application::mStatistics.mPlaneTriangleTests;
  /******Student:Assignment1******/
  IntersectionType::Type type1;
  IntersectionType::Type type2;
  IntersectionType::Type type3;
  type1 = PointPlane(triP0, plane, epsilon);
  type2 = PointPlane(triP1, plane, epsilon);
  type3 = PointPlane(triP2, plane, epsilon);

  // Cop = 0
  //out = 1
  // in = 2
  // over = 3
  if (type1 == IntersectionType::Coplanar &&
    type2 == IntersectionType::Coplanar &&
    type3 == IntersectionType::Coplanar)
    return IntersectionType::Coplanar;
  else if (type1 == IntersectionType::Outside &&
    type2 == IntersectionType::Outside &&
    type3 == IntersectionType::Outside)
    return IntersectionType::Outside;
  else if (type1 == IntersectionType::Inside &&
    type2 == IntersectionType::Inside &&
    type3 == IntersectionType::Inside)
    return IntersectionType::Inside;

  bool cinoutcheck[3] = { false, false, false };

  if (type1 == IntersectionType::Coplanar)
    cinoutcheck[0] = true;
  if (type1 == IntersectionType::Inside)
    cinoutcheck[1] = true;
  if (type1 == IntersectionType::Outside)
    cinoutcheck[2] = true;
  if (type2 == IntersectionType::Coplanar)
    cinoutcheck[0] = true;
  if (type2 == IntersectionType::Inside)
    cinoutcheck[1] = true;
  if (type2 == IntersectionType::Outside)
    cinoutcheck[2] = true;
  if (type3 == IntersectionType::Coplanar)
    cinoutcheck[0] = true;
  if (type3 == IntersectionType::Inside)
    cinoutcheck[1] = true;
  if (type3 == IntersectionType::Outside)
    cinoutcheck[2] = true;
  if ((cinoutcheck[0] && cinoutcheck[1] && cinoutcheck[2]))
    return IntersectionType::Overlaps;
  else if (cinoutcheck[0] && cinoutcheck[2])
    return IntersectionType::Outside;
  else if (cinoutcheck[0] && cinoutcheck[1])
    return IntersectionType::Inside;
  else if (cinoutcheck[1] && cinoutcheck[2])
    return IntersectionType::Overlaps;
  return IntersectionType::Outside;
}

// DONE Passed TEST
IntersectionType::Type PlaneSphere(const Vector4& plane,
                                   const Vector3& sphereCenter, float sphereRadius)
{
  ++Application::mStatistics.mPlaneSphereTests;
  /******Student:Assignment1******/
  // Project sphere center on plane 
  // test if that point is inside sphere
  IntersectionType::Type r = PointPlane(sphereCenter, plane, sphereRadius);
  if (r == IntersectionType::Type::Coplanar)
    return IntersectionType::Overlaps;
  else
    return r;
}

// DONE Passed TEST
IntersectionType::Type PlaneAabb(const Vector4& plane,
                                 const Vector3& aabbMin, const Vector3& aabbMax)
{
  ++Application::mStatistics.mPlaneAabbTests;
  /******Student:Assignment1******/
  // Refered to http://www.gamedev.net/topic/646404-box-vs-plane-collision-detection/

  // Represent the box by a min and max vector and a plane normal
  Vector3 vec1;
  Vector3 vec2;
  float minX, maxX;
  float minY, maxY;
  float minZ, maxZ;
  Vector3 aabbCenter;
  Vector3 hlfExtents;
  Vector3 normal(plane.x, plane.y, plane.z);
  aabbCenter = (aabbMin + aabbMax) / 2.0f;
  hlfExtents = aabbMax - aabbCenter;
  minX = aabbCenter.x + hlfExtents.x;
  maxX = aabbCenter.x - hlfExtents.x;
  minY = aabbCenter.y + hlfExtents.y;
  maxY = aabbCenter.y - hlfExtents.y;
  minZ = aabbCenter.z + hlfExtents.z;
  maxZ = aabbCenter.z - hlfExtents.z;

  if (normal.x >= 0.0f)
  {
    vec1.x = minX;
    vec2.x = maxX;
  }
  else
  {
    vec1.x = maxX;
    vec2.x = minX;
  }
  if (normal.y >= 0.0f)
  {
    vec1.y = minY;
    vec2.y = maxY;
  }
  else
  {
    vec1.y = maxY;
    vec2.y = minY;
  }
  if (normal.z >= 0.0f)
  {
    vec1.z = minZ;
    vec2.z = maxZ;
  }
  else
  {
    vec1.z = maxZ;
    vec2.z = minZ;
  }
  //float posSide = (normal.x * vec2.x) + (normal.y * vec2.y) +
  //  (normal.y * vec2.y) + plane.w;
  //if (posSide > 0)
  //  return IntersectionType::Outside;
  //float negSide = (plane.x * vec1.x)
  //  + (normal.y * vec1.y) + (normal.y * vec1.y) + normal.w;
  //if (negSide < 0)
  //  return IntersectionType::Outside;

  // Use these points and check where they lie on the plane
  // if both are in the aabb is in, is both are out the the aabb lies outside
  // if one in one out the overlaps
  IntersectionType::Type type1, type2;
  type1 = PointPlane(vec1, plane, 0.0f);
  type2 = PointPlane(vec2, plane, 0.0f);

  if (type1 == IntersectionType::Inside &&
    type2 == IntersectionType::Inside)
    return IntersectionType::Inside;
  else if (type1 == IntersectionType::Outside &&
    type2 == IntersectionType::Outside)
    return IntersectionType::Outside;
  else if ((type1 == IntersectionType::Inside &&
    type2 == IntersectionType::Outside) ||
    (type1 == IntersectionType::Outside &&
    type2 == IntersectionType::Inside))
    return IntersectionType::Overlaps;

  // Warning
  return IntersectionType::Overlaps;
}
// DONE Passed TEST
IntersectionType::Type FrustumTriangle(const Vector4 planes[6],
                                       const Vector3& triP0, const Vector3& triP1, const Vector3& triP2,
                                       float epsilon)
{
  ++Application::mStatistics.mFrustumTriangleTests;
  /******Student:Assignment1******/
  //
  int lastaxis = 0;
  IntersectionType::Type interstionType;
  // loop through the 6 planes 
  for (unsigned i = 0; i < 6; ++i)
  {
    interstionType = PlaneTriangle(planes[i], triP0, triP1, triP2, epsilon);//PlaneAabb(planes[i], aabbMin, aabbMax);

    if (interstionType == IntersectionType::Outside)
      return IntersectionType::Outside;

    if (interstionType == IntersectionType::Overlaps)
      lastaxis++;
  }
  if (lastaxis >= 1)
    return IntersectionType::Overlaps;

  return IntersectionType::Inside;

}
// DONE Passed TEST
IntersectionType::Type FrustumSphere(const Vector4 planes[6],
                                     const Vector3& sphereCenter, float sphereRadius, size_t& lastAxis)
{
  ++Application::mStatistics.mFrustumSphereTests;
  /******Student:Assignment1******/

  // We should tests for all 6 planes for interections and where 
  // the sphere lies
  IntersectionType::Type interstionType;
  for (unsigned i = 0; i < 6; ++i)
  {
    interstionType = PlaneSphere(planes[i], sphereCenter, sphereRadius);
    if (interstionType == IntersectionType::Outside)
      return IntersectionType::Outside;

    if (interstionType == IntersectionType::Overlaps)
      lastAxis++;

  } 
  if (lastAxis >= 1)
    return IntersectionType::Overlaps;

  return IntersectionType::Inside;
}

// DONE Passed TEST
IntersectionType::Type FrustumAabb(const Vector4 planes[6],
                                   const Vector3& aabbMin, const Vector3& aabbMax, size_t& lastAxis)
{
  ++Application::mStatistics.mFrustumAabbTests;
  /******Student:Assignment1******/
  // Same As sphere 
  // Test all 6 planes
  // if anyoutside reutrn outside
  // if all in retrun in 
  // eslse overlap
  IntersectionType::Type interstionType;

  // loop through the 6 planes 
  for (unsigned i = 0; i < 6; ++i)
  {
    interstionType = PlaneAabb(planes[i], aabbMin, aabbMax);

    if (interstionType == IntersectionType::Outside)
      return IntersectionType::Outside;
    if (interstionType == IntersectionType::Overlaps)
      lastAxis++;

  }
  if (lastAxis >= 1)
    return IntersectionType::Overlaps;

  return IntersectionType::Inside;
}

// DONE Passed TEST
bool SphereSphere(const Vector3& sphereCenter0, float sphereRadius0,
                  const Vector3& sphereCenter1, float sphereRadius1)
{
  ++Application::mStatistics.mSphereSphereTests;
  /******Student:Assignment1******/
  //Compute the squared radius
  float sqrdradii = (sphereRadius0 + sphereRadius1) * (sphereRadius0 + sphereRadius1);
  Vector3 dist = sphereCenter1 - sphereCenter0;
  float distancesqrd = dist.x * dist.x + dist.y * dist.y + dist.z * dist.z;

  if (distancesqrd <= sqrdradii)
    return true;
  else
    return false;
}

// DONE Passed TEST
bool AabbAabb(const Vector3& aabbMin0, const Vector3& aabbMax0,
              const Vector3& aabbMin1, const Vector3& aabbMax1)
{
  ++Application::mStatistics.mAabbAabbTests;
  /******Student:Assignment1******/

  // Check for wrong coll
  if (aabbMin1.x > aabbMax0.x || aabbMin0.x > aabbMax1.x)
    return false;
  if (aabbMin1.y > aabbMax0.y || aabbMin0.y > aabbMax1.y)
    return false;
  if (aabbMin1.z > aabbMax0.z || aabbMin0.z > aabbMax1.z)
    return false;

  return true;
}

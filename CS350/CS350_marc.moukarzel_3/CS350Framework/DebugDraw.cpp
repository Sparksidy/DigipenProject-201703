///////////////////////////////////////////////////////////////////////////////
///
/// Authors: Joshua Davis
/// Copyright 2015, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#include "DebugDraw.hpp"

#include "Application.hpp"
#include "Main/Support.hpp"
#include "Math/MathFunctions.hpp"

#define ShowDebugDrawWarnings true
DebugDrawer* gDebugDrawer = new DebugDrawer();

//-----------------------------------------------------------------------------DebugShape
DebugShape::DebugShape()
{
  mColor = Vector4(.6f);
  mMask = (unsigned int)-1;
  mTimer = 0;
  mOnTop = false;
  mTransform.SetIdentity();
}

DebugShape& DebugShape::Color(const Vector4& color)
{
  mColor = color;
  return *this;
}

DebugShape& DebugShape::OnTop(bool state)
{
  mOnTop = state;
  return *this;
}

DebugShape& DebugShape::Time(float time)
{
  mTimer = time;
  return *this;
}

DebugShape& DebugShape::SetMaskBit(int bitIndex)
{
  mMask = 1 << bitIndex;
  return *this;
}

DebugShape& DebugShape::SetTransform(const Matrix4& transform)
{
  mTransform = transform;
  return *this;
}

//-----------------------------------------------------------------------------DebugDrawer
DebugDrawer::DebugDrawer()
{
  mActiveMask = (unsigned int)-1;
  mApplication = NULL;
}

void DebugDrawer::Update(float dt)
{
  std::vector<DebugShape> newShapes;
  for(size_t i = 0; i < mShapes.size(); ++i)
  {
    DebugShape& shape = mShapes[i];
    shape.mTimer -= dt;

    // If the shape still has time left then add it to the list of shapes to keep drawing,
    // anything that has a timer that ran out will not be in the new list
    if(shape.mTimer >= 0)
      newShapes.push_back(shape);
  }

  mShapes.swap(newShapes);
}

void DebugDrawer::Draw()
{
  for(size_t i = 0; i < mShapes.size(); ++i)
  {
    DebugShape& shape = mShapes[i];

    // If the shape doesn't have one of the active mask bits set then don't draw it
    if((shape.mMask & mActiveMask) == 0)
      continue;
    
    // If this shape always draws on top then disable depth testing
    if(shape.mOnTop)
      glDisable(GL_DEPTH_TEST);


    // Decompose the matrix to set the gl transform (too lazy to properly transform the matrix between formats)
    float radians;
    Vector3 scale, translation, axis;
    Matrix3 rotationMat;
    shape.mTransform.Decompose(&scale, &rotationMat, &translation);
    Math::ToAxisAngle(Math::ToQuaternion(rotationMat), &axis, &radians);
    glPushMatrix();
    // Set the transform
    glTranslatef(translation.x, translation.y, translation.z);
    glRotatef(Math::RadToDeg(radians), axis.x, axis.y, axis.z);
    glScalef(scale.x, scale.y, scale.z);

    glBegin(GL_LINES);
    glColor3fv(shape.mColor.array);

    // Draw all of the line segments of this shape
    for(size_t j = 0; j < shape.mSegments.size(); ++j)
    {
      LineSegment& segment = shape.mSegments[j];

      glVertex3fv(segment.mStart.array);
      glVertex3fv(segment.mEnd.array);
    }

    glEnd();
    glPopMatrix();

    // Make sure to re-enable depth testing
    if(shape.mOnTop)
      glEnable(GL_DEPTH_TEST);
  }
}

DebugShape& DebugDrawer::GetNewShape()
{
  mShapes.push_back(DebugShape());
  return mShapes.back();
}

// DONE
DebugShape& DebugDrawer::DrawPoint(const Vector3& point)
{
  return DrawSphere(Sphere(point, 0.5f));
}

// DONE
DebugShape& DebugDrawer::DrawLine(const LineSegment& line)
{
  DebugShape& shape = GetNewShape();
  /******Student:Assignment2******/
  shape.mSegments.push_back(line);
  return shape;
}

// DONE
DebugShape& DebugDrawer::DrawRay(const Ray& ray, float t)
{
  DebugShape& shape = GetNewShape();
  /******Student:Assignment2******/
  //AddRay(shape, ray.mStart, ray.mDirection, t);
  Vector3 x_Axis(1.0f, 0.0f, 0.0f);
  Vector3 y_Axis(0.0f, 1.0f, 0.0f);
  Vector3 z_Axis(0.0f, 0.0f, 1.0f);

  // Draw the line 
  Vector3 raySegment = ray.mStart + ray.mDirection * t;
  LineSegment line;
  line.mStart = ray.mStart;
  line.mEnd = raySegment;
  shape.mSegments.push_back(line);

  //Math::Vec3Ptr BasisX = new Vector3(0,0,0);
  //Math::Vec3Ptr BasisY = new Vector3(0, 0, 0);
  Vector3 BasisX;
  Vector3 BasisY;
  //as long as ux or uy is non - zero, then the z - axis is a valid axis to use, otherwise we
  // can just use the x - axis.
  Vector3 normDirection = ray.mDirection;
  //Math::AttemptNormalize(&normDirection);
 // Math::GenerateOrthonormalBasis(ray.mDirection, &BasisX, &BasisY);
  if (ray.mDirection.x != 0.0f)
  {
    BasisX = ray.mDirection.Cross((y_Axis.Normalized()));
    BasisY = ray.mDirection.Cross(BasisX.Normalized());
  }
  else if (ray.mDirection.y != 0.0f)
  {
    BasisX = ray.mDirection.Cross((z_Axis.Normalized()));
    BasisY = ray.mDirection.Cross(BasisX.Normalized());
  }
  else if (ray.mDirection.z != 0.0f)
  {
    BasisX = ray.mDirection.Cross((x_Axis.Normalized()));
    BasisY = ray.mDirection.Cross(BasisX.Normalized());
  }
    float theta = 0.f;
    std::vector<Vector3> disc;
    Vector3 disc_ = line.mEnd - (ray.mDirection * 0.3f);
    // Need four segements connecting from the endpoint of the ray to the disc
    // hapening at 0/90/180/270 degrees
    for (int i = 0; i < 360; ++i)
    {
      theta += 1.0f;
      float toRadians = Math::DegToRad(theta);
      disc.push_back(0.05f * ((BasisX)* cos(toRadians) + (BasisY) * sin(toRadians)));
    }
    // Draw the four segments
    for (int i = 0; i < 360; ++i)
    {
      if (i == 0 || i == 90 || i == 180 || i == 270)
        shape.mSegments.push_back(LineSegment(disc_ + disc[i], raySegment));
      if (i != 359)
        shape.mSegments.push_back(LineSegment(disc_ + disc[i], disc_ + disc[i + 1]));
      else
        shape.mSegments.push_back(LineSegment(disc_ + disc[i], disc_ + disc[0]));
    }
    return shape;
}

// DONE
DebugShape& DebugDrawer::DrawSphere(const Sphere& sphere)
{
  // To access the camera's position for the horizon disc calculation use mApplication->mCamera.mTranslation
  DebugShape& shape = GetNewShape();
  /******Student:Assignment2******/
  if (mApplication == nullptr)
    return shape;

  // horizontal disc
  float dist = (sphere.mCenter - mApplication->mCamera.mTranslation).Length();
  float len = Math::Sqrt(Math::Sq(dist) - Math::Sq(sphere.mRadius));
  float h = (len * sphere.mRadius) / dist;
  float z = Math::Sqrt(Math::Sq(sphere.mRadius) - Math::Sq(h));
  Vector3 hcenter = sphere.mCenter - (-mApplication->mCamera.GetDirection().Normalized() * z);

  Vector3 basisX, basisY;
  Vector3 planeNormal =mApplication->mCamera.GetDirection();
  Math::GenerateOrthonormalBasis(planeNormal, &basisX, &basisY);
  Vector3 x_Axis(1.0f, 0.0f, 0.0f);
  Vector3 y_Axis(0.0f, 1.0f, 0.0f);
  Vector3 z_Axis(0.0f, 0.0f, 1.0f);

  float theta = 0;
  std::vector<Vector3> xy, yz, zx, horizontal;
  // circle points
  for (int i = 0; i < 360; ++i)
  {
    theta += 1.f;
    float thetaRadian = Math::DegToRad(theta);
    xy.push_back(sphere.mRadius * (Vector3::cXAxis * cos(thetaRadian) + y_Axis * sin(thetaRadian)));
    yz.push_back(sphere.mRadius * (Vector3::cYAxis * cos(thetaRadian) + z_Axis * sin(thetaRadian)));
    zx.push_back(sphere.mRadius * (Vector3::cZAxis * cos(thetaRadian) + x_Axis * sin(thetaRadian)));
    horizontal.push_back(h * (basisX * cos(thetaRadian) + basisY * sin(thetaRadian)));
  }

  for (int i = 0; i < 360; ++i)
  {
    if (i != 359)
    {
      shape.mSegments.push_back(LineSegment(sphere.mCenter + xy[i], sphere.mCenter + xy[i + 1]));
      shape.mSegments.push_back(LineSegment(sphere.mCenter + yz[i], sphere.mCenter + yz[i + 1]));
      shape.mSegments.push_back(LineSegment(sphere.mCenter + zx[i], sphere.mCenter + zx[i + 1]));
      shape.mSegments.push_back(LineSegment(hcenter + horizontal[i], hcenter + horizontal[i + 1]));
    }
    else
    {
      shape.mSegments.push_back(LineSegment(sphere.mCenter + xy[i], sphere.mCenter + xy[0]));
      shape.mSegments.push_back(LineSegment(sphere.mCenter + yz[i], sphere.mCenter + yz[0]));
      shape.mSegments.push_back(LineSegment(sphere.mCenter + zx[i], sphere.mCenter + zx[0]));
      shape.mSegments.push_back(LineSegment(hcenter + horizontal[i], hcenter + horizontal[0]));
    }
  }
  return shape;
}

// DONE
DebugShape& DebugDrawer::DrawAabb(const Aabb& aabb)
{
  DebugShape& shape = GetNewShape();
  /******Student:Assignment2******/

  LineSegment l_1;
  l_1.mStart = Vector3(aabb.mMin.x, aabb.mMin.y, aabb.mMin.z);
  l_1.mEnd = Vector3(aabb.mMax.x, aabb.mMin.y, aabb.mMin.z);
  shape.mSegments.push_back(l_1);

  LineSegment l_2;
  l_2.mStart = Vector3(aabb.mMin.x, aabb.mMax.y, aabb.mMin.z);
  l_2.mEnd = Vector3(aabb.mMax.x, aabb.mMax.y, aabb.mMin.z);
  shape.mSegments.push_back(l_2);

  LineSegment l_3;
  l_3.mStart = Vector3(aabb.mMin.x, aabb.mMin.y, aabb.mMax.z);
  l_3.mEnd = Vector3(aabb.mMax.x, aabb.mMin.y, aabb.mMax.z);
  shape.mSegments.push_back(l_3);

  LineSegment l_4;
  l_4.mStart = Vector3(aabb.mMin.x, aabb.mMax.y, aabb.mMax.z);
  l_4.mEnd = Vector3(aabb.mMax.x, aabb.mMax.y, aabb.mMax.z);
  shape.mSegments.push_back(l_4);

  LineSegment l_5;
  l_5.mStart = Vector3(aabb.mMin.x, aabb.mMin.y, aabb.mMin.z);
  l_5.mEnd = Vector3(aabb.mMin.x, aabb.mMax.y, aabb.mMin.z);
  shape.mSegments.push_back(l_5);

  LineSegment l_6;
  l_6.mStart = Vector3(aabb.mMax.x, aabb.mMin.y, aabb.mMin.z);
  l_6.mEnd = Vector3(aabb.mMax.x, aabb.mMax.y, aabb.mMin.z);
  shape.mSegments.push_back(l_6);

  LineSegment l_7;
  l_7.mStart = Vector3(aabb.mMin.x, aabb.mMin.y, aabb.mMax.z);
  l_7.mEnd = Vector3(aabb.mMin.x, aabb.mMax.y, aabb.mMax.z);
  shape.mSegments.push_back(l_7);

  LineSegment l_8;
  l_8.mStart = Vector3(aabb.mMax.x, aabb.mMin.y, aabb.mMax.z);
  l_8.mEnd = Vector3(aabb.mMax.x, aabb.mMax.y, aabb.mMax.z);
  shape.mSegments.push_back(l_8);

  LineSegment l_9;
  l_9.mStart = Vector3(aabb.mMin.x, aabb.mMin.y, aabb.mMin.z);
  l_9.mEnd = Vector3(aabb.mMin.x, aabb.mMin.y, aabb.mMax.z);
  shape.mSegments.push_back(l_9);

  LineSegment l_10;
  l_10.mStart = Vector3(aabb.mMin.x, aabb.mMax.y, aabb.mMin.z);
  l_10.mEnd = Vector3(aabb.mMin.x, aabb.mMax.y, aabb.mMax.z);
  shape.mSegments.push_back(l_10);

  LineSegment l_11;
  l_11.mStart = Vector3(aabb.mMax.x, aabb.mMin.y, aabb.mMin.z);
  l_11.mEnd = Vector3(aabb.mMax.x, aabb.mMin.y, aabb.mMax.z);
  shape.mSegments.push_back(l_11);

  LineSegment l_12;
  l_12.mStart = Vector3(aabb.mMax.x, aabb.mMax.y, aabb.mMin.z);
  l_12.mEnd = Vector3(aabb.mMax.x, aabb.mMax.y, aabb.mMax.z);
  shape.mSegments.push_back(l_12);
  return shape;
}

// DONE
DebugShape& DebugDrawer::DrawBox(const Aabb& aabb, const Matrix4& transform)
{
  DebugShape& shape = GetNewShape();

  Vector3 v1 = Math::TransformPoint(transform, Vector3(aabb.mMin.x, aabb.mMin.y, aabb.mMin.z));
  Vector3 v2 = Math::TransformPoint(transform, Vector3(aabb.mMax.x, aabb.mMin.y, aabb.mMin.z));
  Vector3 v3 = Math::TransformPoint(transform, Vector3(aabb.mMin.x, aabb.mMax.y, aabb.mMin.z));
  Vector3 v4 = Math::TransformPoint(transform, Vector3(aabb.mMax.x, aabb.mMax.y, aabb.mMin.z));
  Vector3 v5 = Math::TransformPoint(transform, Vector3(aabb.mMin.x, aabb.mMin.y, aabb.mMax.z));
  Vector3 v6 = Math::TransformPoint(transform, Vector3(aabb.mMax.x, aabb.mMin.y, aabb.mMax.z));
  Vector3 v7 = Math::TransformPoint(transform, Vector3(aabb.mMin.x, aabb.mMax.y, aabb.mMax.z));
  Vector3 v8 = Math::TransformPoint(transform, Vector3(aabb.mMax.x, aabb.mMax.y, aabb.mMax.z));

  shape.mSegments.push_back(LineSegment(v1, v2));
  shape.mSegments.push_back(LineSegment(v3, v2));
  shape.mSegments.push_back(LineSegment(v5, v6));
  shape.mSegments.push_back(LineSegment(v7, v8));
  shape.mSegments.push_back(LineSegment(v1, v3));
  shape.mSegments.push_back(LineSegment(v2 , v4));
  shape.mSegments.push_back(LineSegment(v5 , v7));
  shape.mSegments.push_back(LineSegment(v6, v8));
  shape.mSegments.push_back(LineSegment(v1, v5));
  shape.mSegments.push_back(LineSegment(v2, v6));
  shape.mSegments.push_back(LineSegment(v3, v7));
  shape.mSegments.push_back(LineSegment(v4, v8));

  return shape;
}

// DONE
DebugShape& DebugDrawer::DrawTriangle(const Triangle& triangle)
{
  DebugShape& shape = GetNewShape();
  /******Student:Assignment2******/
  shape.mSegments.push_back(LineSegment(triangle.mPoints[0], triangle.mPoints[1]));
  shape.mSegments.push_back(LineSegment(triangle.mPoints[0], triangle.mPoints[2]));
  shape.mSegments.push_back(LineSegment(triangle.mPoints[1], triangle.mPoints[2]));

  return shape;
}

// DONE
DebugShape& DebugDrawer::DrawPlane(const Plane& plane, float sizeX, float sizeY)
{
  DebugShape& shape = GetNewShape();

  Vector3 planeNormal = plane.GetNormal();
  Vector3 b0, b1;
  Math::GenerateOrthonormalBasis(planeNormal, &b0, &b1);

  Vector3 v1(planeNormal - b0 * sizeX - b1 * sizeY);
  Vector3 v2(planeNormal + b0 * sizeX - b1 * sizeY);
  Vector3 v3(planeNormal + b0 * sizeX + b1 * sizeY);
  Vector3 v4(planeNormal - b0 * sizeX + b1 * sizeY);

  shape.mSegments.push_back(LineSegment(planeNormal, planeNormal + planeNormal));
  shape.mSegments.push_back(LineSegment(v1, v2));
  shape.mSegments.push_back(LineSegment(v2, v3));
  shape.mSegments.push_back(LineSegment(v3, v4));
  shape.mSegments.push_back(LineSegment(v4, v1));

  return shape;
}

// DONE
DebugShape& DebugDrawer::DrawQuad(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
  DebugShape& shape = GetNewShape();
  /******Student:Assignment2******/
  shape.mSegments.push_back(LineSegment(p0, p1));
  shape.mSegments.push_back(LineSegment(p1, p2));
  shape.mSegments.push_back(LineSegment(p2, p3));
  shape.mSegments.push_back(LineSegment(p3, p0));
  return shape;
}

// DONE
DebugShape& DebugDrawer::DrawFrustum(const Frustum& frustum)
{
  DebugShape& shape = GetNewShape();

  shape.mSegments.push_back(LineSegment(frustum.mPoints[0], frustum.mPoints[1]));
  shape.mSegments.push_back(LineSegment(frustum.mPoints[1], frustum.mPoints[2]));
  shape.mSegments.push_back(LineSegment(frustum.mPoints[2], frustum.mPoints[3]));
  shape.mSegments.push_back(LineSegment(frustum.mPoints[3], frustum.mPoints[0]));
  shape.mSegments.push_back(LineSegment(frustum.mPoints[4], frustum.mPoints[5]));
  shape.mSegments.push_back(LineSegment(frustum.mPoints[5], frustum.mPoints[6]));
  shape.mSegments.push_back(LineSegment(frustum.mPoints[6], frustum.mPoints[7]));
  shape.mSegments.push_back(LineSegment(frustum.mPoints[7], frustum.mPoints[4]));
  shape.mSegments.push_back(LineSegment(frustum.mPoints[0], frustum.mPoints[4]));
  shape.mSegments.push_back(LineSegment(frustum.mPoints[1], frustum.mPoints[5]));
  shape.mSegments.push_back(LineSegment(frustum.mPoints[3], frustum.mPoints[7]));
  shape.mSegments.push_back(LineSegment(frustum.mPoints[2], frustum.mPoints[6]));
  return shape;
}

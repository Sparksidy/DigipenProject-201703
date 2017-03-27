///////////////////////////////////////////////////////////////////////////////
///
/// Authors: Joshua Davis
/// Copyright 2015, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.hpp"

//-----------------------------------------------------------------------------NSquaredSpatialPartition
NSquaredSpatialPartition::NSquaredSpatialPartition()
{
  mType = SpatialPartitionTypes::NSquared;
}

void NSquaredSpatialPartition::InsertData(SpatialPartitionKey& key, SpatialPartitionData& data)
{
  // Doing this lazily (and bad, but it's n-squared...).
  // Just store as the key what the client data is so we can look it up later.
  key.mVoidKey = data.mClientData;
  mData.push_back(data.mClientData);
}

void NSquaredSpatialPartition::UpdateData(SpatialPartitionKey& key, SpatialPartitionData& data)
{
  // Nothing to do here, update doesn't do anything
}

void NSquaredSpatialPartition::RemoveData(SpatialPartitionKey& key)
{
  // Find the key data and remove it
  for(size_t i = 0; i < mData.size(); ++i)
  {
    if(mData[i] == key.mVoidKey)
    {
      mData[i] = mData.back();
      mData.pop_back();
      break;
    }
  }
}

void NSquaredSpatialPartition::DebugDraw(int level, const Math::Matrix4& transform, const Vector4& color, int bitMask)
{
  // Nothing to debug draw
}

void NSquaredSpatialPartition::CastRay(const Ray& ray, CastResults& results)
{
  // Add everything
  for(size_t i = 0; i < mData.size(); ++i)
  {
    CastResult result;
    result.mClientData = mData[i];
    results.AddResult(result);
  }
}

void NSquaredSpatialPartition::CastFrustum(const Frustum& frustum, CastResults& results)
{
  // Add everything
  for(size_t i = 0; i < mData.size(); ++i)
  {
    CastResult result;
    result.mClientData = mData[i];
    results.AddResult(result);
  }
}

void NSquaredSpatialPartition::SelfQuery(QueryResults& results)
{
  // Add everything
  for(size_t i = 0; i < mData.size(); ++i)
  {
    for(size_t j = i + 1; j < mData.size(); ++j)
    {
      results.AddResult(QueryResult(mData[i], mData[j]));
    }
  }
}

void NSquaredSpatialPartition::GetDataFromKey(const SpatialPartitionKey& key, SpatialPartitionData& data) const
{
  data.mClientData = key.mVoidKey;
}

void NSquaredSpatialPartition::FilloutData(std::vector<SpatialPartitionQueryData>& results) const
{
  for(size_t i = 0; i < mData.size(); ++i)
  {
    SpatialPartitionQueryData data;
    data.mClientData = mData[i];
    results.push_back(data);
  }
}

//-----------------------------------------------------------------------------BoundingSphereSpatialPartition
BoundingSphereSpatialPartition::BoundingSphereSpatialPartition()
{
  mType = SpatialPartitionTypes::NSquaredSphere;
}

void BoundingSphereSpatialPartition::InsertData(SpatialPartitionKey& key, SpatialPartitionData& data)
{
  //check if there were anything been removed
  if (removed.empty())
  {
    key.mUIntKey = mSphereData.size();
    mSphereData.push_back(data.mBoundingSphere);
    mClientData.push_back(data.mClientData);
    allocated.push_back(true);

  }
  //else insert to the idx that exist in removed list
  else
  {
    mSphereData[removed.front()] = data.mBoundingSphere;
    mClientData[removed.front()] = data.mClientData;
    allocated[removed.front()] = true;

    key.mUIntKey = removed.front();
    removed.pop_front();
  }
}

void BoundingSphereSpatialPartition::UpdateData(SpatialPartitionKey& key, SpatialPartitionData& data)
{
  //Warn("Assignment2: Required function un-implemented");

  if (allocated[key.mUIntKey] == false)
  {
    return;
  }

  mSphereData[key.mUIntKey] = data.mBoundingSphere;
  mClientData[key.mUIntKey] = data.mClientData;

}

void BoundingSphereSpatialPartition::RemoveData(SpatialPartitionKey& key)
{
  //add current key to removed list
  removed.push_back(key.mUIntKey);
  allocated[removed.back()] = false;

}

void BoundingSphereSpatialPartition::DebugDraw(int level, const Math::Matrix4& transform, const Vector4& color, int bitMask)
{
  ////Warn("Assignment2: Required function un-implemented");
  //for (auto it : mData)
  //{
  //  if (it == nullptr)
  //    continue;

  //  it->mBoundingSphere.DebugDraw().SetMaskBit(bitMask);
  //  it->mBoundingSphere.DebugDraw().SetTransform(transform);
  //  it->mBoundingSphere.DebugDraw().Color(color);
  //  gDebugDrawer->DrawSphere(it->mBoundingSphere);
  //}
  for (unsigned i = 0; i < mSphereData.size(); ++i)
  {
    if (allocated[i] == false)
      continue;

    mSphereData[i].DebugDraw().SetMaskBit(bitMask);
    mSphereData[i].DebugDraw().SetTransform(transform);
    mSphereData[i].DebugDraw().Color(color);

    gDebugDrawer->DrawSphere(mSphereData[i]);
  }
}

void BoundingSphereSpatialPartition::CastRay(const Ray& ray, CastResults& results)
{
  for (unsigned i = 0; i < mSphereData.size(); ++i)
  {
    if (allocated[i] == false)
      continue;

    float t = 0;
    if (RaySphere(ray.mStart, ray.mDirection, mSphereData[i].mCenter, mSphereData[i].mRadius, t))
    {
      CastResult curResult;
      curResult.mTime = t;
      curResult.mClientData = mClientData[i];
      results.AddResult(curResult);
    }
  }
}

void BoundingSphereSpatialPartition::CastFrustum(const Frustum& frustum, CastResults& results)
{
  for (unsigned i = 0; i < mSphereData.size(); ++i)
  {
    if (allocated[i] == false)
      continue;

    size_t axis;

    IntersectionType::Type intersectType = FrustumSphere(frustum.GetPlanes(), mSphereData[i].mCenter, mSphereData[i].mRadius, axis);
    if (intersectType == IntersectionType::Inside || intersectType == IntersectionType::Overlaps)
    {
      CastResult curResult;
      curResult.mTime = 0;
      curResult.mClientData = mClientData[i];
      results.AddResult(curResult);
    }
  }
}

void BoundingSphereSpatialPartition::SelfQuery(QueryResults& results)
{
  for (unsigned i = 0; i < mSphereData.size(); ++i)
  {
    Sphere it1 = mSphereData[i];
    for (unsigned j = i; j < mSphereData.size(); ++j)
    {
      if (i == j)
        continue;
      Sphere it2 = mSphereData[j];

      const Sphere& sphere0 = it1;
      const Sphere& sphere1 = it2;

      if (SphereSphere(sphere0.mCenter, sphere0.mRadius, sphere1.mCenter, sphere1.mRadius))
      {
        results.AddResult(QueryResult(mClientData[i], mClientData[j]));
      }
    }
  }
}

void BoundingSphereSpatialPartition::FilloutData(std::vector<SpatialPartitionQueryData>& results) const
{
  for (unsigned i = 0; i < mSphereData.size(); ++i)
  {
    if (allocated[i] == false)
      continue;

    SpatialPartitionQueryData spData;
    spData.mClientData = mClientData[i];
    spData.mBoundingSphere = mSphereData[i];
    results.push_back(spData);
  }
}

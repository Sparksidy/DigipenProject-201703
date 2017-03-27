///////////////////////////////////////////////////////////////////////////////
///
/// Authors: Joshua Davis
/// Copyright 2015, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#include "SimpleNSquared.hpp"
#include "Math\Utilities.hpp"
#include "DebugDraw.hpp"
#include "Geometry.hpp"

//-----------------------------------------------------------------------------NSquaredSpatialPartition
NSquaredSpatialPartition::NSquaredSpatialPartition()
{
  mType = SpatialPartitionTypes::NSquared;
}

void NSquaredSpatialPartition::CreateProxy(Proxy& proxy, SpatialPartitionData& data)
{
  // Doing this lazily (and bad, but it's n-squared...).
  // Just store as the proxy what the client data is so we can look it up later.
  proxy.mProxy = data.mClientData;
  mData.push_back(data.mClientData);
}

void NSquaredSpatialPartition::UpdateProxy(Proxy& proxy, SpatialPartitionData& data)
{
  // Nothing to do here, update doesn't do anything
}

void NSquaredSpatialPartition::RemoveProxy(Proxy& proxy)
{
  // Find the proxy data and remove it
  for(size_t i = 0; i < mData.size(); ++i)
  {
    if(mData[i] == proxy.mProxy)
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

void NSquaredSpatialPartition::GetDataFromProxy(const Proxy& proxy, SpatialPartitionData& data) const
{
  data.mClientData = proxy.mProxy;
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

// DONE
void BoundingSphereSpatialPartition::CreateProxy(Proxy& proxy, SpatialPartitionData& data)
{
  proxy.mProxy = data.mClientData;
  _data.push_back(data.mClientData);
  _sdata.push_back(data.mBoundingSphere);
}

// DONE
void BoundingSphereSpatialPartition::UpdateProxy(Proxy& proxy, SpatialPartitionData& data)
{
  int i = (int)proxy.mProxy;
  if ((int)proxy.mProxy == -1)
  {
    _data[i] = proxy.mProxy;
  }
  _sdata[i] = data.mBoundingSphere;
}

// DONE
void BoundingSphereSpatialPartition::RemoveProxy(Proxy& proxy)
{
  // set to -1 
  int i = (int)proxy.mProxy;
  _data[i] = (void*)-1;
}

// DONE
void BoundingSphereSpatialPartition::DebugDraw(int level, const Math::Matrix4& transform, const Vector4& color, int bitMask)
{

  for (unsigned i = 0; i < _data.size(); ++i)
  {
    // do nothing and skip this iteration
    if ((int)_data[i] == -1)
      continue;
    _sdata[i].DebugDraw().SetMaskBit(bitMask);
    _sdata[i].DebugDraw().SetTransform(transform);
    _sdata[i].DebugDraw().Color(color);
    gDebugDrawer->DrawSphere(_sdata[i]);
  }
}

// DONE
void BoundingSphereSpatialPartition::CastRay(const Ray& ray, CastResults& results)
{
  for (unsigned i = 0; i < _data.size(); ++i)
  {
    if ((int)_data[i] == -1)
      continue;

    float t;
    // Check for ray intersection
    if (RaySphere(ray.mStart, ray.mDirection, _sdata[i].mCenter, _sdata[i].mRadius, t))
    {
      CastResult r;
      r.mClientData = _data[i];
      r.mTime = t;
      results.AddResult(r);
    }
  }
}

// DONE
void BoundingSphereSpatialPartition::CastFrustum(const Frustum& frustum, CastResults& results)
{
  for (unsigned i = 0; i < _data.size(); ++i)
  {
    // do nothing and skip this iteration
    if ((int)_data[i] == -1)
      continue;

    Vector4 fPlanes[6];
    for (int j = 0; j < 6; ++j)
      fPlanes[j] = frustum.mPlanes[j].mData;
    size_t lastaxis;
    IntersectionType::Type isType = FrustumSphere(fPlanes, _sdata[i].mCenter, _sdata[i].mRadius, lastaxis);
    // if interesection inside or overlap add 
    if (isType == IntersectionType::Inside || isType == IntersectionType::Overlaps)
    {
      CastResult r;
      r.mTime = 0.0f;
      r.mClientData = _data[i];
      results.AddResult(r);
    }
  }
}

// DONE
void BoundingSphereSpatialPartition::SelfQuery(QueryResults& results)
{
  for (unsigned i = 0; i < _data.size(); ++i)
  {
    if ((int)_data[i] == -1)
      continue;
    for (unsigned j = i + 1; j < _data.size(); ++j)
    {
      if (SphereSphere(_sdata[i].mCenter, _sdata[i].mRadius,
        _sdata[j].mCenter, _sdata[j].mRadius))
      {
        // Check for non valid data
        if (_data[j] == nullptr)
          return;
        results.AddResult(QueryResult(_data[i], _data[j]));
      }
    }
  }
}

// DONE
void BoundingSphereSpatialPartition::FilloutData(std::vector<SpatialPartitionQueryData>& results) const
{
  // fill out the array with valid data only
  for (unsigned i = 0; i < _data.size(); ++i)
  {
    if ((int)_data[i] == -1)
      continue;
    SpatialPartitionQueryData spData;
    spData.mClientData = _data[i];
    spData.mBoundingSphere = _sdata[i];
    results.push_back(spData);
  }
}

//-----------------------------------------------------------------------------BoundingAabbSpatialPartition
BoundingAabbSpatialPartition::BoundingAabbSpatialPartition()
{
  mType = SpatialPartitionTypes::NSquaredAabb;
}

// DONE
void BoundingAabbSpatialPartition::CreateProxy(Proxy& proxy, SpatialPartitionData& data)
{
  proxy.mProxy = data.mClientData;
  _data.push_back(data.mClientData);
  _aabbdata.push_back(data.mAabb);
}

// DONE
void BoundingAabbSpatialPartition::UpdateProxy(Proxy& proxy, SpatialPartitionData& data)
{
  int i = (int)proxy.mProxy;
  if ((int)proxy.mProxy == -1)
  {
    _data[i] = proxy.mProxy;
  }
  _aabbdata[i] = data.mAabb;
}

// DONE
void BoundingAabbSpatialPartition::RemoveProxy(Proxy& proxy)
{
  int i = (int)proxy.mProxy;
  _data[i] = (void*)-1;
}

// DONE
void BoundingAabbSpatialPartition::DebugDraw(int level, const Math::Matrix4& transform, const Vector4& color, int bitMask)
{
  for (unsigned i = 0; i < _data.size(); ++i)
  {
    // do nothing and skip this iteration
    if ((int)_data[i] == -1)
      continue;

    _aabbdata[i].DebugDraw().Color(color);
    _aabbdata[i].DebugDraw().SetMaskBit(bitMask);
    _aabbdata[i].DebugDraw().SetTransform(transform);

    gDebugDrawer->DrawAabb(_aabbdata[i]);
  }
}

// DONE
void BoundingAabbSpatialPartition::CastRay(const Ray& ray, CastResults& results)
{
  for (unsigned i = 0; i < _data.size(); ++i)
  {
    // do nothing and skip this iteration
    if ((int)_data[i] == -1)
      continue;
    float t;
    if (RayAabb(ray.mStart, ray.mDirection, _aabbdata[i].mMin, _aabbdata[i].mMax, t))
    {
      CastResult r;
      r.mTime = t;
      r.mClientData = _data[i];
      results.AddResult(r);
    }
  }
}

// DONE
void BoundingAabbSpatialPartition::CastFrustum(const Frustum& frustum, CastResults& results)
{
  for (unsigned i = 0; i < _data.size(); ++i)
  {
    // do nothing and skip this iteration
    if ((int)_data[i] == -1)
      continue;

    Vector4 fPlanes[6];
    for (int j = 0; j < 6; ++j)
      fPlanes[j] = frustum.mPlanes[j].mData;
    size_t lastaxis;
    IntersectionType::Type isType = FrustumAabb(fPlanes, _aabbdata[i].mMin, _aabbdata[i].mMax, lastaxis);
    // if interesection inside or overlap add 
    if (isType == IntersectionType::Inside || isType == IntersectionType::Overlaps)
    {
      CastResult r;
      r.mTime = 0.0f;
      r.mClientData = _data[i];
      results.AddResult(r);
    }
  }
}

// DONE
void BoundingAabbSpatialPartition::SelfQuery(QueryResults& results)
{
  for (unsigned i = 0; i < _data.size(); ++i)
  {
    if ((int)_data[i] == -1)
      continue;
    for (unsigned j = i + 1; j < _data.size(); ++j)
    {
      if (_data[j] == nullptr)
        return;

      if (AabbAabb(_aabbdata[i].mMin, _aabbdata[i].mMax,_aabbdata[j].mMin, _aabbdata[j].mMax))
      {
        results.AddResult(QueryResult(_data[i], _data[j]));
      }
    }
  }
}

// DONE
void BoundingAabbSpatialPartition::FilloutData(std::vector<SpatialPartitionQueryData>& results) const
{
  // fill out the array with valid data only
  for (unsigned i = 0; i < _data.size(); ++i)
  {
    if ((int)_data[i] == -1)
      continue;
    SpatialPartitionQueryData spData;
    spData.mClientData = _data[i];
    spData.mAabb = _aabbdata[i];
    results.push_back(spData);
  }
}
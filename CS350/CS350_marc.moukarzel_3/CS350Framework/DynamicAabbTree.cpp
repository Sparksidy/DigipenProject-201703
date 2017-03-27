///////////////////////////////////////////////////////////////////////////////
///
/// Authors: Joshua Davis
/// Copyright 2015, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#include "DynamicAabbTree.hpp"
#include "Geometry.hpp"
#include "Math/Utilities.hpp"
#include "DebugDraw.hpp"
#include <iostream>

const float DynamicAabbTree::mFatteningFactor = 1.1f;
int DynamicAabbTree::Node::id = 0;

DynamicAabbTree::DynamicAabbTree()
{
  mType = SpatialPartitionTypes::AabbTree;
  mRootNode = nullptr;
}

DynamicAabbTree::~DynamicAabbTree()
{
}

DynamicAabbTree::Node* DynamicAabbTree::MakeNode(void)
{
  Node* newNode = new Node();
  // creating an empty node
  newNode->m_parent = nullptr;
  newNode->m_Left = nullptr;
  newNode->m_right = nullptr;
  //newNode->last_axis = -1;
  newNode->_id = ++DynamicAabbTree::Node::id;

  return newNode;
}

DynamicAabbTree::Node* DynamicAabbTree::MakeNode(Proxy& proxy, SpatialPartitionData& data)
{
  // Allocate new node
  Node* node = new Node(data);
  // Set the aabb
  node->mAabb = Aabb::BuildFromCenterAndHalfExtents(data.mAabb.GetCenter(),
    data.mAabb.GetHalfSize() * mFatteningFactor);
  proxy.mProxy = node;
  //node->last_axis = -1;
  node->_id = ++DynamicAabbTree::Node::id;
  return node;
}

DynamicAabbTree::Node* DynamicAabbTree::SelectNode(Node* insertingNode, Node* node0, Node* node1)
{
  // Node selection surface area
  Aabb left, right;

  left = Aabb::Combine(node0->mAabb, insertingNode->mAabb);
  right = Aabb::Combine(node1->mAabb, insertingNode->mAabb);
  float dist0 = left.GetSurfaceArea() - node0->mAabb.GetSurfaceArea();
  float dist1 = right.GetSurfaceArea() - node1->mAabb.GetSurfaceArea();
  
  if (dist0 < dist1)
    return node0;

  return node1;
}

DynamicAabbTree::Node* DynamicAabbTree::WhereToInsert(Node* node)
{
  Node* wheretoinsert = mRootNode;

  // return if its a lead node
  if (IsLeafNode(mRootNode))
    return wheretoinsert;

  while (true)
  {
    wheretoinsert = SelectNode(node, wheretoinsert->m_Left,
      wheretoinsert->m_right);

    if (wheretoinsert->mHeight == 0)
      break;
  }

  return wheretoinsert;
}

bool DynamicAabbTree::IsLeafNode(Node* node)
{
  if (node->m_Left == nullptr && node->m_right == nullptr)
    return true;

  return false;
}

DynamicAabbTree::Node* DynamicAabbTree::FindLastLeaft(Node* node)
{
  if (!node->m_Left && !node->m_right)
    return node;

  if (node->m_Left->mHeight >= node->m_right->mHeight)
    return FindLastLeaft(node->m_Left);
  else
    return FindLastLeaft(node->m_right);
}

void DynamicAabbTree::UpdateHeight(Node* node)
{
  if (IsLeafNode(node))
  {
    node->mHeight = 0;
    return;
  }

  UpdateHeight(node->m_Left);
  UpdateHeight(node->m_right);

  if (node->m_Left->mHeight >= node->m_right->mHeight)
    node->mHeight = (node->m_Left->mHeight + 1);
  else
    node->mHeight = (node->m_right->mHeight + 1);

}

void DynamicAabbTree::UpdateBalance(Node* node)
{
  if (node == mRootNode)
    return;

  Node* parent = node->m_parent;

  if (abs((int)(parent->m_Left->mHeight - parent->m_right->mHeight)) >= 2)
    Balance(parent);

  UpdateBalance(parent);
  return;
}

void DynamicAabbTree::BlanceAABB(Node* node)
{
  if (node == mRootNode)
    return;

  Node* parent = node->m_parent;
  parent->mAabb = Aabb::Combine(parent->m_Left->mAabb, parent->m_right->mAabb);
  BlanceAABB(parent);

  return;
}

DynamicAabbTree::Node* DynamicAabbTree::GetImbalancedNode(Node * node)
{
  Node* imbalancedNode = nullptr;

  if (node->m_Left->mHeight >= node->m_right->mHeight)
    imbalancedNode = node->m_Left;
  else
    imbalancedNode = node->m_right;

  return imbalancedNode;
}

void DynamicAabbTree::Balance(Node* node)
{
  // Find Imbalanced node
  Node* imbalancedNode = GetImbalancedNode(node);
  // Identify the small and large child
  Node* smallChild;
  Node* largeChild;
  Node* grandParent;

  // If the node doesnt have a parent then it is the root node
  if (node->m_parent == nullptr)
  {
    imbalancedNode->m_parent = nullptr;
    mRootNode = imbalancedNode;
  }
  // Otherwise we can store the grand parent and link it correctly acording to 
  // parent side
  else
  {
    grandParent = node->m_parent;
    if (node == grandParent->m_Left)
      grandParent->m_Left = imbalancedNode;
    else
      grandParent->m_right = imbalancedNode;

    imbalancedNode->m_parent = grandParent;
  }

  if (imbalancedNode->m_Left->mHeight <= imbalancedNode->m_right->mHeight)
  {
    smallChild = imbalancedNode->m_Left;
    largeChild = imbalancedNode->m_right;
    imbalancedNode->m_Left = node;
  }
  else
  {
    smallChild = imbalancedNode->m_right;
    largeChild = imbalancedNode->m_Left;
    imbalancedNode->m_right = node;
  }
  node->m_parent = imbalancedNode;

  if (imbalancedNode == node->m_Left)
    node->m_Left = smallChild;
  else
    node->m_right = smallChild;

  smallChild->m_parent = node;

  Node* pNode;
  if (node->m_Left->mHeight >= node->m_right->mHeight)
    pNode = FindLastLeaft(node->m_Left);
  else
    pNode = FindLastLeaft(node->m_right);

 
  UpdateHeight(mRootNode);
  BlanceAABB(pNode);
}

void DynamicAabbTree::CreateProxy(Proxy& proxy, SpatialPartitionData& data)
{
  // First case inserting into an empty tree node becomes roots
  if (mRootNode == nullptr)
    mRootNode = MakeNode(proxy, data);
  else
  {
    Node* pNode = MakeNode(proxy, data);
    Node* whereToInsert = WhereToInsert(pNode);

    if (IsLeafNode(whereToInsert))
    {
      Node* node = MakeNode();
      Node* parent;
      // Second Case 1 Node Create new node to the parent of 2 leaf
      // Link them up . old node the left and new is right
      if (whereToInsert == mRootNode)
      {
        node->m_Left = mRootNode;
        node->m_right = pNode;
        mRootNode = node;
        node->m_Left->m_parent = node;
        node->m_right->m_parent = node;
        node->mClientData = node->m_Left->mClientData;
        node->m_parent = nullptr;
      }
      else
      {
        parent = whereToInsert->m_parent;

        // connect node with initial parent
        node->m_parent = parent;
        node->m_Left = whereToInsert;
        node->m_Left->m_parent = node;
        node->m_right = pNode;
        node->m_right->m_parent = node;
        node->mClientData = nullptr;

        if (node->m_Left == parent->m_Left)
          parent->m_Left = node;
        else
          parent->m_right = node;

      }
      UpdateHeight(mRootNode);
      UpdateBalance(pNode);
      BlanceAABB(pNode);
    }
  }
}

void DynamicAabbTree::UpdateProxy(Proxy& proxy, SpatialPartitionData& data)
{
  Node* updatingNode = (Node*)proxy.mProxy;

  if (!updatingNode->mAabb.Contains(data.mAabb))
  {
    RemoveProxy(proxy);
    CreateProxy(proxy, data);
  }
}

void DynamicAabbTree::RemoveProxy(Proxy& proxy)
{
  //replace parent with sibling

  Node* pNode = (Node*)proxy.mProxy;
  
  if (pNode == nullptr)
    return;

  Node* parent = pNode->m_parent;
  Node* grandParent;
  
  if (parent)
    grandParent = parent->m_parent;

  Node* promotingNode;

 
  // Case 1 : no parent
  if (parent == nullptr)
  {
    mRootNode = nullptr;
    delete pNode;
  }
  // Case 2: No Grandparanet
  else if (parent && grandParent == nullptr)
  {
    // select promoting node
    if (pNode == parent->m_Left)
      promotingNode = parent->m_right;
    else if (pNode == parent->m_right)
      promotingNode = parent->m_Left;

    promotingNode->m_parent= nullptr;
    mRootNode = promotingNode;

    delete parent;
    delete pNode;
  }
  // Case 3 : we have a Gp
  else if (parent && grandParent)
  {
    // if the node is left of parent
    if (pNode == parent->m_Left)
    {
      promotingNode = parent->m_right;
      promotingNode->m_parent = grandParent;
    }
    else if (pNode == parent->m_right)
    {
      promotingNode = parent->m_Left;
      promotingNode->m_parent = grandParent;
    }

    /// Fix links depending on parent pos
    if (parent == grandParent->m_Left)
      grandParent->m_Left= promotingNode;
    else
      grandParent->m_right = promotingNode;

    delete parent;
    delete pNode;

    UpdateHeight(mRootNode);
    UpdateBalance(promotingNode);
    BlanceAABB(promotingNode);
  }
}

void DynamicAabbTree::DebugDraw(int level, const Math::Matrix4& transform, const Vector4& color, int bitMask)
{
  Warn("Assignment3: Required function un-implemented");
}

void DynamicAabbTree::cRay(Node* node, const Ray& ray, CastResults& r)
{
  if (node == nullptr)
    return;

  Aabb aabb = node->mAabb;
  float time;
    
  if (!RayAabb(ray.mStart, ray.mDirection, aabb.GetMin(), aabb.GetMax(), time))
    return;
  else  if (IsLeafNode(node))
  {
      CastResult result;
      result.mTime = time;
      result.mClientData = node->mClientData;
      r.AddResult(result);
  }

  // walk nodes tree
  cRay(node->m_Left, ray, r);
  cRay(node->m_right, ray, r);
}

void DynamicAabbTree::CastRay(const Ray& ray, CastResults& results)
{
  cRay(mRootNode, ray, results);
}

void DynamicAabbTree::cFrustum(Node* node,
  const Frustum& frustum, CastResults& results)
{
  if (node == nullptr)
    return;
  Aabb aabb = node->mAabb;

  Vector4 plane[6];
  for (int i = 0; i < 6; ++i)
    plane[i] = frustum.mPlanes[i].mData;

  size_t lastaxis = 0;
  IntersectionType::Type type =
    FrustumAabb(plane, node->mAabb.GetMin(),
    node->mAabb.GetMax(),lastaxis);

  // If an aabb is outisie the frustum then skip it 
  if (type == IntersectionType::Outside)
    return;
  // if aabb is full contained then add all children no more checks
  else if (type == IntersectionType::Inside)
  {
    AddAllChildren(node, results);
    return;
  }

  // if the cast shape overlaps a nodes aabb recurse
  if (IsLeafNode(node))
  {
    if (type == IntersectionType::Overlaps)
    {
      CastResult result;
      result.mTime = 0;
      result.mClientData = node->mClientData;
      results.AddResult(result);
    }
  }
  // otherwise recurse
  cFrustum(node->m_Left, frustum, results);
  cFrustum(node->m_right, frustum, results);
}

void DynamicAabbTree::CastFrustum(const Frustum& frustum, CastResults& results)
{
  if (mRootNode == nullptr)
    return;
  mRootNode->last_axis = -1;

  cFrustum(mRootNode, frustum, results);
}

void DynamicAabbTree::SelfQuery(QueryResults& results)
{
  SelfQuery(mRootNode, results);
}

void DynamicAabbTree::FilloutData(std::vector<SpatialPartitionQueryData>& results) const
{
  Filldata(mRootNode, results);
}

void DynamicAabbTree::Filldata(Node* root, std::vector<SpatialPartitionQueryData>& data) const
{
  if (!root)
    return;

  SpatialPartitionQueryData insertingData;
  insertingData.mAabb = root->mAabb;
  insertingData.mClientData = root->mClientData;
  insertingData.mDepth = getLevel(mRootNode, root->_id);
  data.push_back(insertingData);
  Filldata(root->m_Left, data);
  Filldata(root->m_right, data);
}

//Helper function for getLevel().  It returns level of the data if data is
// present in tree, otherwise returns 0.
int DynamicAabbTree::getLevelUtil(Node*node, int id, int level) const
{
  if (node == nullptr)
    return 0;

  if (node->_id == id)
    return level;

  int downlevel = getLevelUtil(node->m_Left, id, level + 1);
  if (downlevel != 0)
    return downlevel;

  downlevel = getLevelUtil(node->m_right, id, level + 1);
  return downlevel;
}

/* Returns level of given data value */
int DynamicAabbTree::getLevel(Node* node, int id) const
{
  return getLevelUtil(node, id,0);
}

void DynamicAabbTree::AddAllChildren(Node* node, CastResults& results)
{
  if (!node)
    return;

  if (!node->m_Left && !node->m_right)
  {
    CastResult result;
    result.mTime = 0;
    result.mClientData = node->mClientData;
    results.AddResult(result);
  }

  AddAllChildren(node->m_Left, results);
  AddAllChildren(node->m_right, results);
}


void DynamicAabbTree::SelfQuery(Node* node, QueryResults& results)
{
  if (node == nullptr)
    return;

  if (IsLeafNode(node))
    return;

  SelfQuery(node->m_Left, node->m_right, results);
  SelfQuery(node->m_Left, results);
  SelfQuery(node->m_right, results);
}

void DynamicAabbTree::SelfQuery(Node* nodeA, Node* nodeB, QueryResults& results)
{
  if (!nodeA || !nodeB)
    return;

  if (AabbAabb(nodeA->mAabb.mMin, nodeA->mAabb.mMax, nodeB->mAabb.mMin, nodeB->mAabb.mMax))
  {
    if (IsLeafNode(nodeA) && IsLeafNode(nodeB))
    {
      QueryResult r(nodeA->mClientData, nodeB->mClientData);
      results.AddResult(r);
    }
  }
  else
  {
    return;
  }

  
  if (IsLeafNode(nodeA) && !IsLeafNode(nodeB))
  {
    SelfQuery(nodeA, nodeB->m_Left, results);
    SelfQuery(nodeA, nodeB->m_right, results);
  }

  else if (!IsLeafNode(nodeA) && !IsLeafNode(nodeB))
  {
    if (nodeA->mAabb.GetVolume() <= nodeB->mAabb.GetVolume())
    {
      SelfQuery(nodeA, nodeB->m_Left, results);
      SelfQuery(nodeA, nodeB->m_right, results);
    }
    else
    {
      SelfQuery(nodeB, nodeA->m_Left, results);
      SelfQuery(nodeB, nodeA->m_right, results);
    }
  }
  else
  {
    SelfQuery(nodeB, nodeA->m_Left, results);
    SelfQuery(nodeB, nodeA->m_right, results);
  }
}
///////////////////////////////////////////////////////////////////////////////
///
/// Authors: Joshua Davis
/// Copyright 2015, DigiPen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "SpatialPartition.hpp"
#include "Shapes.hpp"

/******Student:Assignment3******/
/// You must implement a dynamic aabb as we discussed in class.
class DynamicAabbTree : public SpatialPartition
{
public:
  DynamicAabbTree();
  ~DynamicAabbTree();

  // Spatial Partition Interface
  void CreateProxy(Proxy& proxy, SpatialPartitionData& data) override;
  void UpdateProxy(Proxy& proxy, SpatialPartitionData& data) override;
  void RemoveProxy(Proxy& proxy) override;

  void DebugDraw(int level, const Math::Matrix4& transform, const Vector4& color = Vector4(1), int bitMask = 0) override;

  void CastRay(const Ray& ray, CastResults& results) override;
  void CastFrustum(const Frustum& frustum, CastResults& results) override;

  void SelfQuery(QueryResults& results) override;

  void FilloutData(std::vector<SpatialPartitionQueryData>& results) const override;

  static const float mFatteningFactor;

  // Add your implementation here

  // Node Data
  struct Node
  {
    Node()
    {
      m_Left = nullptr;
      m_right = nullptr;
      m_parent = nullptr;
      mHeight = 0;
    }

    Node(SpatialPartitionData& data)
    {
      m_Left = nullptr;
      m_right = nullptr;
      m_parent = nullptr;
      mHeight = 0;
      mClientData = data.mClientData;
    }

    Aabb mAabb;
    void *mClientData;
    Node* m_Left;
    Node* m_right;
    Node* m_parent;
    size_t mHeight;
    size_t last_axis;
    int _id;
    static int id;
  };

  // Keep Track of the root node
  Node* mRootNode;
  // Functions
  Node* MakeNode(Proxy& proxy, SpatialPartitionData& data);
  Node* MakeNode(void);
  // Node selection function
  Node* SelectNode(Node* insertingNode, Node* node0, Node* node1);
  // Find where we can insert the node
  Node* WhereToInsert(Node* node);
  // Check if it is a leaf node
  bool IsLeafNode(Node* node);
  Node* FindLastLeaft(Node* node);
  void Balance(Node* node);
  void UpdateHeight(Node* node);
  void UpdateBalance(Node* node);
  void BlanceAABB(Node* node);
  void Filldata(Node* filloutData, std::vector<SpatialPartitionQueryData>& data)const;
  int getLevelUtil(Node *node, int id, int level) const ;
  int getLevel(Node* node, int id) const ;
  Node* GetImbalancedNode(Node * node);
  void cRay(Node* node,
    const Ray& ray, CastResults& r);
  void cFrustum(Node* node,
    const Frustum& frustum, CastResults& results);
  void AddAllChildren(Node* node, CastResults& results);

  // Split into two recursive functions
  void SelfQuery(Node* node,QueryResults& results);
  void SelfQuery(Node* nodeA, Node* nodeB, QueryResults& results);


  std::vector<void*> mData;
};

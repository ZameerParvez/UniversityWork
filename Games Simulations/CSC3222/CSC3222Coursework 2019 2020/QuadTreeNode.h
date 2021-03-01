#pragma once
#include <algorithm>
#include "../../Common/Vector2.h"
#include "CollisionVolume.h"
#include "RigidBody.h"

namespace NCL {
	namespace CSC3222 {
		class QuadTreeNode {
		public:
			static vector<QuadTreeNode*> allNodes;
			int nodeCapacity = 5;
			int maxDepth = 4;
			int currentDepth = 0;
			bool isLeafNode = true;

			float xLength;
			float yLength;
			float xCorner;
			float yCorner; // All nodes will start at the top left

			//QuadTreeNode();
			QuadTreeNode(Vector2 corner, Vector2 xyLengths, int nodeCapacity = 5, int maxDepth = 3, int currentDepth = 0);
			QuadTreeNode(float xCorner, float yCorner, float xLength, float yLength, int nodeCapacity = 5, int maxDepth = 3, int currentDepth = 0);
			~QuadTreeNode();

			bool Insert(CollisionVolume* v);
			bool Split(); //returns false if a split can't be done, checks if max depth has been reached, depth is set on split too.
			bool NodeContainsVolume(CollisionVolume* v);
			static vector<QuadTreeNode*> GetLeafNodes(QuadTreeNode* q = nullptr);

			QuadTreeNode* children[4] = { nullptr, nullptr, nullptr, nullptr }; // 0-topleft, 1-topright, 2-bottomright, 3-bottomleft
			vector<CollisionVolume*> contents;

		protected:


		};
	}
}
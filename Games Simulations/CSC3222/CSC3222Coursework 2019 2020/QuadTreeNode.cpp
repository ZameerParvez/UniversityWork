#include "QuadTreeNode.h"

using namespace NCL;
using namespace CSC3222;

//QuadTreeNode::QuadTreeNode() {
//
//}

vector<QuadTreeNode*> QuadTreeNode::allNodes = {};

QuadTreeNode::QuadTreeNode(Vector2 corner, Vector2 xyLengths, int nodeCapacity, int maxDepth, int currentDepth) {
	this->nodeCapacity = nodeCapacity;
	this->maxDepth = maxDepth;
	this->currentDepth = currentDepth;
	this->xLength = xyLengths.x;
	this->yLength = xyLengths.y;
	this->xCorner = corner.x;
	this->yCorner = corner.y;
	this->children;
	allNodes.push_back(this);
}

QuadTreeNode::QuadTreeNode(float xCorner, float yCorner, float xLength, float yLength, int nodeCapacity, int maxDepth, int currentDepth) {
	this->nodeCapacity = nodeCapacity;
	this->maxDepth = maxDepth;
	this->currentDepth = currentDepth;
	this->xLength = xLength;
	this->yLength = yLength;
	this->xCorner = xCorner;
	this->yCorner = yCorner;
	this->children;
	allNodes.push_back(this);
}

QuadTreeNode::~QuadTreeNode() {
	// this is needed becasue delete[] only deletes dynamically allocated arrays made with new[], the array here is statically allocated, but contains pointers
	for (QuadTreeNode* q : children) {
		delete q;
	}
	if (currentDepth == 0) {
		allNodes.clear();
	}
	// nothing else is dynamically allocated so default should be ok
}

bool QuadTreeNode::Insert(CollisionVolume* v) {
	if (!NodeContainsVolume(v)) return false; // collisions will only be done for things in the quad tree
	if (currentDepth == maxDepth || (isLeafNode && contents.size() < nodeCapacity)) {
		contents.push_back(v);
		return true;
	} else if (isLeafNode && contents.size() >= nodeCapacity) {
		// split and sort them out
		Split();

		for (CollisionVolume* c : contents) {
			//if (children[0]->Insert(c)) continue;
			//else if (children[1]->Insert(c)) continue;
			//else if (children[2]->Insert(c)) continue;
			//else if (children[3]->Insert(c)) continue;
			children[0]->Insert(c);
			children[1]->Insert(c);
			children[2]->Insert(c);
			children[3]->Insert(c);
		}

		contents.clear();
	}
	
	//if (children[0]->Insert(v)) return true;
	//else if (children[1]->Insert(v)) return true;
	//else if (children[2]->Insert(v)) return true;
	//else if (children[3]->Insert(v)) return true;

	// this makes sure that if an object fits in multiple nodes a refrence to the collision volume would be stored there
	bool result = false;
	result |= children[0]->Insert(v);
	result |= children[1]->Insert(v);
	result |= children[2]->Insert(v);
	result |= children[3]->Insert(v);
	return result;

	// insert volumes into node (if there are children nodes then things should be moved to those first)
		// simple box collision detection between collision vol and quadtree node
	// if max capacity is reached then split
	// place all things in lower nodes
		// could be iterative insert
}

bool QuadTreeNode::Split() {
	if (currentDepth == maxDepth) return false; // This line should never happen because of the check in the other function
	isLeafNode = false;
	//Vector2 corners[] = {
	//	corner,
	//	Vector2(corner.x + (xyLengths.x / 2), corner.y),
	//	corner + (xyLengths / 2),
	//	Vector2(corner.x, corner.y + (xyLengths.y / 2)) 
	//};

	//for (int i = 0; i < 4; i++) {
	//	children[i] = new QuadTreeNode(corners[i], xyLengths / 2, nodeCapacity, maxDepth, currentDepth + 1);
	//}
	float newXLength = xLength / 2;
	float newYLength = yLength / 2;
	children[0] = new QuadTreeNode(xCorner, yCorner, newXLength, newYLength, nodeCapacity, maxDepth, currentDepth + 1);
	children[1] = new QuadTreeNode(xCorner + newXLength, yCorner, newXLength, newYLength, nodeCapacity, maxDepth, currentDepth + 1);
	children[2] = new QuadTreeNode(xCorner + newXLength, yCorner + newYLength, newXLength, newYLength, nodeCapacity, maxDepth, currentDepth + 1);
	children[3] = new QuadTreeNode(xCorner, yCorner + newYLength, newXLength, newYLength, nodeCapacity, maxDepth, currentDepth + 1);
	return true;
}

bool QuadTreeNode::NodeContainsVolume(CollisionVolume* v) {
	// just check for a collision between the nodes box and the colllision volume

	 //Approximate calculation should be a little quicker
	Vector2 cvCentre = v->GetLinkedObject()->GetPosition();
	float diffX = abs(cvCentre.x - (xCorner + xLength / 2));
	float diffY = abs(cvCentre.y - (yCorner + yLength / 2));
	float summedXHalfLengths = xLength / 2;
	float summedYHalfLengths = yLength / 2;

	switch (v->GetCollisionVolumeType()) {
		case CollisionVolumeType::Circle:
			summedXHalfLengths += v->GetVolumeRadius();
			summedYHalfLengths += v->GetVolumeRadius();
			break;		
		case CollisionVolumeType::Square:
		default:
			summedXHalfLengths += v->GetVolumeXY().x;
			summedYHalfLengths += v->GetVolumeXY().y;
			break;
	}

	return (diffX < summedXHalfLengths && diffY < summedYHalfLengths);

	//Vector2 cvCentre = v->GetLinkedObject()->GetPosition();
	//Vector2 nodeCentre = this->corner + (xyLengths / 2);
	//Vector2 centreDiff = Vector2(abs(cvCentre.x - nodeCentre.x), abs(cvCentre.y - nodeCentre.y));
	//Vector2 summedHalfSizes = xyLengths / 2;

	//switch (v->GetCollisionVolumeType()) {
	//	case CollisionVolumeType::Circle:
	//		summedHalfSizes += Vector2(v->GetVolumeRadius(), v->GetVolumeRadius());
	//		break;		
	//	case CollisionVolumeType::Square:
	//	default:
	//		summedHalfSizes += v->GetVolumeXY();
	//		break;
	//}

	//return (centreDiff.x < summedHalfSizes.x&& centreDiff.y < summedHalfSizes.y);
}

vector<QuadTreeNode*> QuadTreeNode::GetLeafNodes(QuadTreeNode* q) {
	vector<QuadTreeNode*> leafnodes;
	for (QuadTreeNode* q : allNodes) {
		if (q->isLeafNode && q->contents.size() > 1) leafnodes.push_back(q);
	}
	return leafnodes;


	//if (q->isLeafNode) {
	//	if (q->contents.empty()) return {};
	//	return { q };
	//}
	//vector<QuadTreeNode*> leafnodes;
	//for (int i = 0; i < 4; i++) {
	//	vector<QuadTreeNode*> leafs = GetLeafNodes(q->children[i]);
	//	leafnodes.insert(leafnodes.end(), leafs.begin(), leafs.end());
	//}

	//return leafnodes;
}

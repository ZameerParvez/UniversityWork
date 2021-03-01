#pragma once
#include "GameSimsPhysics.h"
#include "RigidBody.h"
#include "SimObject.h"
#include "CollisionVolume.h"
#include "../../Common/Vector2.h"
#include "QuadTreeNode.h"
#include <chrono>

using namespace NCL;
using namespace CSC3222;

GameSimsPhysics::GameSimsPhysics()	{

}

GameSimsPhysics::~GameSimsPhysics()	{

}


void GameSimsPhysics::Update(float dt) {
	timeRemaining += dt;

	// This is so that the pysics calculations are done at a fixed timestep
	while (timeRemaining > physicsTimeStep) {
		AccelerationIntegration(physicsTimeStep);
		// Because of the slowness of the collision detection some tiles are left out
		CollisionDetection();	// This is currently the slowest performing part of the physics. It uses a quadtree for broad phase and increasing tree depth reduces the times for checking if collisions between 2 objects are even possible, but this increases the tree construction time
		CollisionResolution();
		VelocityIntegration(physicsTimeStep);

		timeRemaining -= physicsTimeStep;

#if DEBUGPHYSICS == 1
		//std::cout << "`````````````````````````````````````````" << std::endl;
#endif
	}
}

void GameSimsPhysics::AddRigidBody(RigidBody* b) {
	allBodies.emplace_back(b);
}

void GameSimsPhysics::RemoveRigidBody(RigidBody* b) {
	auto at = std::find(allBodies.begin(), allBodies.end(), b);

	if (at != allBodies.end()) {
		//maybe delete too?
		allBodies.erase(at);
	}
}

void GameSimsPhysics::AddCollider(CollisionVolume* c) {
	allColliders.emplace_back(c);
}

void GameSimsPhysics::RemoveCollider(CollisionVolume* c) {
	auto at = std::find(allColliders.begin(), allColliders.end(), c);

	if (at != allColliders.end()) {
		//maybe delete too?
		allColliders.erase(at);
	}
}

void GameSimsPhysics::AccelerationIntegration(float dt) {
	Vector2 acceleration;

	for (auto rb : allBodies) {
		Vector2 acceleration = rb->force * rb->inverseMass;
		rb->velocity = rb->velocity + acceleration * dt;
	}
}


void GameSimsPhysics::VelocityIntegration(float dt) {
	for (auto rb : allBodies) {
		rb->position += rb->velocity;
		rb->velocity = rb->velocity * rb->dampening;
	}
}

// require collision normals first
// There needs to be a check to make sure that only the most up to date pair of collisions is kept
void GameSimsPhysics::CollisionResolution() {
	for (CollisionPair pair : this->collisionPairs) {
		RigidBody* b1 = pair.c1->GetLinkedObject();
		RigidBody* b2 = pair.c2->GetLinkedObject();

		float distance = pair.distance;
		Vector2 normal = pair.normal;

		// top right is ax > bx & ay < by
		if (b1->GetPosition().x > b2->GetPosition().x && b1->GetPosition().y < b2->GetPosition().y) {
			normal.x = -normal.x;
		}
		// bottom right is ax > bx & ay > by
		else if (b1->GetPosition().x >= b2->GetPosition().x && b1->GetPosition().y >= b2->GetPosition().y) {
			normal = -normal;
		}
		//// bottom left is ax < bx & ay < by, this doesnt need to be implemented because it is the normal behaviour
		//else if (b1->GetPosition().x < b2->GetPosition().x && b1->GetPosition().y < b2->GetPosition().y) {
		//}
		// top left is ax < bx & ay < by
		else if (b1->GetPosition().x < b2->GetPosition().x && b1->GetPosition().y > b2->GetPosition().y) {
			normal.y = -normal.y;
		}

		float coefficientOfRestitution = (b1->elasticity * b2->elasticity);
		float normalVelocity = Vector2::Dot(b2->GetVelocity() - b1->GetVelocity(), normal);

		if (normalVelocity > 0) {
			continue;
		}

		float inverseMassSum = b1->inverseMass + b2->inverseMass;
		float impulse = (-(1 + coefficientOfRestitution) / inverseMassSum) * normalVelocity;

		b1->SetPosition(b1->position - (normal * distance * (b1->inverseMass / inverseMassSum)));
		b2->SetPosition(b2->position + (normal * distance * (b2->inverseMass / inverseMassSum)));
		b1->SetVelocity((b1->GetVelocity() - normal * (b1->inverseMass * impulse)));
		b2->SetVelocity((b2->GetVelocity() + normal * (b2->inverseMass * impulse)));
	}

	collisionPairs.clear();
	possibleCollisionPairs.clear();
}

// bitmask would be an improvement over this
// also checks if pairs of objects have already collided in the same physics iteration
bool GameSimsPhysics::CollisionShouldHappen(CollisionVolume* col1, CollisionVolume* col2) {
	// checks if a collision pair of objects has already been checked
	pair<CollisionVolume*, CollisionVolume*> normalPair = { col1, col2 };
	pair<CollisionVolume*, CollisionVolume*> reversedPair = { col2, col1 };
	const bool pairInSet = possibleCollisionPairs.find(normalPair) != possibleCollisionPairs.end();
	const bool reversedPairInSet = possibleCollisionPairs.find(reversedPair) != possibleCollisionPairs.end();
	if (pairInSet || reversedPairInSet) {
		return false;
	}

	GameObjectType type1 = col1->GetLinkedObject()->GetGameObjectType();
	GameObjectType type2 = col2->GetLinkedObject()->GetGameObjectType();
	bool result = false;

	switch (type1) {
		case NCL::CSC3222::GameObjectType::ImpassableTile:
			switch (type2) {
				case NCL::CSC3222::GameObjectType::Balloon:
				case NCL::CSC3222::GameObjectType::BadRobot:
				case NCL::CSC3222::GameObjectType::Laser:
				case NCL::CSC3222::GameObjectType::Player:
				case NCL::CSC3222::GameObjectType::InteractiveTile:
					result = true;
					break;
				case NCL::CSC3222::GameObjectType::ImpassableTile:
				case NCL::CSC3222::GameObjectType::DampeningTile:
				case NCL::CSC3222::GameObjectType::Coin:
				case NCL::CSC3222::GameObjectType::Food:
				default:
					result = false;
					break;
				}
			break;
		case NCL::CSC3222::GameObjectType::DampeningTile:
			switch (type2) {
			case NCL::CSC3222::GameObjectType::Player:
				//result = true;
				//break;
			case NCL::CSC3222::GameObjectType::Balloon:
			case NCL::CSC3222::GameObjectType::BadRobot:
			case NCL::CSC3222::GameObjectType::Laser:
			case NCL::CSC3222::GameObjectType::InteractiveTile:
			case NCL::CSC3222::GameObjectType::ImpassableTile:
			case NCL::CSC3222::GameObjectType::DampeningTile:
			case NCL::CSC3222::GameObjectType::Coin:
			case NCL::CSC3222::GameObjectType::Food:
			default:
				result = false;
				break;
			}
			break;
		case NCL::CSC3222::GameObjectType::InteractiveTile:
			switch (type2) {
			case NCL::CSC3222::GameObjectType::Player:
				result = true;
				break;
			case NCL::CSC3222::GameObjectType::Balloon:
			case NCL::CSC3222::GameObjectType::BadRobot:
			case NCL::CSC3222::GameObjectType::Laser:
			case NCL::CSC3222::GameObjectType::InteractiveTile:
			case NCL::CSC3222::GameObjectType::ImpassableTile:
			case NCL::CSC3222::GameObjectType::DampeningTile:
			case NCL::CSC3222::GameObjectType::Coin:
			case NCL::CSC3222::GameObjectType::Food:
			default:
				result = false;
				break;
			}
			break;
		case NCL::CSC3222::GameObjectType::Player:
			switch (type2) {
				case NCL::CSC3222::GameObjectType::Laser:
				case NCL::CSC3222::GameObjectType::BadRobot:
				case NCL::CSC3222::GameObjectType::InteractiveTile:
				case NCL::CSC3222::GameObjectType::ImpassableTile:
				case NCL::CSC3222::GameObjectType::Balloon:
				case NCL::CSC3222::GameObjectType::Coin:
				case NCL::CSC3222::GameObjectType::Food:
					result = true;
					break;
				case NCL::CSC3222::GameObjectType::DampeningTile:
				case NCL::CSC3222::GameObjectType::Player:
				default:
					result = false;
					break;
				}
			break;
		case NCL::CSC3222::GameObjectType::Balloon:
			switch (type2) {
				case NCL::CSC3222::GameObjectType::Player:
				case NCL::CSC3222::GameObjectType::ImpassableTile:
					result = true;
					break;
				case NCL::CSC3222::GameObjectType::InteractiveTile:
				case NCL::CSC3222::GameObjectType::DampeningTile:
				case NCL::CSC3222::GameObjectType::Balloon:
				case NCL::CSC3222::GameObjectType::BadRobot:
				case NCL::CSC3222::GameObjectType::Laser:
				case NCL::CSC3222::GameObjectType::Coin:
				case NCL::CSC3222::GameObjectType::Food:
				default:
					result = false;
					break;
				}
			break;
		case NCL::CSC3222::GameObjectType::BadRobot:
			switch (type2) {
				case NCL::CSC3222::GameObjectType::ImpassableTile:
				case NCL::CSC3222::GameObjectType::Player:
				case NCL::CSC3222::GameObjectType::BadRobot:
				case NCL::CSC3222::GameObjectType::Laser:
					result = true;
					break;
				case NCL::CSC3222::GameObjectType::InteractiveTile:
				case NCL::CSC3222::GameObjectType::DampeningTile:
				case NCL::CSC3222::GameObjectType::Balloon:
				case NCL::CSC3222::GameObjectType::Coin:
				case NCL::CSC3222::GameObjectType::Food:
				default:
					result = false;
					break;
				}
			break;
		case NCL::CSC3222::GameObjectType::Laser:
			switch (type2) {
				case NCL::CSC3222::GameObjectType::ImpassableTile:
				case NCL::CSC3222::GameObjectType::Player:
				case NCL::CSC3222::GameObjectType::BadRobot:
					result = true;
					break;
				case NCL::CSC3222::GameObjectType::InteractiveTile:
				case NCL::CSC3222::GameObjectType::DampeningTile:
				case NCL::CSC3222::GameObjectType::Balloon:
				case NCL::CSC3222::GameObjectType::Laser:
				case NCL::CSC3222::GameObjectType::Coin:
				case NCL::CSC3222::GameObjectType::Food:
				default:
					result = false;
					break;
				}
			break;
		case NCL::CSC3222::GameObjectType::Coin:
			switch (type2) {
				case NCL::CSC3222::GameObjectType::Player:
					result = true;
					break;
				case NCL::CSC3222::GameObjectType::InteractiveTile:
				case NCL::CSC3222::GameObjectType::ImpassableTile:
				case NCL::CSC3222::GameObjectType::DampeningTile:
				case NCL::CSC3222::GameObjectType::Balloon:
				case NCL::CSC3222::GameObjectType::BadRobot:
				case NCL::CSC3222::GameObjectType::Laser:
				case NCL::CSC3222::GameObjectType::Coin:
				case NCL::CSC3222::GameObjectType::Food:
				default:
					result = false;
					break;
				}
			break;
		case NCL::CSC3222::GameObjectType::Food:
			switch (type2) {
				case NCL::CSC3222::GameObjectType::Player:
					result = true;
					break;
				case NCL::CSC3222::GameObjectType::InteractiveTile:
				case NCL::CSC3222::GameObjectType::ImpassableTile:
				case NCL::CSC3222::GameObjectType::DampeningTile:
				case NCL::CSC3222::GameObjectType::Balloon:
				case NCL::CSC3222::GameObjectType::BadRobot:
				case NCL::CSC3222::GameObjectType::Laser:
				case NCL::CSC3222::GameObjectType::Coin:
				case NCL::CSC3222::GameObjectType::Food:
				default:
					result = false;
					break;
				}
			break;
		default:
			result = true;
			break;
	}

	if (result) {
		possibleCollisionPairs.insert(normalPair);
	}
	
	return result;
}


// Quad tree has some issues with things not colliding properly, I think it is because objects are falling into different nodes when they are colliding
// The collisions position should be taken from the reference in the collider
void GameSimsPhysics::CollisionDetection() {
	this->collisionPairs.clear();
	this->possibleCollisionPairs.clear();

	// These should be a part of the class not here
	int depth = 30;
	int nodeCapacity = 5;
	QuadTreeNode quadTree(Vector2(0, 0), Vector2(490, 325), nodeCapacity, depth);	// hard coded xy size for now, should be taken from window size
	for (CollisionVolume* v : allColliders) {
		quadTree.Insert(v);
	}

#if DEBUGQUADTREE == 1
	linkedGame->DrawQuadTree(&quadTree);
#endif // DEBUGPHYSICS

	vector<QuadTreeNode*> leafnodes = QuadTreeNode::GetLeafNodes(&quadTree);

	// This is the slowest bit of the collision detection if the quadtree is shallow
	for (QuadTreeNode* leaf : leafnodes) {
		vector<CollisionVolume*>* contents = &leaf->contents;

		for (int i = 0; i < contents->size(); i++) {
			for (int j = i + 1; j < contents->size(); j++) {
				// better name is "collision should be checked"
				CollisionShouldHappen((*contents)[i], (*contents)[j]);
			}
		}
	}


	for (pair<CollisionVolume*, CollisionVolume*> pair : possibleCollisionPairs) {
		bool a = CheckCollision(pair.first, pair.second);
#if DEBUGPHYSICS == 1
					if (a) {
						string obj1 = GetGameObjectTypeString(pair.first);
						string obj2 = GetGameObjectTypeString(pair.second);

						std::cout
							<< obj1
							<< " { "
							<< pair.first
							<< ", "
							<< GetCollisionVolumeCentre(pair.first)
							<< " } "
							<< " Collided with "
							<< obj2
							<< " { "
							<< pair.second
							<< ", "
							<< GetCollisionVolumeCentre(pair.second)
							<< " } "
							<< std::endl;
					}
#endif
	}
}

// These collision volumes should reference the body they're linked to so that there position can be found
// for collision volumes without bodies, e.g. walls and other invisible things, their position is around their centre
// so they wont refernce a linked body
// they shouldnt be moved by collisions or resolutions
bool GameSimsPhysics::CheckCollision(CollisionVolume* c1, CollisionVolume* c2) {
	// check what collision volume type they are
	// then check if they are colliding based on their collision types

	// cv1 will always be a circle in the case of a circle and square collision
	CollisionType cType = CollisionType::CircleToCircle;
	CollisionVolume* cv1 = c1;
	CollisionVolume* cv2 = c2;
	switch (c1->GetCollisionVolumeType() + c2->GetCollisionVolumeType()) {
	case 2:
		cType = CollisionType::CircleToCircle;
		cv1 = c1;
		cv2 = c2;
		break;
	case 3:
		cType = CollisionType::CircleToSquare;
		if (c1->GetCollisionVolumeType() == CollisionVolumeType::Circle) {
			cv1 = c1;
			cv2 = c2;
		}
		else {
			cv1 = c2;
			cv2 = c1;
		}
		break;
	case 4:
		cType = CollisionType::SquareToSquare;
		cv1 = c1;
		cv2 = c2;
		break;
	default:
		return false;
	}

	switch (cType) {
	case CollisionType::CircleToCircle:
		return CircleCircle(cv1, cv2);
	case CollisionType::CircleToSquare:
		return CircleSquare(cv1, cv2);
	case CollisionType::SquareToSquare:
		return SquareSquare(cv1, cv2);
	default:
		break;
	}
	
	return false;
}

// Need some checks first so that different behaviour happens if there is no linked simobject
bool GameSimsPhysics::CircleCircle(CollisionVolume* c1, CollisionVolume* c2) {
	float minCentreDistanceSquared = pow(c1->GetVolumeRadius() + c2->GetVolumeRadius(), 2);
	Vector2 vectorDiff = DiffVec2(&GetCollisionVolumeCentre(c1), &GetCollisionVolumeCentre(c2));
	if (minCentreDistanceSquared - vectorDiff.LengthSquared() > floatingError) {
		// works from running into things from the right and bottom
		float distance = vectorDiff.Length() - (c1->GetVolumeRadius() + c2->GetVolumeRadius());
		Vector2 o1 = c1->GetLinkedObject()->GetPosition();
		Vector2 o2 = c2->GetLinkedObject()->GetPosition();
		this->collisionPairs.push_back(CollisionPair{ c1, c2, vectorDiff.Normalised(), -distance });
		return true;
	}

	return false;
}

bool GameSimsPhysics::CircleSquare(CollisionVolume* circle, CollisionVolume* square) {
	Vector2 circleCentre = GetCollisionVolumeCentre(circle);
	Vector2 squareCentre = GetCollisionVolumeCentre(square);
	Vector2 closestPointOnSquareToCircle = Vector2(
		clamp(circleCentre.x, squareCentre.x - square->GetVolumeXY().x, squareCentre.x + square->GetVolumeXY().x),
		clamp(circleCentre.y, squareCentre.y - square->GetVolumeXY().y, squareCentre.y + square->GetVolumeXY().y));

	Vector2 vectorDiff = DiffVec2(&circleCentre, &closestPointOnSquareToCircle);
	if (vectorDiff.LengthSquared() - pow(circle->GetVolumeRadius(), 2) < floatingError) {
		this->collisionPairs.push_back(CollisionPair{ circle, square, vectorDiff.Normalised(), circle->GetVolumeRadius() - vectorDiff.Length() });

		return true;
	}
	return false;
}

bool GameSimsPhysics::SquareSquare(CollisionVolume* c1, CollisionVolume* c2) {
	Vector2 centreDiff = DiffVec2(&GetCollisionVolumeCentre(c1), &GetCollisionVolumeCentre(c2));
	Vector2 summedHalfSizes = c1->GetVolumeXY() + c2->GetVolumeXY();

	// Negative intersections means that the shapes overlap in that axis
	// the more positive it is the less they are intersecting
	float xIntersection = centreDiff.x - summedHalfSizes.x;
	float yIntersection = centreDiff.y - summedHalfSizes.y;

	// If they intersect in both axis then there is a collision
	if (xIntersection < floatingError && yIntersection < floatingError) {
		// the more positive it is the less they are intersecting
		if (xIntersection - yIntersection > floatingError) {
			// response in x axis
			this->collisionPairs.push_back(CollisionPair{ c1, c2, Vector2(1, 0), abs(xIntersection) });
		}
		else {
			// response in y axis
			this->collisionPairs.push_back(CollisionPair{ c1, c2, Vector2(0, 1), abs(yIntersection) });
		}

		return true;
	}

	return false;
}
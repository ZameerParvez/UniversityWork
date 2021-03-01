#pragma once
#include <algorithm>
#include <set>
#include "../../Common/Vector2.h"
#include "CollisionVolume.h"
#include "RigidBody.h"


#define DEBUGPHYSICS 0
#define DEBUGQUADTREE 1

#if DEBUGQUADTREE == 1
#include "BadFoodGame.h"
#endif // DEBUGQUADTREE

namespace NCL {
	namespace CSC3222 {
		//class RigidBody;
		//class CollisionVolume;

		enum CollisionType {
			CircleToCircle,
			CircleToSquare,
			SquareToSquare
		};

		struct CollisionPair {
			CollisionVolume* c1;
			CollisionVolume* c2;
			Vector2 normal;
			float distance;
		};

		class GameSimsPhysics	{
		public:
#if DEBUGQUADTREE == 1
			BadFoodGame* linkedGame;
#endif
			GameSimsPhysics();
			~GameSimsPhysics();

			void Update(float dt);

			void AddRigidBody(RigidBody* b);
			void RemoveRigidBody(RigidBody* b);

			void AddCollider(CollisionVolume* c);
			void RemoveCollider(CollisionVolume* c);

			std::vector<CollisionVolume*>* GetColliders() {
				return &allColliders;
			}

			string GetGameObjectTypeString(CollisionVolume* c) {
				switch (c->GetLinkedObject()->GetGameObjectType()) {
					case GameObjectType::ImpassableTile:
						return "ImpassableTile"; 
					case GameObjectType::DampeningTile:
						return "DampeningTile"; 
					case GameObjectType::InteractiveTile:
							return "InteractiveTile";
					case GameObjectType::Player:
						return "Player";
					case GameObjectType::Balloon:
						return "Balloon";
					case GameObjectType::BadRobot:
						return "BadRobot";
					case GameObjectType::Laser:
						return "Laser";
					case GameObjectType::Coin:
						return "Coin";
					case GameObjectType::Food:
						return "Food";
					default:
						return "Don't know";
				}
			}
		protected:
			void AccelerationIntegration(float dt);
			void VelocityIntegration(float dt);
			void CollisionDetection();

			void CollisionResolution();
			bool CollisionShouldHappen(CollisionVolume* col1, CollisionVolume* col2);
			bool CheckCollision(CollisionVolume* c1, CollisionVolume* c2);
			bool CircleCircle(CollisionVolume* c1, CollisionVolume* c2);
			bool CircleSquare(CollisionVolume* c1, CollisionVolume* c2);
			bool SquareSquare(CollisionVolume* c1, CollisionVolume* c2);
			
			Vector2 DiffVec2(Vector2* v1, Vector2* v2) {
				return Vector2(abs(v1->x - v2->x), abs(v1->y - v2->y));
			}
			
			Vector2 GetCollisionVolumeCentre(CollisionVolume* c) {
				if (!c->GetLinkedObject()) return c->GetVolumeCentre();
				return (*((RigidBody*)(c->GetLinkedObject()))).GetPosition() + c->GetVolumeCentre();
			}
			
			float clamp(float val, float min, float max) {
				val = std::fmin(val, max);
				val = std::fmax(val, min);
				return val;
			}

			std::vector<RigidBody*>			allBodies;
			std::vector<CollisionVolume*>	allColliders;	// These colliders should only be the parent colliders of objects
			std::vector<CollisionPair> collisionPairs;
			std::set<std::pair<CollisionVolume*, CollisionVolume*>> possibleCollisionPairs;

			//link game here and have method in game to draw quadtree
			const float physicsTimeStep = 1.0 / 120;
			float timeRemaining = 0.0f;
			float floatingError = 0.001;
		};
	}
}


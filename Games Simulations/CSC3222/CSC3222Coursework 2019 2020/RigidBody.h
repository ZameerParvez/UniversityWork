#pragma once
#include "../../Common/Vector2.h"

namespace NCL {
	using namespace Maths;
	namespace CSC3222 {
		enum class GameObjectType {
			ImpassableTile,
			DampeningTile,
			InteractiveTile,
			Player,
			Balloon,
			BadRobot,
			Laser,
			Coin,
			Food
		};

		class RigidBody	{
			friend class GameSimsPhysics;
		public:
			RigidBody(float invMass = 1, float elasticity = 0.5);
			~RigidBody();

			Vector2 GetPosition() const {
				return position;
			}

			void SetPosition(const Vector2& newPosition) {
				position = newPosition;
			}

			Vector2 GetVelocity() const {
				return velocity;
			}

			void SetVelocity(const Vector2& newVelocity) {
				velocity = newVelocity;
			}

			//float getDampening() {
			//	return dampening;
			//}

			//float getInverseMass() {
			//	return inverseMass;
			//}

			//Vector2 GetForce() const {
			//	return force;
			//}

			void AddForce(const Vector2& newForce) {
				force += newForce;
			}

			virtual GameObjectType GetGameObjectType() const {
				return GameObjectType::ImpassableTile;
			};

		protected:
			Vector2 position;
			Vector2 velocity;
			Vector2 force;

			float dampening = 1.0f;
			float inverseMass = 0;
			float elasticity = 0.6f;

		};
	}
}

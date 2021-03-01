#pragma once
#include <vector>
#include "../../Common/Vector2.h"

using namespace NCL;
using namespace Maths;
using namespace std;

namespace NCL {
	namespace CSC3222 {
		class RigidBody;	// this forwards decleration allows you to reference the class becuase it is in the same namespace, but it is different to include because it only holds type information, so you can't use any of its functions

		enum CollisionVolumeType {
			Circle = 1,
			Square = 2
		};

		class CollisionVolume {
		public:
			RigidBody* linkedObject = nullptr;

			CollisionVolume(
				RigidBody*linkedObject,
				Vector2 volumeDisplacementFromSimObject, 
				float x, 
				float y = 0, 
				CollisionVolumeType volumeType = CollisionVolumeType::Circle, 
				const vector<CollisionVolume>* childVolumes = nullptr
			);
			
			CollisionVolume(
				Vector2 volumeCentre,
				float x,
				float y = 0,
				CollisionVolumeType volumeType = CollisionVolumeType::Circle,
				const vector<CollisionVolume>* childVolumes = nullptr
			);

			CollisionVolume() : CollisionVolume{ {0,0}, 1 } {}
			~CollisionVolume();

			CollisionVolumeType GetCollisionVolumeType() const {
				return volumeType;
			}
			
			Vector2 GetVolumeXY() const {
				return Vector2(this->xHalfSize, this->yHalfSize);
			}
			
			Vector2 GetVolumeCentre() const {
				return volumeDisplacement;
			}			

			float GetVolumeRadius() const {
				return radius;
			}
			
			RigidBody* GetLinkedObject() const {
				return linkedObject;
			}

			// this would allow more childVolumes to be added after if needed
			vector<CollisionVolume>& GetChildVolumes() {
				return childVolumes;
			}

		private:
			void init(
				RigidBody* linkedObject,
				Vector2 volumeDisplacementFromSimObject,
				float x,
				float y,
				CollisionVolumeType volumeType,
				const vector<CollisionVolume> * childVolumes
			);

			Vector2 volumeDisplacement;
			vector<CollisionVolume> childVolumes;
			CollisionVolumeType volumeType;
			float radius;
			float yHalfSize;
			float xHalfSize;
		};
	}
}


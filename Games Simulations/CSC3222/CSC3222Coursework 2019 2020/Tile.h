#pragma once
#include "RigidBody.h"
#include "CollisionVolume.h"
namespace NCL {
	namespace Maths {
		class Vector2;
	}
	namespace CSC3222 {
		enum class TileType {
			Dampening,
			Impassable,
			Interactive
		};

		class Tile : public RigidBody {
		public:
			Tile(TileType tType, Vector2 position, Vector2 halfSize, float dampening = 0);
			~Tile();
			
			virtual GameObjectType GetGameObjectType() const {
				return objType;
			};
			
			TileType tileType;
			Vector2 halfSize;
			CollisionVolume	 collisionVol;
			CollisionVolume* collider;
			GameObjectType objType = GameObjectType::ImpassableTile;
		};
	}
}
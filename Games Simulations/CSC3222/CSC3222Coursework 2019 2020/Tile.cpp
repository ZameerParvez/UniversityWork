#include "Tile.h"
using namespace NCL;
using namespace CSC3222;

Tile::Tile(TileType tType, Vector2 position, Vector2 halfSize, float dampening) {
	this->tileType = tType;
	this->position = position;
	this->halfSize = halfSize;

	this->dampening = dampening;
	this->inverseMass = 0;

	switch (tType) {
		case TileType::Impassable:
			this->elasticity = 0.7f;
			this->objType = GameObjectType::ImpassableTile;
			break;
		case TileType::Dampening:
			this->objType = GameObjectType::DampeningTile;
			break;
		case TileType::Interactive:
			this->objType = GameObjectType::InteractiveTile;
			break;
		default:
			this->objType = GameObjectType::ImpassableTile;
			this->elasticity = 1.0f;
	}

	collisionVol = CollisionVolume(this, Vector2(0, 0), halfSize.x, halfSize.y, CollisionVolumeType::Square);

	collider = &collisionVol;

}

Tile::~Tile()
{
}

#include "CollisionVolume.h"

using namespace NCL;
using namespace CSC3222;
using namespace Maths;
using namespace std;

void CollisionVolume::init(
	RigidBody* linkedObject,
	Vector2 centre, 
	float x, 
	float y,
	CollisionVolumeType volumeType, 
	const vector<CollisionVolume>* childVolumes 
) {
	this->linkedObject = linkedObject;
	this->volumeType = volumeType;
	this->volumeDisplacement = centre;

	switch (volumeType) {
		case CollisionVolumeType::Circle:
			this->radius = x;
			this->yHalfSize = 0;
			this->xHalfSize = 0;
			break;
		case CollisionVolumeType::Square:
			this->radius = 0;
			this->xHalfSize = x;
			this->yHalfSize = y;
			break;
		default:
			this->radius = 0;
			this->yHalfSize = 0;
			this->xHalfSize = 0;
	}

	if (childVolumes) {
		this->childVolumes = *childVolumes;
	}

}

CollisionVolume::CollisionVolume(
	Vector2 centre,
	float x, 
	float y, 
	CollisionVolumeType volumeType,
	const vector<CollisionVolume>* childVolumes
) {
	init(nullptr, centre, x, y, volumeType, childVolumes);
}

CollisionVolume::CollisionVolume(
	RigidBody* linkedObject,
	Vector2 volumeDisplacementFromSimObject,
	float x,
	float y,
	CollisionVolumeType volumeType,
	const vector<CollisionVolume> * childVolumes
) {
	init(linkedObject, volumeDisplacementFromSimObject, x, y, volumeType, childVolumes);
}

CollisionVolume::~CollisionVolume() {}



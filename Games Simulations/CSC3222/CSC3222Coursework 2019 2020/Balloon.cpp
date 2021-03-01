#include "Balloon.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"

#define PI 3.14159265

using namespace NCL;
using namespace CSC3222;

Balloon::Balloon() : SimObject() {
	texture = texManager->GetTexture("TL_Creatures.png");

	this->inverseMass = 1000;
	collisionVol = CollisionVolume(this, Vector2(0, 0), 8, 14, CollisionVolumeType::Square);
	collider = &collisionVol;
	dampening = 0.85f;
	elasticity = 0.95f;
}

Balloon::~Balloon() {

}

// A better solution for this sort of thing is build something in the physics loop that handles soft bodies, which would be objects linked to eachother by springs
// this step would add the forces that the springs determine
bool Balloon::UpdateObject(float dt) {
	
	timeAlive = (timeAlive > 10*PI) ? dt: timeAlive + dt;

	// These should be in the physics class so that they update more often, and so dampening is properly taken care of for the forces added by the spring
	// I think that this implementation of spring physics is also incorrect, because changing the values it uses a little makes the balloon behave wildly
	this->force.y += maxFloat * sin(timeAlive * PI);
	if (player) {
		RigidBody* obj = this;
		RigidBody* attachedTo = player;
		float restingSpringLength = 40;
		Vector2 between = obj->GetPosition() - attachedTo->GetPosition();
		Vector2 springDirection = between.Normalised();
		float actualSpringLength = between.Length();// Distance between objects, is the x
		Vector2 force = springDirection * (this->k * (restingSpringLength - actualSpringLength) - this->dampeningconstant * Vector2::Dot(obj->GetVelocity(), springDirection));
		obj->AddForce(force);
	}

	this->force *= dampening;
	return true;
}

void Balloon::DrawObject(GameSimsRenderer& r) {
	Vector2 texPos	= Vector2(73, 99);
	Vector2 texSize = Vector2(14, 28);
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position);
}
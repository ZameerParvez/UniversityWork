#include "BadRobot.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"

using namespace NCL;
using namespace CSC3222;

PlayerCharacter* BadRobot::player = nullptr;
vector<BadRobot*> BadRobot::allRobots = {};

Vector4 animFramesGreen[] = {
	Vector4(71,142,18,18),
	Vector4(102,142,20,18),
	Vector4(135,142,19,18),
	Vector4(166,142,19,18),
	Vector4(198,140,20,18)
};

Vector4 animFramesWhite[] = {
	Vector4(71,266,17,22),
	Vector4(104,265,19,22),
	Vector4(137,266,17,22),
	Vector4(168,266,17,22),
	Vector4(200,264,17,23)
};

BadRobot::BadRobot() : SimObject() {
	texture = texManager->GetTexture("TL_Creatures.png");
	animFrameCount	= 4;

	int r = rand() % 2;

	if (r == 0) {
		type = RobotType::Green;
	}
	else {
		type = RobotType::White;
	}

	inverseMass = 1 / 50.f;
	dampening = 0.6f;

	collisionVol = CollisionVolume(this, Vector2(0, 0), 10);
	collider = &collisionVol;

	allRobots.push_back(this);
}

BadRobot::~BadRobot() {

}

bool BadRobot::UpdateObject(float dt) {
	UpdateAnimFrame(dt);

	force = Vector2(0, 0);
	Vector2 temp;
	float amountOfForce = 1000;
	switch (state)
	{
	case RobotState::Patrol:
		temp = Patrol(dt);
		break;
	case RobotState::Attack:
		temp = Attack(dt);
		break;
	case RobotState::Protect:
		temp = Protect(dt);
		break;
	default:
		break;
	}

	force += temp * amountOfForce;
	return true;
}

// Need a common "travel to" method
// Need "get random free tile" method
Vector2 BadRobot::Patrol(float dt) {
	// if player is more than 80 units away
		// pick a clear tile on map and travel to it. pick another place once reaching there
	Vector2 diff = Vector2(position.x - player->GetPosition().x, position.y - player->GetPosition().y);
	float distance = diff.Length();
	//Vector2 normal = diff.Normalised();
	//do patrols
	// check if time to transition state


	// go for the attack
	if (distance > 80) {
		state = RobotState::Patrol;
	}
	else {
		state = RobotState::Attack;
	}

	//return normal;
	return Vector2();
}

// flocking should happen in attack mode
Vector2 BadRobot::Attack(float dt) {
	// if player is in 80 units
		// robot tries to reach player
		// fires laser in the direction of player every 2 seconds
	targetPosition = player->GetPosition();
	Flocking(&allRobots, dt);

	Vector2 diff = -Vector2(position.x - targetPosition.x, position.y - targetPosition.y);
	float distance = diff.Length();
	Vector2 normal = diff.Normalised();

	// go for the attack
	if (distance > 150) {
		state = RobotState::Patrol;
	}
	else {
		state = RobotState::Attack;
	}

	return normal;
}

// flocking should happen in this mode too
Vector2 BadRobot::Protect(float dt) {
	// if player is holding 5 food
		// robot tries to move to a random tile withing 120 units of the shop entrance
	return Vector2(0, 0);
}

void BadRobot::DrawObject(GameSimsRenderer& r) {
	Vector2	texPos;
	Vector2 texSize;

	switch (type) {
		case RobotType::Green:{
			texPos	= Vector2(animFramesGreen[currentanimFrame].x, animFramesGreen[currentanimFrame].y);
			texSize = Vector2(animFramesGreen[currentanimFrame].z, animFramesGreen[currentanimFrame].w);
		}break;
		case RobotType::White:{
			texPos	= Vector2(animFramesWhite[currentanimFrame].x, animFramesWhite[currentanimFrame].y);
			texSize = Vector2(animFramesWhite[currentanimFrame].z, animFramesWhite[currentanimFrame].w);
		}break;
	}

	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position, false);
}

void BadRobot::Flocking(vector<BadRobot*>* robots, float dt) {

	if (this->state == RobotState::Patrol) return;
	Vector2 dir;
	dir += Alignment(robots);
	dir += Seperation(robots);
	dir += Cohesion(robots);
	dir += Avoidance(robots);
	this->force += dir * 1000;

	// A function like this should really be in the physics loop too
	//for (BadRobot* bot : *robots) {
	//	if (bot->state == RobotState::Patrol) continue;
	//	Vector2 dir;
	//	dir += Alignment(robots);
	//	dir += Seperation(robots);
	//	dir += Cohesion(robots);
	//	dir += Avoidance(robots);
	//	bot->force += dir * 1000;
	//}
}

Vector2 BadRobot::Alignment(vector<BadRobot*>* robots) {
	Vector2 dir = this->velocity;
	float alignmentThreshold = 20;

	for (BadRobot* b : *robots) {
		if (b == this || b->state == RobotState::Patrol) continue;

		float distance = (this->position - b->position).Length();
		if (distance > alignmentThreshold) continue;

		dir += b->velocity;
	}

	return dir.Normalised();
}

Vector2 BadRobot::Seperation(vector<BadRobot*>* robots) {
	Vector2 dir;
	float seperationThreshold = 27;

	for (BadRobot* b : *robots) {
		if (b == this || b->state == RobotState::Patrol) continue;

		float distance = (this->position - b->position).Length();
		if (distance > seperationThreshold) continue;

		float strength = 1.0f - (distance / seperationThreshold);
		dir += (this->position - b->position).Normalised() * strength;
	}

	return dir.Normalised();
}
Vector2 BadRobot::Cohesion(vector<BadRobot*> * robots) {
	Vector2 avgPosition = this->position;
	float cohesionThreshold = 20;
	int count = 1;

	for (BadRobot* b : *robots) {
		if (b == this || b->state == RobotState::Patrol) continue;

		float distance = (this->position - b->position).Length();
		if (distance > cohesionThreshold) continue;

		avgPosition += b->position;
		count++;
	}

	avgPosition = avgPosition / count;
	Vector2 dir = avgPosition - this->position;
	return dir.Normalised();
}

// Not needed for this, but current implementation makes it a little easier for the player to walk through a group of robots
Vector2 BadRobot::Avoidance(vector<BadRobot*>* robots) {
	constexpr float minDistanceApart = 30;
	Vector2 dir;

	float distance = (this->position - player->GetPosition()).Length();
	if (distance > minDistanceApart) return Vector2();

	dir += (this->position - player->GetPosition()).Normalised();

	return dir;
}
#include "PlayerCharacter.h"
#include "TextureManager.h"
#include "../../Common/Window.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

int  charARunY		= 128;
int  charARunX[]	= { 32,64,96,128,160,192,224,256 };
//bool charAFlips[]	= { false, true, true, false, false };

Vector2 charAIdle(64,64);
Vector2 charBIdle(64,64);

int  charBRunY		= 96;
int  charBRunX[]	= { 128,160,192,224,256,288 };
//bool charBFlips[]	= { false, true, true, false, false };

PlayerCharacter::PlayerCharacter() : SimObject() {
	currentAnimDir		= MovementDir::Left;
	SetCharacterType(CharacterType::TYPE_A);
	inverseMass = 1 / 50.0f;
	dampening = 0.6f;
	elasticity = 0.7f;

	// Creates a circular collision volume, which will be centred at the players 0,0 coordinates
	collisionVol = CollisionVolume(this, Vector2(0, 0), 10);
	//collisionVol = CollisionVolume(this, Vector2(0, 0), 10, 15, CollisionVolumeType::Square);
	collider = &collisionVol;
}

PlayerCharacter::~PlayerCharacter() {

}

bool PlayerCharacter::UpdateObject(float dt) {
	float movementForce = 3000;
	force = Vector2(0, 0);

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		currentAnimDir = MovementDir::Up;
		
		force += Vector2(0, -movementForce);
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		currentAnimDir = MovementDir::Down;
		
		force += Vector2(0, movementForce);
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		currentAnimDir = MovementDir::Left;
		currentlyFlipped = true;
		
		force += Vector2(-movementForce, 0);
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		currentAnimDir = MovementDir::Right;
		currentlyFlipped = false;
		
		force += Vector2(movementForce, 0);
	}


	if (force == Vector2()) currentAnimDir = MovementDir::Idle;
	
	UpdateAnimFrame(dt);

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
	}

	return true;
}

void PlayerCharacter::DrawObject(GameSimsRenderer& r) {
	switch (charType) {
		case CharacterType::TYPE_A: DrawCharacterA(r); break;
		case CharacterType::TYPE_B: DrawCharacterB(r); break;
	}
}

void PlayerCharacter::DrawCharacterA(GameSimsRenderer& r) {
	Vector2 screenPos	= position;
	Vector2 texSize		= Vector2(32, 32);

	Vector2	texPos;
	texPos.x = (float)charARunX[currentanimFrame];
	texPos.y = (float)charARunY;

	if (currentAnimDir == MovementDir::Idle) {
		texPos = charAIdle;
	}

	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, screenPos, currentlyFlipped);
}

void PlayerCharacter::DrawCharacterB(GameSimsRenderer& r) {
	Vector2 screenPos	= position;
	Vector2 texSize		= Vector2(32, 32);
	
	Vector2	texPos;
	texPos.x = (float)charBRunX[currentanimFrame];
	texPos.y = (float)charBRunY;

	if (currentAnimDir == MovementDir::Idle) {
		texPos = charBIdle;
	} 

	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, screenPos, currentlyFlipped);
}

void PlayerCharacter::SetCharacterType(CharacterType t) {
	charType = t;
	switch (charType) {
		case CharacterType::TYPE_A:
			texture = texManager->GetTexture("TL_oldcharacters.png"); 
			animFrameCount = 8;
			break;
		case CharacterType::TYPE_B:
			texture = texManager->GetTexture("Twiggy_spritesheet.png"); 
			animFrameCount = 6;
			break;
	}
}
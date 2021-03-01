#include "Food.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"

using namespace NCL;
using namespace CSC3222;

Vector4 foodFrames[] = {
	Vector4(0,0,15,16), //Apple
	Vector4(17,0,14,16),//Can
	Vector4(31,0,18,16),//Pizza
	Vector4(49,0,15,16),//Sundae
	Vector4(64,0,16,16) //Bread
};

Food::Food() : SimObject() {
	texture = texManager->GetTexture("food_items16x16.png");

	foodItem = rand() % 5;

	collisionVol = CollisionVolume(this, Vector2(0, 0), 8, 8, CollisionVolumeType::Square);
	collider = &collisionVol;
}

Food::~Food() {

}

bool Food::UpdateObject(float dt) {
	return true;
}

void Food::DrawObject(GameSimsRenderer& r) {
	Vector2 texPos	= Vector2(foodFrames[currentanimFrame].x, foodFrames[currentanimFrame].y);
	Vector2 texSize = Vector2(foodFrames[currentanimFrame].z, foodFrames[currentanimFrame].w);
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position);
}
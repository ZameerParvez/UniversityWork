#include "BadFoodGame.h"
#include "SimObject.h"
#include "GameMap.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "GameSimsPhysics.h"
#include "PlayerCharacter.h"
#include "BadRobot.h"
#include "Laser.h"
#include "Food.h"
#include "Coin.h"
#include "Balloon.h"

#include "../../Common/Window.h"
#include "../../Common/TextureLoader.h"

using namespace NCL;
using namespace CSC3222;

#define DEBUGLINES 1


/*
position needs to be related to tiles in world space because windows coordinates are not linked to world coordinates
And world coordinates can change depending on the map width x height

So knowing tile type, size and width and height of world in tiles is useful for positioning

To spawn robots in only the enclosed area, spawn them only in that tile type

Read the spec for what behaviour needs to exist

implement force based movement, for things, would need friction and force applied by pressing button makes acceleration, without button acceleration should reduce a lot because of friction
*/

BadFoodGame::BadFoodGame()	{
	renderer	= new GameSimsRenderer();
	texManager	= new TextureManager();
	physics		= new GameSimsPhysics();
#if DEBUGQUADTREE == 1
	physics->linkedGame = this;
#endif // DEBUGLINES


	SimObject::InitObjects(this, texManager);
	InitialiseGame();
}

BadFoodGame::~BadFoodGame()	{
	delete currentMap;
	delete texManager;
	delete renderer;
	delete physics;
}


void BadFoodGame::Update(float dt) {
	for (auto i : newObjects) {
		gameObjects.emplace_back(i);
	}
	newObjects.clear();

	gameTime += dt;

	renderer->Update(dt);
	physics->Update(dt);
	currentMap->DrawMap(*renderer);

	srand((int)(gameTime * 1000.0f));

	for (auto i = gameObjects.begin(); i != gameObjects.end(); ) {
		if (!(*i)->UpdateObject(dt)) { //object has said its finished with
			delete (*i);
			gameObjects.erase(i);
		}
		else {
			(*i)->DrawObject(*renderer);
			++i;
		}
	}

	renderer->DrawString("B4DF00D", Vector2(420, 700));

	renderer->DrawString("Score: " + std::to_string(currentScore), Vector2(10, 10));
	renderer->DrawString("Lives left: " + std::to_string(lives), Vector2(10, 30));
	renderer->DrawString("Food: " + std::to_string(foodCount), Vector2(10, 50));
	renderer->DrawString("Coins: " + std::to_string(coins), Vector2(10, 70));
	renderer->DrawString("Balloons: " + std::to_string(balloons), Vector2(10, 90));
	renderer->DrawString("Player Position: " + std::to_string(player->GetPosition().x) + ", " + std::to_string(player->GetPosition().y), Vector2(10, 110));
	
	if (robots.size() < this->maxBadRobots) {
		if (this->timeToNextRobot > 0) {
			this->timeToNextRobot -= dt;
		}
		else
		{
			SpawnRobot();
			this->timeToNextRobot = this->robotSpawnTime;
		}
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM0)) {
		vector<Vector2> spawnPoints = this->currentMap->importantStructurePoints[MapStructureType::RedBuilding];
		player->SetPosition(spawnPoints[rand() % spawnPoints.size()]);
	}

#if DEBUGLINES == 1
	DrawDebugLines();
#endif

	renderer->Render();
}

void BadFoodGame::InitialiseGame() {
	delete currentMap;
	for (auto o : gameObjects) {
		delete o;
	}
	gameObjects.clear();

	currentMap = new GameMap("BadFoodMap.txt", gameObjects, *texManager);
	currentMap->SetLinkedPhysics(this->physics);
	currentMap->AddTilesToPhysics(); // This is not a good solution

	renderer->SetScreenProperties(16, currentMap->GetMapWidth(), currentMap->GetMapHeight());

	SpawnPlayer();
	BadRobot::player = this->player;
	SpawnRobot();

	Laser* testLaser = new Laser(Vector2(1, 0));
	//Laser* testLaser = new Laser(Vector2(0,0));
	testLaser->SetPosition(Vector2(200, 200));
	AddNewObject(testLaser);

	Food* testFood = new Food();
	testFood->SetPosition(Vector2(250, 200));
	AddNewObject(testFood);

	Coin* testCoin = new Coin();
	testCoin->SetPosition(Vector2(300, 200));
	AddNewObject(testCoin);

	Balloon* testBalloon = new Balloon();
	testBalloon->SetPosition(Vector2(350, 200));
	AddNewObject(testBalloon);

	Balloon* testBalloon2 = new Balloon();
	testBalloon2->SetPosition(Vector2(200, 150));
	AddNewObject(testBalloon2);

	testBalloon2->player = player;

	gameTime		= 0;
	currentScore	= 0;
	foodCount		= 0;
	coins			= 0;
	balloons		= 1;
	lives			= 3;
}

void BadFoodGame::AddNewObject(SimObject* object) {
	newObjects.emplace_back(object);
	physics->AddRigidBody(object);
	if (object->GetCollider()) {
		physics->AddCollider(object->GetCollider());
	}
}

void BadFoodGame::SpawnPlayer() {
	vector<Vector2> spawnPoints = this->currentMap->importantStructurePoints[MapStructureType::RedBuilding];
	player = new PlayerCharacter();
	player->SetCharacterType(PlayerCharacter::CharacterType::TYPE_B);
	player->SetPosition(spawnPoints[rand() % spawnPoints.size()]);
	AddNewObject(player);
}

void BadFoodGame::SpawnRobot() {
	BadRobot* robot = new BadRobot();
	Vector2 position = this->currentMap->robotSpawnTiles[rand() % this->currentMap->robotSpawnTiles.size()];
	robot->SetPosition(position);
	AddNewObject(robot);
	robots.push_back(robot);
}

void BadFoodGame::DrawDebugLines() {
	/*

	Some examples of debug rendering!

	*/
	//renderer->DrawBox(Vector2(16,16), Vector2(8, 8), Vector4(1, 0, 0, 1));
	//renderer->DrawLine(Vector2(16, 16), Vector2(192, 192), Vector4(1, 1, 0, 1));
	//renderer->DrawCircle(Vector2(100, 100), 10.0f, Vector4(1, 0, 1, 1));
	//renderer->DrawBox(Vector2(300, 320), Vector2(1,1), Vector4(1, 0, 0, 1));

	//renderer->DrawBox(Vector2(480, 320), Vector2(1,1), Vector4(1, 0, 0, 1));
	renderer->DrawBox(player->GetPosition(), Vector2(1, 1), Vector4(0, 0, 1, 1));
	/*renderer->DrawBox(player->GetPosition(), Vector2(10, 14), Vector4(1, 0, 0, 1));
	renderer->DrawCircle(player->GetPosition() + player->GetCollider()->GetVolumeCentre(), player->GetCollider()->GetVolumeRadius(), Vector4(1, 0, 1, 1));

	for (auto i = robots.begin(); i != robots.end(); ) {
		auto r = (*i);
		renderer->DrawCircle(r->GetPosition() + r->GetCollider()->GetVolumeCentre(), r->GetCollider()->GetVolumeRadius(), Vector4(1, 0, 1, 1));
		i++;
	}*/

	for (auto i = physics->GetColliders()->begin(); i != physics->GetColliders()->end(); ) {
		auto col = *i;
		auto r = col->GetLinkedObject();
		switch (col->GetCollisionVolumeType()) {
			case CollisionVolumeType::Circle:
				renderer->DrawCircle(r->GetPosition() + col->GetVolumeCentre(), col->GetVolumeRadius(), Vector4(1, 0, 0, 1));
				break;
			case CollisionVolumeType::Square:
				renderer->DrawBox(r->GetPosition() + col->GetVolumeCentre(), col->GetVolumeXY(), Vector4(0, 0, 1, 1));
				break;
		}

		i++;
	}
}

void BadFoodGame::DrawQuadTree(QuadTreeNode* qt) {
	for (QuadTreeNode* q : qt->GetLeafNodes()) {
		Vector2 halfSizes = Vector2(q->xLength / 2, q->yLength / 2);
		Vector2 centre = Vector2(q->xCorner, q->yCorner) + halfSizes;
		renderer->DrawBox(centre, halfSizes, Vector4(0, 1, 1, 1));
	}
}
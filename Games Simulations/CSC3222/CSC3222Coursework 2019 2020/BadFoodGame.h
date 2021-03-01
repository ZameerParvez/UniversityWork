#pragma once
#include <vector>
#include "QuadTreeNode.h"

namespace NCL {
	namespace Maths {
		class Vector2;
	}
	namespace CSC3222 {
		class GameMap;
		class SimObject;
		class GameSimsRenderer;
		class GameSimsPhysics;
		class TextureBase;
		class TextureManager;
		class PlayerCharacter;
		class BadRobot;
		//class Tile;
		//class RigidBody;

		class BadFoodGame {
		public:
			BadFoodGame();
			~BadFoodGame();

			void Update(float dt);

			void AddNewObject(SimObject* object);

			GameSimsPhysics* GetPhysics() {
				return this->physics;
			}

			std::vector<SimObject*>* GetNewObjects() {
				return &this->newObjects;
			}

			void DrawQuadTree(QuadTreeNode* qt);

		protected:
			void InitialiseGame();
			void SpawnRobot();
			void SpawnPlayer();
			void DrawDebugLines();

			TextureManager*		texManager;
			GameSimsRenderer*	renderer;
			GameSimsPhysics*	physics;
			GameMap* currentMap;

			PlayerCharacter* player;

			float gameTime;

			int currentScore;
			int foodCount;
			int coins;
			int balloons;
			int lives;

			int maxBadRobots = 20;
			float robotSpawnTime = 5;
			float timeToNextRobot = robotSpawnTime;
			std::vector<BadRobot*> robots;

			std::vector<SimObject*> gameObjects;
			std::vector<SimObject*> newObjects;
		};
	}
}

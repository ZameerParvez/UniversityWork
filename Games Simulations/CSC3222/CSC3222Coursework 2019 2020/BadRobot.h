#pragma once
#include "SimObject.h"
#include "PlayerCharacter.h"

namespace NCL {
	namespace CSC3222 {
		enum class RobotState {
			Patrol,
			Attack,
			Protect
		};
		class BadRobot : public SimObject
		{
		public:
			BadRobot();
			~BadRobot();

			bool UpdateObject(float dt) override;
			void DrawObject(GameSimsRenderer& r) override;

			virtual GameObjectType GetGameObjectType() const {
				return GameObjectType::BadRobot;
			};

			Vector2 Patrol(float dt);
			Vector2 Attack(float dt);
			Vector2 Protect(float dt);

			// this will be called in the main game class
			void Flocking(vector<BadRobot*>* robots, float dt);
			Vector2 Alignment(vector<BadRobot*>* robots);
			Vector2 Seperation(vector<BadRobot*>* robots);
			Vector2 Cohesion(vector<BadRobot*>* robots);
			Vector2 Avoidance(vector<BadRobot*>* robots);

			Vector2 targetPosition;
			RobotState state = RobotState::Patrol;
			static vector<BadRobot*> allRobots;
			static PlayerCharacter* player;
		protected:
			enum class RobotType {
				Green,
				White,
			};
			RobotType type;
		};
	}
}


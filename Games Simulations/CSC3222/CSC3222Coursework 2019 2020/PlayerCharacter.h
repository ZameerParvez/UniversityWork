#pragma once
#include "SimObject.h"

namespace NCL {
	namespace CSC3222 {
		class PlayerCharacter : public SimObject
		{
		public:
			enum class CharacterType {
				TYPE_A,
				TYPE_B
			};
			PlayerCharacter();
			~PlayerCharacter();

			bool UpdateObject(float dt) override;

			void DrawObject(GameSimsRenderer& r) override;

			void SetCharacterType(CharacterType t);

			virtual GameObjectType GetGameObjectType() const {
				return GameObjectType::Player;
			};
		protected:
			enum class MovementDir {
				Up,
				Down,
				Left,
				Right,
				Idle
			};
			void DrawCharacterA(GameSimsRenderer& r);
			void DrawCharacterB(GameSimsRenderer& r);

			MovementDir		currentAnimDir;
			CharacterType	charType;

			bool currentlyFlipped = false;
		};
	}
}


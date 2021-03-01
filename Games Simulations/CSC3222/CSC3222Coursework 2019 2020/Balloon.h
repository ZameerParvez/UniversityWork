#pragma once
#include "SimObject.h"
#include "PlayerCharacter.h"

namespace NCL {
	namespace CSC3222 {
		class Balloon : public SimObject
		{
		public:
			Balloon();
			~Balloon();

			bool UpdateObject(float dt) override;
			void DrawObject(GameSimsRenderer& r) override;

			virtual GameObjectType GetGameObjectType() const {
				return GameObjectType::Balloon;
			};

			PlayerCharacter* player = nullptr;
		protected:
			float timeAlive = 0;
			float maxFloat = 0.0005f;

			// numbers in F = -kx - c.v
			float dampeningconstant = 0.01; // 0 < c < 1
			float k = 0.0008;
		};
	}
}


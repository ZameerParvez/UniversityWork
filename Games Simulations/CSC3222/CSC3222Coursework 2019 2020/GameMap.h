#pragma once
#include <string>
#include <vector>
#include <map>

#include "../../Common/Vector2.h"
#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"
#include "../../Common/MeshGeometry.h"

#include "BadFoodGame.h"

#include "Tile.h"

namespace NCL {
	namespace Rendering {
		class TextureBase;
	}
	using namespace Maths;
	namespace CSC3222 {
		class GameSimsRenderer;
		class SimObject;
		class TextureManager;
		
		enum MapStructureType {
			Tree,
			BlueBuilding,
			RedBuilding,
			GreenBuilding
		};

		struct StructureData {
			Vector2				startPos;
			MapStructureType	structureType;
		};

		class GameMap	{
		public:
			GameMap(const std::string& filename, std::vector<SimObject*>& objects, TextureManager& texManager);
			~GameMap();

			void DrawMap(GameSimsRenderer & r);

			int GetMapWidth() const {
				return mapWidth;
			}

			int GetMapHeight() const {
				return mapHeight;
			}

			char GetTileTypeAtPos(const Vector2 &pos) const {
				int yTilePos = (int)pos.y / 16;
				int xTilePos = (int)pos.x / 16;
				int tileIndex = yTilePos * mapWidth + xTilePos;
				return this->mapData[tileIndex];
			}

			vector<Vector2> robotSpawnTiles;
			std::map<MapStructureType, std::vector<Vector2>> importantStructurePoints;

			void SetLinkedPhysics(GameSimsPhysics* p) {
				this->linkedPhysics = p;
			}

			// This might not be the right way to do things
			// need a way to be able to use the collision info on the objects
			void AddImpassableBody(Vector2 position, Vector2 tileWidth);
			void AddDampeningBody(Vector2 position, Vector2 tileWidth, float dampening);
			void AddInteractiveCollisionBody(Vector2 position, Vector2 tileWidth);
			void AddTilesToPhysics();

			Vector2 WorldPosToTilePos(Vector2 worldPos) const;
			Vector2 TilePosToWorldPos(Vector2 tilePos) const;
			Vector2 GetRandomPosition() const;


		protected:
			void BuildMapMesh();
			void AddNewTilePos(int x, int y, std::vector<Vector3>& pos);
			void AddNewTileTex(int x, int y, std::vector<Vector2>& tex);


			int mapWidth;
			int mapHeight;
			int structureCount;

			Rendering::TextureBase*	tileTexture;
			char*	mapData;
			int*			mapCosts;
			Vector2*		mapTexCoords;
			StructureData*	structureData;		

			MeshGeometry* mapMesh;

			GameSimsPhysics* linkedPhysics = nullptr;
			vector<Tile*> tiles;
		};
	}
}


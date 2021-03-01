#include "GameMap.h"
#include "GameSimsRenderer.h"
#include "TextureManager.h"
#include "../../Common/Maths.h"
#include "../../Common/Assets.h"
#include "../../Common/TextureLoader.h"
#include <fstream>
#include <iostream>
#include "GameSimsPhysics.h"

using namespace NCL;
using namespace CSC3222;
using namespace Rendering;

Vector4 buildingTypes[4] = {
	Vector4(320,16,64,80),	//tree
	Vector4(16,16,96,80), //blue building
	Vector4(128,32,64,64), //red building
	Vector4(208,32,96,64)  //green building
};

GameMap::GameMap(const std::string& filename, std::vector<SimObject*>& objects, TextureManager& texManager)	{
	tileTexture = texManager.GetTexture("badfoodTiles.png");

	std::ifstream mapFile(Assets::DATADIR + filename);

	if (!mapFile) {
		std::cout << "GameMap can't be loaded in!" << std::endl;
		return;
	}

	mapFile >> mapWidth;
	mapFile >> mapHeight;

	mapData		= new char[mapWidth * mapHeight];
	mapCosts	= new int[mapWidth * mapHeight];

	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			int tileIndex = (y * mapWidth) + x;
			mapFile >> mapData[tileIndex];
		}
	}

	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			int tileIndex = (y * mapWidth) + x;
			char c;
			mapFile >> c;
			mapCosts[tileIndex] = c - '0';
		}
	}

	BuildMapMesh();

	mapFile >> structureCount;

	structureData = new StructureData[structureCount];

	for (int i = 0; i < structureCount; ++i) {
		int type  = 0;
		int xTile = 0;
		int yTile = 0;

		mapFile >> type;
		mapFile >> xTile;
		mapFile >> yTile;

		structureData[i].structureType = (MapStructureType)type;
		structureData[i].startPos.x = xTile * 16.0f;
		structureData[i].startPos.y = yTile * 16.0f;

		if (importantStructurePoints.find(structureData[i].structureType) == importantStructurePoints.end()) {
			importantStructurePoints[structureData[i].structureType];
		}

		Vector4 buildingData = buildingTypes[structureData[i].structureType];

		switch (structureData[i].structureType) {
			case MapStructureType::RedBuilding:
			case MapStructureType::BlueBuilding:
			case MapStructureType::GreenBuilding:
			case MapStructureType::Tree:
				importantStructurePoints[structureData[i].structureType].push_back((structureData[i].startPos + Vector2(0, buildingData.w / 2)));
				break;
		}
	}
}

GameMap::~GameMap()	{
	delete[] mapData;
	delete[] mapCosts;
	delete[] mapTexCoords;

	delete mapMesh;
}

// need a map of structure type to vector of positions, so that spawn points and other points of interest can be calculated
// The structures are defined in the data text file by there type (0-4) and the position of their top left tile, and they are the size described by the building types array where the vector is {x1, y1, x2, y2}
// So I need the {x,y} for where the doors are and the topleft {x,y}
void GameMap::DrawMap(GameSimsRenderer & r) {
	r.DrawMesh((OGLMesh*)mapMesh, (OGLTexture*)tileTexture);
	//structures then go on top
	for (int i = 0; i < structureCount; ++i) {
		Vector4 buildingData = buildingTypes[structureData[i].structureType];

		// need to add colliders for doors and buildings here
		Vector2 texPos  = Vector2(buildingData.x, buildingData.y);
		Vector2 texSize = Vector2(buildingData.z, buildingData.w);

		Vector2 screenPos = structureData[i].startPos;

		r.DrawTextureArea((OGLTexture*)tileTexture, texPos, texSize, screenPos, false);
	}
}

void GameMap::BuildMapMesh() {
	vector<Vector2> texCoords;
	vector<Vector3> positions;

	Vector2 flatGrassTile = Vector2(288, 144);
	Vector2 primaryTile;
	Vector2 secondaryTile = flatGrassTile; //some sit on top of another tile type, usually grass!

	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			int tileType = mapData[(y * mapWidth) + x];
			bool doSecondary = false;

			switch (tileType) {
				//the normal tile types first
				case 'A':primaryTile = Vector2(272,112);break; //top left grass
				case 'B':primaryTile = Vector2(288, 112); break;//top grass
				case 'C':primaryTile = Vector2(320, 112); break;//top right grass
				case 'D':primaryTile = Vector2(272, 128); break;//left grass
				case 'E':primaryTile = Vector2(320, 128); break;//right grass
				case 'F':primaryTile = Vector2(272, 160); break;//bottom left
				case 'G':primaryTile = Vector2(320, 160); break;//bottom right
				case 'H':primaryTile = Vector2(288, 160); break;//bottom grass

				case 'I':primaryTile = Vector2(336, 112); break; //top left grass
				case 'J':primaryTile = Vector2(352, 112); break; //top right grass
				case 'K':primaryTile = Vector2(336, 128); break; //bottom left grass
				case 'L':primaryTile = Vector2(352, 128); break; //bottom right grass

				case 'M':primaryTile = Vector2(208, 112); doSecondary = true;break; //top left fence
				case 'N':primaryTile = Vector2(224, 112); doSecondary = true;break; //top fence
				case 'O':primaryTile = Vector2(240, 112); doSecondary = true;break; //top right fence
				case 'P':primaryTile = Vector2(208, 128); doSecondary = true;break; //left fence
				case 'Q':primaryTile = Vector2(208, 144); doSecondary = true;break; //bottom left fence
				case 'R':primaryTile = Vector2(240, 144); doSecondary = true;break; //bottom right fence
//Stone wall bits
				case 'a':primaryTile = Vector2(16, 112); doSecondary = true; break; //top left grass
				case 'b':primaryTile = Vector2(32, 112); break;//Horizontal A
				case 'c':primaryTile = Vector2(32, 128); break;//Horizontal B
				case 'd':primaryTile = Vector2(144, 112); doSecondary = true; break;//top right grass
				case 'e':primaryTile = Vector2(144, 128); break;//Side
				case 'f':primaryTile = Vector2(16, 160); break;//bottom left A
				case 'g':primaryTile = Vector2(16, 176); doSecondary = true; break;//bottom left B
	
				case 'h':primaryTile = Vector2(64, 160); doSecondary = true; break;//right hand corner A
				case 'i':primaryTile = Vector2(64, 176); doSecondary = true; break;//right hand corner B

				case 'j':primaryTile = Vector2(96, 160); doSecondary = true; break;//Left hand corner A
				case 'k':primaryTile = Vector2(96, 176); doSecondary = true; break;//Left hand corner B

				case 'l':primaryTile = Vector2(144, 160); break;//Bottom right A
				case 'm':primaryTile = Vector2(144, 176); doSecondary = true;  break;//Bottom right B
				case 'n':primaryTile = Vector2(32, 176); doSecondary = true; break;//bottom horizontal

				case 'o':primaryTile = Vector2(80, 176); doSecondary = true; break;//Stone to grass transition
				case 'p':primaryTile = Vector2(80, 176); doSecondary = true; break;//stone to road transition

				case '1':primaryTile = Vector2(352, 144); break; //Road Tile
				case '2':										//Stone Tile
					primaryTile = Vector2(64, 144);
					this->robotSpawnTiles.push_back(Vector2(x + 0.5, y) * 16);	// x and y are in tile space
					break;
				case '3':primaryTile = Vector2(16, 192); doSecondary  = true; break; //Flower A
				case '4':primaryTile = Vector2(32, 192); doSecondary  = true; break;//Flower B
				case '5':primaryTile = Vector2(48, 192); doSecondary  = true; break;//Flower C
				case '6':primaryTile = Vector2(64, 192); doSecondary  = true; break;//Flower D
				case '7':primaryTile = Vector2(176, 176); doSecondary = true; break;//Sign A
				case '8':primaryTile = Vector2(192, 176); doSecondary = true; break;//Sign B
				case '9':primaryTile = Vector2(208, 176); doSecondary = true; break;//Sign C	
				case '0': {
					static Vector2 grassTiles[] = {
						Vector2(288,128),	//	
						Vector2(304,144),	//
						Vector2(304,128),	//
						Vector2(288,144),
					};
					primaryTile = grassTiles[Maths::Clamp(rand() % 20, 0, 3)];
				}break;//Random grass tiles
			}
			switch (tileType) {
				case '0':
					//this->AddDampeningBody(Vector2(x + 0.5, y) * 16, Vector2(1, 1) * 8, 0.5);
					break;
				case 'M': //top left fence
				case 'N': //top fence
				case 'O': //top right fence
				case 'P': //left fence
				case 'Q': //bottom left fence
				case 'R': //bottom right fence
					this->AddImpassableBody(Vector2(x + 0.5, y + 0.5) * 16, Vector2(1, 1) * 8);
					break;
				case 'a': // The following are for the stone walls around the robots
				case 'b':
				case 'c':
				case 'e':
				case 'f':
				case 'g':
				case 'h':
				case 'i':
				case 'j':
				case 'k':
				case 'l':
				case 'm':
				case 'n':
					this->AddImpassableBody(Vector2(x + 0.5, y) * 16, Vector2(1, 1) * 8);
					break;

			}
			if (doSecondary) {
				AddNewTilePos(x, y, positions);
				AddNewTileTex((int)secondaryTile.x, (int)secondaryTile.y, texCoords);
				secondaryTile = flatGrassTile;
			}
			AddNewTilePos(x, y, positions);
			AddNewTileTex((int)primaryTile.x, (int)primaryTile.y, texCoords);

			switch (tileType) {
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
				case 'G':
				case 'H':
				case 'I':
				case 'J':
				case 'K':
				case 'L':
				case 'M':
				case 'N':
				case 'O':
				case 'P':
				case 'Q':
				case 'R':
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'g':
				case 'h':
				case 'i':
				case 'j':
				case 'k':
				case 'l':
				case 'm':
				case 'n':
				case 'o':
				case 'p':
				default:
					break;
			}
		}
	}
	mapMesh = new OGLMesh();
	mapMesh->SetVertexPositions(positions);
	mapMesh->SetVertexTextureCoords(texCoords);
	mapMesh->SetPrimitiveType(NCL::GeometryPrimitive::Triangles);
	mapMesh->UploadToGPU();
}

void GameMap::AddNewTilePos(int x, int y, std::vector<Vector3>& pos) {
	Vector3 topLeft		= Vector3((x + 0) * 16.0f, (y + 0) * 16.0f, 0);
	Vector3 topRight	= Vector3((x + 1) * 16.0f, (y + 0) * 16.0f, 0);
	Vector3 bottomLeft	= Vector3((x + 0) * 16.0f, (y + 1) * 16.0f, 0);
	Vector3 bottomRight = Vector3((x + 1) * 16.0f, (y + 1) * 16.0f, 0);

	pos.emplace_back(topLeft);
	pos.emplace_back(bottomLeft);
	pos.emplace_back(topRight);

	pos.emplace_back(topRight);
	pos.emplace_back(bottomLeft);
	pos.emplace_back(bottomRight);
}

void GameMap::AddNewTileTex(int x, int y, std::vector<Vector2>& tex) {
	Vector2 topLeft		= Vector2((x + 0.0f) , (y + 0.0f));
	Vector2 topRight	= Vector2((x + 16.0f), (y + 0.0f));
	Vector2 bottomLeft	= Vector2((x + 0.0f) , (y + 16.0f));
	Vector2 bottomRight = Vector2((x + 16.0f), (y + 16.0f));

	tex.emplace_back(topLeft);
	tex.emplace_back(bottomLeft); 
	tex.emplace_back(topRight);

	tex.emplace_back(topRight);
	tex.emplace_back(bottomLeft);
	tex.emplace_back(bottomRight);
}

// linked physics isnt defined early enough
void GameMap::AddImpassableBody(Vector2 position, Vector2 tileWidth) {
	Tile* t = new Tile(TileType::Impassable, position, tileWidth);
	this->tiles.emplace_back(t);
}

void GameMap::AddDampeningBody(Vector2 position, Vector2 tileWidth, float dampening) {
	Tile* t = new Tile(TileType::Dampening, position, tileWidth, dampening);
	this->tiles.emplace_back(t);
}

void GameMap::AddInteractiveCollisionBody(Vector2 position, Vector2 tileWidth) {
	Tile* t = new Tile(TileType::Interactive, position, tileWidth);
	this->tiles.emplace_back(t);
}

void GameMap::AddTilesToPhysics() {
	for (Tile* t : this->tiles) {
		linkedPhysics->AddRigidBody(t);
		linkedPhysics->AddCollider(t->collider);
	}
}

Vector2 GameMap::WorldPosToTilePos(Vector2 worldPos) const {
	return Vector2(((int) worldPos.x) / 16, ((int)worldPos.y) / 16);
}

// returns the centre of the tile
Vector2 GameMap::TilePosToWorldPos(Vector2 tilePos) const{
	return tilePos * 16 + Vector2(8, 8);
}

Vector2 GameMap::GetRandomPosition() const {
	int randomTileIndex = std::rand() % (mapWidth * mapHeight);

	while (mapCosts[randomTileIndex] != 0) {
		randomTileIndex = std::rand() % (mapWidth * mapHeight);
	}
	auto tilepos = Vector2(randomTileIndex % mapWidth, randomTileIndex / mapWidth);
	auto worldpos = TilePosToWorldPos(tilepos);
	return worldpos;
}
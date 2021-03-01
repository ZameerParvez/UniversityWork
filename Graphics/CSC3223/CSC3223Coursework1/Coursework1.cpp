#include "../../Common/Window.h"
#include "../../Common/TextureLoader.h"
#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"
#include "../../Common/MeshGeometry.h"
#include "../../Common/Maths.h"

#include "../../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"

#include "Renderer.h"
#include "Controls.h"

// including the hack that makes it run properly
#include "../../Common/HackySleep.h"

using namespace NCL;
using namespace CSC3223;

const unsigned int RANDSEED = 12981212351699124512;

Vector3 viewPosition;
Vector3 viewRotation;
Matrix4 rotationMatrix;
float farPlane = 1000.0f;	// hardcoded, but should match the controls

void tutorial5(Renderer& renderer);

int randIntToN(int n);
float randFloatToN(double n);

void generateOriginalStars(int n);
void updateStarField(Renderer& r, int numberOfStarsToGenerate, float maxScale);

// Bounding box for starfield
// new is used to make new stars in a larger box as you move around
bool starFieldInitialised = false;
Vector3 minStarFieldOld(0, 0, 0);
Vector3 maxStarFieldOld(0, 0, 0);
Vector3 minStarFieldNew(0, 0, 0);
Vector3 maxStarFieldNew(0, 0, 0);
vector<OGLMesh*> starTypes;
int maxNumOfStars = 1000000;
int numOfStars = 0;

void generateOriginalAsteroids(int n);
// should update much less often
void updateAsteroidField(Renderer& r, int numberOfAsteroidsToGenerate, float maxScale);

bool asteroidFieldInitialised = false;
Vector3 minAsteroidFieldOld(0, 0, 0);
Vector3 maxAsteroidFieldOld(0, 0, 0);
Vector3 minAsteroidFieldNew(0, 0, 0);
Vector3 maxAsteroidFieldNew(0, 0, 0);
vector<OGLMesh*> asteroidTypes;
int maxNumOfAsteroids = 1000;
int numOfAsteroids = 0;

// Planets would be good to add, but they would require more spherical shapes

OGLMesh* spaceShip;
RenderObject* spaceShipRO;
RenderObject* s;
void generateSpaceShip();
void updateSpaceShip(Renderer& r);

int main() {
	// Seeded random number generator, so generated items are the same in different runs
	srand(RANDSEED);
	
	Window*w = Window::CreateGameWindow("CSC3223 Coursework 1!", 1920, 1080, false);
	if (!w->HasInitialised()) {
		return -1;
	}

	Controls* ctrls = Controls::getControls();
	Renderer* renderer = ctrls->initRenderer(*w);

	ctrls->setCameraType(CameraType::NOTQUITEFREE);
	ctrls->setSpeed(15);
	
	viewPosition = ctrls->getViewPos();
	viewRotation = ctrls->getViewRotation();
	rotationMatrix = ctrls->getRotationMatrix();

	glClearColor(0, 0, 0, 1);

	tutorial5(*renderer);

	generateOriginalAsteroids(40);
	updateAsteroidField(*renderer, 5, 20);

	generateOriginalStars(50);
	updateStarField(*renderer, 2000, 4);

	generateSpaceShip();
	updateSpaceShip(*renderer);

	float totalDeltaTime = 0;
	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {

		HACKYSLEEPER

		float time = w->GetTimer()->GetTimeDelta();
		totalDeltaTime += time;

		renderer->Update(time);

		renderer->Render();

		ctrls->renderControls(*renderer);
		ctrls->cameraControls(*renderer);

		w->SetTitle(std::to_string(time) + ctrls->getInfo());

		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_NEXT)) {
			w->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_HOME)) {
			w->SetFullScreen(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KEYBOARD_END)) {
			w->SetFullScreen(false);
		}

		viewPosition = ctrls->getViewPos();
		viewRotation = ctrls->getViewRotation();
		rotationMatrix = ctrls->getRotationMatrix();

		updateStarField(*renderer, 30, 2);
		updateSpaceShip(*renderer);

		if ((int)totalDeltaTime % 15  == 0) {
			updateAsteroidField(*renderer, 1, 10);
		}
	}

	delete renderer;

	Window::DestroyGameWindow();
}

// These methods might reduce the actual randomness of rand()
int randIntToN(int n) {
	return rand() % (n + 1);
}

float randFloatToN(double n) {
	return ((double)rand() / RAND_MAX) * n;
}

void updateSpaceShip(Renderer& r) {
	const Matrix4 scale = Matrix4::Scale(Vector3({ 10, 5, 20 }));
	
	Vector3 infrontOfCamera = rotationMatrix * Vector3({ 0, -10, -50 });
	Matrix4 translate = Matrix4::Translation(Vector3(viewPosition.x, viewPosition.y, viewPosition.z) + infrontOfCamera);
	Matrix4 modelMat = translate * rotationMatrix * scale;

	spaceShipRO->SetTransform(modelMat);

	r.AddRenderObject(spaceShipRO);
}

void generateSpaceShip() {
	vector<Vector3> shipShape = {
		Vector3(-1, -1, 0), Vector3(0, -0.5, -1), Vector3(0, 0, -2), Vector3(1, -1, 0), Vector3(0, -0.5, -1)
	};
	
	vector<Vector4> shipColours;
	for (int j = 0; j < shipShape.size(); j++) {
		shipColours.push_back({ Vector4(randFloatToN(1), randFloatToN(1), randFloatToN(1), 1) });
	}

	spaceShip = new OGLMesh();
	spaceShip->SetVertexPositions(shipShape);
	spaceShip->SetVertexColours(shipColours);
	spaceShip->SetPrimitiveType(GeometryPrimitive::TriangleStrip);
	spaceShip->UploadToGPU();

	spaceShipRO = new RenderObject(spaceShip);
}

// Would be nice to develop an algorithm to generate an asteroid looking object within the bounds of some parameters
void generateOriginalAsteroids(int n) {
	// Have n types of Asteroids, colour is randomised and shape is chosen at random from existing sets of points
	vector<vector<Vector3>> asteroidShapes = {
		{
			Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), Vector3(-1, 0, 0), Vector3(1, 0, 0), Vector3(0, -1, 0), Vector3(0, 0, -1)
		}
	};

	for (int i = 0; i < n; i++) {
		//Vector4 colour = { Vector4(randFloatToN(1), randFloatToN(1), randFloatToN(1), 1) };
		OGLMesh* asteroid = new OGLMesh();
		vector<Vector3> asteroidShape = asteroidShapes[randIntToN(asteroidShapes.size() - 1)];
		vector<Vector4> asteroidColours;
		for (int j = 0; j < asteroidShape.size(); j++) {
			asteroidColours.push_back({ Vector4(randFloatToN(1), randFloatToN(1), randFloatToN(1), 1) });
		}

		asteroid->SetVertexPositions(asteroidShape);
		asteroid->SetVertexColours(asteroidColours);
		asteroid->SetPrimitiveType(GeometryPrimitive::TriangleStrip);
		asteroid->UploadToGPU();
		asteroidTypes.push_back(asteroid);
	}
}

void updateAsteroidField(Renderer& r, int numberOfAsteroidsToGenerate, float maxScale) {
	if (numOfAsteroids > maxNumOfAsteroids) return;
	// Could be further optimised if Asteroids verticies were only generated in view
	// if the boundary has expanded set the new boundary to the expanded size, if not keep it the same as the old one
	maxAsteroidFieldNew.x = (viewPosition.x + farPlane > maxAsteroidFieldOld.x) ? viewPosition.x + farPlane : maxAsteroidFieldOld.x;
	maxAsteroidFieldNew.y = (viewPosition.y + farPlane > maxAsteroidFieldOld.y) ? viewPosition.y + farPlane : maxAsteroidFieldOld.y;
	maxAsteroidFieldNew.z = (viewPosition.z + farPlane > maxAsteroidFieldOld.z) ? viewPosition.z + farPlane : maxAsteroidFieldOld.z;

	minAsteroidFieldNew.x = (viewPosition.x - farPlane < minAsteroidFieldOld.x) ? viewPosition.x - farPlane : minAsteroidFieldOld.x;
	minAsteroidFieldNew.y = (viewPosition.y - farPlane < minAsteroidFieldOld.y) ? viewPosition.y - farPlane : minAsteroidFieldOld.y;
	minAsteroidFieldNew.z = (viewPosition.z - farPlane < minAsteroidFieldOld.z) ? viewPosition.z - farPlane : minAsteroidFieldOld.z;


	Vector3 maxAsteroidFieldOldCopy(maxAsteroidFieldOld.x, maxAsteroidFieldOld.y, maxAsteroidFieldOld.z);
	Vector3 minAsteroidFieldOldCopy(minAsteroidFieldOld.x, minAsteroidFieldOld.y, minAsteroidFieldOld.z);

	maxAsteroidFieldOld.x = maxAsteroidFieldNew.x;
	maxAsteroidFieldOld.y = maxAsteroidFieldNew.y;
	maxAsteroidFieldOld.z = maxAsteroidFieldNew.z;

	minAsteroidFieldOld.x = minAsteroidFieldNew.x;
	minAsteroidFieldOld.y = minAsteroidFieldNew.y;
	minAsteroidFieldOld.z = minAsteroidFieldNew.z;

	if (!asteroidFieldInitialised) {
		for (int i = 0; i < numberOfAsteroidsToGenerate; i++) {
			if (numOfAsteroids++ > maxNumOfAsteroids) return;

			int x = (randFloatToN(1) > 0.5) ? randFloatToN(maxAsteroidFieldNew.x) : randFloatToN(minAsteroidFieldNew.x);
			int y = (randFloatToN(1) > 0.5) ? randFloatToN(maxAsteroidFieldNew.y) : randFloatToN(minAsteroidFieldNew.y);
			int z = (randFloatToN(1) > 0.5) ? randFloatToN(maxAsteroidFieldNew.z) : randFloatToN(minAsteroidFieldNew.z);

			Matrix4 translate = Matrix4::Translation(Vector3(x, y, z));
			Matrix4 rotate =
				Matrix4::Rotation(randFloatToN(90), Vector3({ 1, 0, 0 }))
				* Matrix4::Rotation(randFloatToN(90), Vector3({ 0, 1, 0 }))
				* Matrix4::Rotation(randFloatToN(90), Vector3({ 0, 0, 1 }));
			Matrix4 scale = Matrix4::Scale(Vector3({ randFloatToN(maxScale), randFloatToN(maxScale), randFloatToN(maxScale) }));
			Matrix4 modelMat = translate * rotate * scale;

			// randomly choose from mesh in AsteroidTypes list
			int AsteroidType = randIntToN(asteroidTypes.size() - 1);
			r.AddRenderObject(new RenderObject(asteroidTypes[AsteroidType], modelMat));
		}

		asteroidFieldInitialised = true;
		return;
	}

	for (int i = 0; i < numberOfAsteroidsToGenerate; i++) {
		if (numOfAsteroids++ > maxNumOfAsteroids) return;

		// problem is maxAsteroidFieldNew.x - maxAsteroidFieldOld.x is 0 when expanding
		// if 0 that means that barrier didnt change
		// so it should be fine to choose any value in the one that didnt grow
		int x = (randFloatToN(1) > 0.5) ? randFloatToN(maxAsteroidFieldNew.x) : randFloatToN(minAsteroidFieldNew.x);
		int y = (randFloatToN(1) > 0.5) ? randFloatToN(maxAsteroidFieldNew.y) : randFloatToN(minAsteroidFieldNew.y);
		int z = (randFloatToN(1) > 0.5) ? randFloatToN(maxAsteroidFieldNew.z) : randFloatToN(minAsteroidFieldNew.z);

		if (abs(maxAsteroidFieldNew.x - maxAsteroidFieldOldCopy.x) > 0) {
			x = maxAsteroidFieldOldCopy.x + randFloatToN(maxAsteroidFieldNew.x - maxAsteroidFieldOldCopy.x);
		}
		else if (abs(minAsteroidFieldNew.x - minAsteroidFieldOldCopy.x) > 0) {
			x = minAsteroidFieldOldCopy.x - randFloatToN(minAsteroidFieldNew.x - minAsteroidFieldOldCopy.x);
		}
		else if (abs(maxAsteroidFieldNew.y - maxAsteroidFieldOldCopy.y) > 0) {
			y = maxAsteroidFieldOldCopy.y + randFloatToN(maxAsteroidFieldNew.y - maxAsteroidFieldOldCopy.y);
		}
		else if (abs(minAsteroidFieldNew.y - minAsteroidFieldOldCopy.y) > 0) {
			y = minAsteroidFieldOldCopy.y - randFloatToN(minAsteroidFieldNew.y - minAsteroidFieldOldCopy.y);
		}
		else if (abs(maxAsteroidFieldNew.z - maxAsteroidFieldOldCopy.z) > 0) {
			z = maxAsteroidFieldOldCopy.z + randFloatToN(maxAsteroidFieldNew.z - maxAsteroidFieldOldCopy.z);
		}
		else if (abs(minAsteroidFieldNew.z - minAsteroidFieldOldCopy.z) > 0) {
			z = minAsteroidFieldOldCopy.z - randFloatToN(minAsteroidFieldNew.z - minAsteroidFieldOldCopy.z);
		}
		else
		{
			// If none of the boundaries have changed, then no new Asteroids need to be drawn
			return;
		}

		Matrix4 translate = Matrix4::Translation(Vector3(x, y, z));
		Matrix4 rotate =
			Matrix4::Rotation(randFloatToN(90), Vector3({ 1, 0, 0 }))
			* Matrix4::Rotation(randFloatToN(90), Vector3({ 0, 1, 0 }))
			* Matrix4::Rotation(randFloatToN(90), Vector3({ 0, 0, 1 }));
		Matrix4 scale = Matrix4::Scale(Vector3({ randFloatToN(maxScale), randFloatToN(maxScale), randFloatToN(maxScale) }));
		Matrix4 modelMat = translate * rotate * scale;

		// randomly choose from mesh in AsteroidTypes list
		int AsteroidType = randIntToN(asteroidTypes.size() - 1);
		r.AddRenderObject(new RenderObject(asteroidTypes[AsteroidType], modelMat));
	}
}

void generateOriginalStars(int n) {
	// Have n types of Stars, colour is randomised and shape is chosen at random from existing sets of points
	vector<vector<Vector3>> starShapes = {
		{ Vector3(0, 0, 0) },
		{ Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), Vector3(-1, 0, 0), Vector3(0, -1, 0), Vector3(0, 0, -1) },
		{ 
			Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), Vector3(-1, 0, 0), Vector3(0, -1, 0), Vector3(0, 0, -1),
			Vector3(0.5, 0.5, 0), Vector3(-0.5, 0.5, 0), Vector3(0.5, -0.5, 0), Vector3(-0.5, -0.5, 0),
			Vector3(0, 0.5, 0.5), Vector3(0, 0.5, -0.5), Vector3(0, -0.5, 0.5), Vector3(0, -0.5, -0.5),
			Vector3(0.5, 0, 0.5), Vector3(-0.5, 0, 0.5), Vector3(0.5, 0, -0.5), Vector3(-0.5, 0, -0.5)}
		};

	for (int i = 0; i < n; i++) {
		Vector4 colour = { Vector4(randFloatToN(1), randFloatToN(1), randFloatToN(1), 1) };
		OGLMesh* star = new OGLMesh();
		vector<Vector3> starShape = starShapes[randIntToN(starShapes.size() - 1)];
		vector<Vector4> starColours;
		for (int j = 0; j < starShape.size(); j++) {
			starColours.push_back(colour);
		}

		star->SetVertexPositions(starShape);
		star->SetVertexColours(starColours);
		star->SetPrimitiveType(GeometryPrimitive::Points);
		star->UploadToGPU();
		starTypes.push_back(star);
	}
}

void updateStarField(Renderer& r, int numberOfStarsToGenerate, float maxScale) {
	if (numOfStars > maxNumOfStars) return;
	// Could be further optimised if stars verticies were only generated in view
	// if the boundary has expanded set the new boundary to the expanded size, if not keep it the same as the old one
	maxStarFieldNew.x = (viewPosition.x + farPlane > maxStarFieldOld.x) ? viewPosition.x + farPlane : maxStarFieldOld.x;
	maxStarFieldNew.y = (viewPosition.y + farPlane > maxStarFieldOld.y) ? viewPosition.y + farPlane : maxStarFieldOld.y;
	maxStarFieldNew.z = (viewPosition.z + farPlane > maxStarFieldOld.z) ? viewPosition.z + farPlane : maxStarFieldOld.z;

	minStarFieldNew.x = (viewPosition.x - farPlane < minStarFieldOld.x) ? viewPosition.x - farPlane : minStarFieldOld.x;
	minStarFieldNew.y = (viewPosition.y - farPlane < minStarFieldOld.y) ? viewPosition.y - farPlane : minStarFieldOld.y;
	minStarFieldNew.z = (viewPosition.z - farPlane < minStarFieldOld.z) ? viewPosition.z - farPlane : minStarFieldOld.z;


	Vector3 maxStarFieldOldCopy(maxStarFieldOld.x, maxStarFieldOld.y, maxStarFieldOld.z);
	Vector3 minStarFieldOldCopy(minStarFieldOld.x, minStarFieldOld.y, minStarFieldOld.z);

	maxStarFieldOld.x = maxStarFieldNew.x;
	maxStarFieldOld.y = maxStarFieldNew.y;
	maxStarFieldOld.z = maxStarFieldNew.z;

	minStarFieldOld.x = minStarFieldNew.x;
	minStarFieldOld.y = minStarFieldNew.y;
	minStarFieldOld.z = minStarFieldNew.z;

	if (!starFieldInitialised) {
		for (int i = 0; i < numberOfStarsToGenerate; i++) {
			if (numOfStars++ > maxNumOfStars) return;

			int x = (randFloatToN(1) > 0.5) ? randFloatToN(maxStarFieldNew.x) : randFloatToN(minStarFieldNew.x);
			int y = (randFloatToN(1) > 0.5) ? randFloatToN(maxStarFieldNew.y) : randFloatToN(minStarFieldNew.y);
			int z = (randFloatToN(1) > 0.5) ? randFloatToN(maxStarFieldNew.z) : randFloatToN(minStarFieldNew.z);

			Matrix4 translate = Matrix4::Translation(Vector3(x, y, z));
			Matrix4 rotate =
				Matrix4::Rotation(randFloatToN(90), Vector3({ 1, 0, 0 }))
				* Matrix4::Rotation(randFloatToN(90), Vector3({ 0, 1, 0 }))
				* Matrix4::Rotation(randFloatToN(90), Vector3({ 0, 0, 1 }));
			Matrix4 scale = Matrix4::Scale(Vector3({ randFloatToN(maxScale), randFloatToN(maxScale), randFloatToN(maxScale) }));
			Matrix4 modelMat = translate * rotate * scale;

			// randomly choose from mesh in starTypes list
			int starType = randIntToN(starTypes.size() - 1);
			r.AddRenderObject(new RenderObject(starTypes[starType], modelMat));
		}

		starFieldInitialised = true;
		return;
	}
	
	for (int i = 0; i < numberOfStarsToGenerate; i++) {
		if (numOfStars++ > maxNumOfStars) return;
		
		// problem is maxStarFieldNew.x - maxStarFieldOld.x is 0 when expanding
		// if 0 that means that barrier didnt change
		// so it should be fine to choose any value in the one that didnt grow
		int x = (randFloatToN(1) > 0.5) ? randFloatToN(maxStarFieldNew.x) : randFloatToN(minStarFieldNew.x);
		int y = (randFloatToN(1) > 0.5) ? randFloatToN(maxStarFieldNew.y) : randFloatToN(minStarFieldNew.y);
		int z = (randFloatToN(1) > 0.5) ? randFloatToN(maxStarFieldNew.z) : randFloatToN(minStarFieldNew.z);

		if (abs(maxStarFieldNew.x - maxStarFieldOldCopy.x) > 0) {
			x = maxStarFieldOldCopy.x + randFloatToN(maxStarFieldNew.x - maxStarFieldOldCopy.x);
		}
		else if (abs(minStarFieldNew.x - minStarFieldOldCopy.x) > 0) {
			x = minStarFieldOldCopy.x - randFloatToN(minStarFieldNew.x - minStarFieldOldCopy.x);
		}
		else if (abs(maxStarFieldNew.y - maxStarFieldOldCopy.y) > 0) {
			y = maxStarFieldOldCopy.y + randFloatToN(maxStarFieldNew.y - maxStarFieldOldCopy.y);
		}
		else if (abs(minStarFieldNew.y - minStarFieldOldCopy.y) > 0) {
			y = minStarFieldOldCopy.y - randFloatToN(minStarFieldNew.y - minStarFieldOldCopy.y);
		}
		else if (abs(maxStarFieldNew.z - maxStarFieldOldCopy.z) > 0) {
			z = maxStarFieldOldCopy.z + randFloatToN(maxStarFieldNew.z - maxStarFieldOldCopy.z);
		}
		else if (abs(minStarFieldNew.z - minStarFieldOldCopy.z) > 0) {
			z = minStarFieldOldCopy.z - randFloatToN(minStarFieldNew.z - minStarFieldOldCopy.z);
		}
		else
		{
			// If none of the boundaries have changed, then no new stars need to be drawn
			return;
		}
		
		Matrix4 translate = Matrix4::Translation(Vector3(x, y, z));
		Matrix4 rotate = 
			Matrix4::Rotation(randFloatToN(90), Vector3({ 1, 0, 0 })) 
			* Matrix4::Rotation(randFloatToN(90), Vector3({ 0, 1, 0 })) 
			* Matrix4::Rotation(randFloatToN(90), Vector3({ 0, 0, 1 }));
		Matrix4 scale = Matrix4::Scale(Vector3({ randFloatToN(maxScale), randFloatToN(maxScale), randFloatToN(maxScale) }));
		Matrix4 modelMat = translate * rotate * scale;

		// randomly choose from mesh in starTypes list
		int starType = randIntToN(starTypes.size() - 1);
		r.AddRenderObject(new RenderObject(starTypes[starType], modelMat));
	}
}

void tutorial5(Renderer& renderer) {
	OGLMesh* tri = new OGLMesh("Cube.msh");
	tri->SetVertexColours({ Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1) });
	tri->SetPrimitiveType(GeometryPrimitive::Triangles);
	tri->UploadToGPU();

	for (int y = -400; y < 300; y += 100) {
		for (int x = -400; x < 300; x += 100) {
			float zAmount = rand() % 1000;
			float scaleAmount = ((rand() % 3) + 1);
			Matrix4 translate = Matrix4::Translation(Vector3((x + 1) * 10, (y + 1) * 10, -50 - zAmount));
			Matrix4 rotate = Matrix4::Rotation(((y * 5) + x) * 10, Vector3(0, 0, 1));
			Matrix4 scale = Matrix4::Scale(Vector3(scaleAmount, scaleAmount, scaleAmount));
			renderer.AddRenderObject(new RenderObject(tri, translate * rotate * scale));
		}
	}
}

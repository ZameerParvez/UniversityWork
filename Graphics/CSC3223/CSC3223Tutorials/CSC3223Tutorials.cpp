#include "../../Common/Camera.h"

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
#include "RasterisationMesh.h"
#include "Controls.h"

// including the hack that makes it run properly
#include "../../Common/HackySleep.h"

using namespace NCL;
using namespace CSC3223;

void tutorial1(Renderer& renderer);
void tutorial2(Renderer& renderer);
// tutorial3 has still needs colour interpolation for span triangles
void tutorial4(Renderer& renderer);
void tutorial5(Renderer& renderer);
void tutorial6(Renderer& renderer);
void tutorial7(Renderer& renderer);
void tutorial8(Renderer& renderer);
void tutorial9(Renderer& renderer);
void tutorial10(Renderer& renderer);
void tutorial11(Renderer& renderer);
void tutorial12(Renderer& renderer);
RenderObject* tutorial13(Renderer& renderer);
void tutorial13Update(RenderObject& litCube, float& cubeRotation, float time);

int main() {

	int screenX = 1080;
	int screenY = 720;
	Window* w = Window::CreateGameWindow("CSC3223 Tutorials!", screenX, screenY);

	if (!w->HasInitialised()) {
		return -1;
	}

	Controls* ctrls = Controls::getControls();
	Renderer* renderer = ctrls->initRenderer(*w);
	ctrls->setViewPos(Vector3{ 0, 0, 200 });

	// no longer work, I think it could have something to do with writing to the gpu too frequently
	// adding shaders seperately seems to make it work a little better
	tutorial5(*renderer);
	tutorial6(*renderer);
	tutorial8(*renderer);

	//tutorial1(*renderer); // not really useful, just drawing primitives
	//tutorial4(*renderer); // not really useful, just model matrix stuff

	tutorial2(*renderer);
	tutorial7(*renderer);
	tutorial9(*renderer);
	tutorial10(*renderer);
	tutorial11(*renderer);
	tutorial12(*renderer);
	RenderObject* litCube = tutorial13(*renderer);
	float cubeRotation = 0.0f;

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {

		HACKYSLEEPER

		float time = w->GetTimer()->GetTimeDelta();

		renderer->Update(time);
		tutorial13Update(*litCube, cubeRotation, time);
		//renderer->DrawString("OpenGL Rendering!", Vector2(10, 10));
		renderer->Render();

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

		ctrls->renderControls(*renderer);
		ctrls->cameraControls(*renderer);

		w->SetTitle(std::to_string(time) + ctrls->getInfo());
	}

	delete renderer;

	Window::DestroyGameWindow();
}

void tutorial1(Renderer& renderer) {
	OGLMesh* points = new OGLMesh();
	points->SetVertexPositions({ Vector3(50 , 50, 0), Vector3(30 , 10, 0), Vector3(200 , 70, 0) });
	points->SetPrimitiveType(GeometryPrimitive::Points);
	points->UploadToGPU();
	renderer.AddRenderObject(new RenderObject(points));

	OGLMesh* lines = new OGLMesh();
	lines->SetVertexPositions({ Vector3(600, 50, 0), Vector3(680,180,0), Vector3(680,50,0), Vector3(600,180,0) });
	lines->SetPrimitiveType(GeometryPrimitive::Lines);
	lines->UploadToGPU();
	renderer.AddRenderObject(new RenderObject(lines));

	OGLMesh* triangles = new OGLMesh();
	triangles->SetVertexPositions({ Vector3(50, 450, 0),	Vector3(250, 450, 0), Vector3(150, 350, 0) });
	triangles->SetPrimitiveType(GeometryPrimitive::Triangles);
	triangles->UploadToGPU();
	renderer.AddRenderObject(new RenderObject(triangles));

	OGLMesh* triangleStrip = new OGLMesh();
	triangleStrip->SetVertexPositions({ Vector3(600, 450, 0), Vector3(700, 450, 0), Vector3(650,350,0), Vector3(750,350,0), Vector3(600, 250, 0), Vector3(700, 250, 0) });
	triangleStrip->SetPrimitiveType(GeometryPrimitive::TriangleStrip);
	triangleStrip->UploadToGPU();
	renderer.AddRenderObject(new RenderObject(triangleStrip));

	OGLMesh* triangleFan = new OGLMesh();
	triangleFan->SetVertexPositions({ Vector3(400, 400, 0), Vector3(350, 350, 0), Vector3(450,350,0), Vector3(500, 400, 0), Vector3(450, 450, 0), Vector3(350, 450, 0), Vector3(300, 400, 0), Vector3(350, 350, 0) });
	triangleFan->SetPrimitiveType(GeometryPrimitive::TriangleFan);
	triangleFan->UploadToGPU();
	renderer.AddRenderObject(new RenderObject(triangleFan));
}

void tutorial2(Renderer& renderer) { // Normal OpenGL Primitives
	Matrix4 scale = Matrix4::Scale(Vector3(.1, .1, 1));
	Matrix4 translate = Matrix4::Translation(Vector3(1, 0, -50));
	Matrix4 modelMat = translate * scale;

	// Normal OpenGL mesh in top left
	OGLMesh* lines = new OGLMesh();
	lines->SetVertexPositions({ Vector3(150 , 50 , 0) ,	Vector3(180 ,80 ,0) , Vector3(100 ,200 ,0) , Vector3(150 ,150 ,0) });
	lines->SetVertexColours({ Vector4(1 ,0 ,0 ,1) ,	Vector4(0 ,1 ,0 ,1) , Vector4(0 ,0 ,1 ,1) , Vector4(1 ,0 ,1 ,1) });
	lines->SetPrimitiveType(GeometryPrimitive::Lines);
	lines->UploadToGPU();
	renderer.AddRenderObject(new RenderObject(lines, modelMat));

	// And another OpenGL mesh in the top right
	OGLMesh* tris = new OGLMesh();
	tris->SetVertexPositions({ Vector3(550 , 200 , 0) ,	Vector3(750 , 200 , 0) , Vector3(650 , 100 , 0) });
	tris->SetVertexColours({ Vector4(1 ,0 ,0 ,1) ,	Vector4(0 ,1 ,0 ,1) , Vector4(0 ,0 ,1 ,1) });
	tris->SetPrimitiveType(GeometryPrimitive::Triangles);
	tris->UploadToGPU();
	renderer.AddRenderObject(new RenderObject(tris, modelMat));

	OGLMesh* rasterLine = (OGLMesh*)RasterisationMesh::CreateLineFromPoints(
		{ Vector3(150 , 350 , 0) , Vector3(180 , 380 , 0) ,	Vector3(100 , 500 , 0) , Vector3(150 , 450 , 0) },
		{ Vector4(1 , 0 , 0 , 1) , Vector4(0 , 1 , 0 , 1) , Vector4(0 , 0 , 1 , 1) , Vector4(1 , 0 , 1 ,1) },
		LineRasteriser::BRESENHAM
	);
	renderer.AddRenderObject(new RenderObject(rasterLine, modelMat));

	OGLMesh* rasterTri = (OGLMesh*)RasterisationMesh::CreateTriangleFromPoints(
		{ Vector3(550 ,400 ,0) , Vector3(750 ,380 ,0) , Vector3(650 ,300 ,0) },
		{ Vector4(1, 0, 0, 1), Vector4(0, 1, 0, 1), Vector4(0, 0, 1, 1) },
		TriangleRasteriser::BARYCENTRIC
	);
	renderer.AddRenderObject(new RenderObject(rasterTri, modelMat));
}

void tutorial4(Renderer& renderer) {
	OGLMesh* triangle = new OGLMesh();
	triangle->SetVertexPositions({ Vector3(-1 , 0 , 0) , Vector3(1 , 0 , 0) , Vector3(0 , 1 , 0) });
	triangle->SetVertexColours({ Vector4(1 ,0 ,0 ,1) , Vector4(0 ,1 ,0 ,1) , Vector4(0 ,0 ,1 ,1) });
	triangle->SetPrimitiveType(GeometryPrimitive::Triangles);
	triangle->UploadToGPU();

	for (int y = 0; y < 5; ++y) {
		for (int x = 0; x < 5; ++x) {
			Matrix4 translate = Matrix4::Translation(Vector3((x + 1) * 130, (y + 1) * 120, 0));
			Matrix4 rotate = Matrix4::Rotation(((y * 5) + x) * 10, Vector3(0, 0, 1));
			Matrix4 scale = Matrix4::Scale(Vector3(50, 50, 50) / (1 + y));
			Matrix4 modelMat = translate * rotate * scale;
			renderer.AddRenderObject(new RenderObject(triangle, modelMat));
		}
	}
}

void tutorial5(Renderer& renderer) {
	OGLMesh* tri = new OGLMesh("Cube.msh");
	tri->SetPrimitiveType(GeometryPrimitive::Triangles);
	tri->UploadToGPU();

	for (int y = -4; y < 3; ++y) {
		for (int x = -4; x < 3; ++x) {
			OGLShader* newShader = new OGLShader("RasterisationVert.glsl", "RasterisationFrag.glsl");

			float zAmount = rand() % 30;
			float scaleAmount = ((rand() % 3) + 1);
			Matrix4 translate = Matrix4::Translation(Vector3((x + 1) * 10, (y + 1) * 10, -150 - zAmount));
			Matrix4 rotate = Matrix4::Rotation(((y * 5) + x) * 10, Vector3(0, 0, 1));
			Matrix4 scale = Matrix4::Scale(Vector3(scaleAmount, scaleAmount, scaleAmount));
			auto temp = new RenderObject(tri, translate * rotate * scale);
			temp->SetShader(newShader);
			renderer.AddRenderObject(temp);
		}
	}
}

void tutorial6(Renderer& renderer) {
	OGLShader* newShader1 = new OGLShader("VertexFocus.glsl", "RasterisationFrag.glsl");
	OGLShader* newShader2 = new OGLShader("VertexFocus.glsl", "RasterisationFrag.glsl");

	std::vector<Vector4> red = { Vector4(1 ,0 ,0 ,1) , Vector4(1 ,0 ,0 ,1) , Vector4(1 ,0 ,0 ,1) };
	std::vector<Vector4> blue = { Vector4(0 ,0 ,1 ,1) , Vector4(0 ,0 ,1 ,1) , Vector4(0 ,0 ,1 ,1) };
	std::vector<Vector3> verts = { Vector3(-1 , -1 ,0) , Vector3(1 , -1 ,0) , Vector3(0 ,1 ,0) };

	OGLMesh* redTri = new OGLMesh();
	redTri->SetVertexPositions(verts);
	redTri->SetVertexColours(red);
	redTri->SetPrimitiveType(GeometryPrimitive::Triangles);
	redTri->UploadToGPU();
	Matrix4 redModelMat = Matrix4::Translation(Vector3(-100, 0, -30)) * Matrix4::Scale(Vector3(5, 5, 5));
	auto temp1 = new RenderObject(redTri, redModelMat);
	temp1->SetShader(newShader1);
	renderer.AddRenderObject(temp1);

	OGLMesh* blueTri = new OGLMesh();
	blueTri->SetVertexPositions(verts);
	blueTri->SetVertexColours(blue);
	blueTri->SetPrimitiveType(GeometryPrimitive::Triangles);
	blueTri->UploadToGPU();
	Matrix4 blueModelMat = Matrix4::Translation(Vector3(-100, 0, -70)) * Matrix4::Scale(Vector3(15, 15, 15));
	auto temp2 = new RenderObject(blueTri, blueModelMat);
	temp2->SetShader(newShader2);
	renderer.AddRenderObject(temp2);
}

void tutorial7(Renderer& renderer) {
	std::vector<Vector3> verts = { Vector3(-1 , -1 ,0) , Vector3(1 , -1 ,0), Vector3(0 ,1 ,0) };
	std::vector<Vector4> white = { Vector4(1 ,1 ,1 ,1) , Vector4(1 ,1 ,1 ,1), Vector4(1 ,1 ,1 ,1) };
	std::vector<Vector2> texCoords = { Vector2(0 ,0) , Vector2(10, 0) , Vector2(5 ,10) };

	OGLMesh* tri = new OGLMesh();
	tri->SetVertexPositions(verts);
	tri->SetVertexColours(white);
	tri->SetVertexTextureCoords(texCoords);
	tri->SetPrimitiveType(GeometryPrimitive::Triangles);
	tri->UploadToGPU();

	Matrix4 modelMat = Matrix4::Translation(Vector3(100, 0, -70)) * Matrix4::Rotation(45.0f, Vector3(1, 0, 0)) * Matrix4::Scale(Vector3(15, 15, 15));
	RenderObject* renderObj = new RenderObject(tri, modelMat);
	TextureBase* newTex = OGLTexture::RGBATextureFromFilename("NCL.PNG");
	renderer.EnableMipMapFiltering(*(OGLTexture*) newTex);
	renderer.EnableTextureRepeating(*(OGLTexture*)newTex, false, false);
	renderObj->SetTexture(newTex);


	renderer.AddRenderObject(renderObj);
}

void tutorial8(Renderer& r) {
	OGLShader* newShader1 = new OGLShader("VertexFocus.glsl", "RasterisationFrag.glsl");
	OGLShader* newShader2 = new OGLShader("VertexFocus.glsl", "RasterisationFrag.glsl");

	std::vector<Vector4> red = { Vector4(1 ,0 ,0 ,0.25f) , Vector4(1 ,0 ,0 ,0.33f) , Vector4(1 ,0 ,0 ,0.5f) };
	std::vector<Vector3> rverts = { Vector3(-1 , -1 ,0) , Vector3(1 , -1 ,0) , Vector3(0 ,1 ,0) };
	
	std::vector<Vector4> blue = { Vector4(0 ,0 ,1 ,0.75f) , Vector4(0 ,0 ,1 ,0.75f) , Vector4(0 ,0 ,1 ,0.75f) };
	std::vector<Vector3> bverts = { Vector3(-1 , -1 ,0) , Vector3(1 , -1 ,0) , Vector3(0 ,1 ,0) };

	OGLMesh* redTri = new OGLMesh();
	redTri->SetVertexPositions(rverts);
	redTri->SetVertexColours(red);
	redTri->SetPrimitiveType(GeometryPrimitive::Triangles);
	redTri->UploadToGPU();

	Matrix4 redModelMat = Matrix4::Translation(Vector3{ -120, 0, -30 }) * Matrix4::Scale(Vector3(5, 5, 5));
	auto t1 = new RenderObject(redTri, redModelMat);
	t1->SetShader(newShader1);
	r.AddRenderObject(t1);
	
	OGLMesh* blueTri = new OGLMesh();
	blueTri->SetVertexPositions(bverts);
	blueTri->SetVertexColours(blue);
	blueTri->SetPrimitiveType(GeometryPrimitive::Triangles);
	blueTri->UploadToGPU();

	Matrix4 blueModelMat = Matrix4::Translation(Vector3{ -120, 0, -50 }) * Matrix4::Scale(Vector3(15, 15, 15));
	auto t2 = new RenderObject(blueTri, blueModelMat);
	t2->SetShader(newShader2);
	r.AddRenderObject(t2);
}

void tutorial9(Renderer& renderer) {
	std::vector<Vector4> rgb = { Vector4(1 ,0 ,0 ,1), Vector4(0 ,1 ,0 ,1), Vector4(0 ,0 ,1 ,1) };
	std::vector<Vector3> verts = { Vector3(-1 , -1 ,0), Vector3(1 , -1 ,0), Vector3(0 ,1 ,0) };

	OGLMesh* triMesh = new OGLMesh();
	triMesh->SetVertexPositions(verts);
	triMesh->SetVertexColours(rgb);
	triMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	triMesh->UploadToGPU();

	Matrix4 modelMat = Matrix4::Translation(Vector3(100, 0, -30)) * Matrix4::Scale(Vector3(5, 5, 5));
	RenderObject* object = new RenderObject(triMesh, modelMat);

	OGLShader* newShader = new OGLShader("VertexFocus.glsl", "RasterisationFrag.glsl");
	object->SetShader(newShader);

	renderer.AddRenderObject(object);
}

void tutorial10(Renderer& renderer) {
	std::vector<Vector4> rgb = { Vector4(1 ,0 ,0 ,1), Vector4(0 ,1 ,0 ,1), Vector4(0 ,0 ,1 ,1) };
	std::vector<Vector3> verts = { Vector3(-1 , -1 ,0), Vector3(1 , -1 ,0), Vector3(0 ,1 ,0) };

	OGLMesh* triMesh = new OGLMesh();
	triMesh->SetVertexPositions(verts);
	triMesh->SetVertexColours(rgb);
	triMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	triMesh->UploadToGPU();

	Matrix4 modelMat = Matrix4::Translation(Vector3(50, 0, -30)) * Matrix4::Scale(Vector3(5, 5, 5));
	RenderObject* object = new RenderObject(triMesh, modelMat);

	OGLShader* newShader = new OGLShader("VertexFocus.glsl", "ScreenColourFrag.glsl");
	object->SetShader(newShader);

	renderer.AddRenderObject(object);

	std::vector<Vector3> smileyVerts = {
		Vector3(1 ,1 ,0), Vector3(-1 , -1 ,0), Vector3(1 , -1 ,0),
		Vector3(1 ,1 ,0), Vector3(-1 ,1 ,0), Vector3(-1 , -1 ,0)
	};

	std::vector<Vector2> smileyTexCoords = {
		Vector2(1 ,1), Vector2(0 ,0), Vector2(1 ,0),
		Vector2(1 ,1), Vector2(0 ,1), Vector2(0 ,0)
	};

	OGLMesh* squareMesh = new OGLMesh();
	squareMesh->SetVertexPositions(smileyVerts);
	squareMesh->SetVertexTextureCoords(smileyTexCoords);

	squareMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	squareMesh->UploadToGPU();

	Matrix4 smileyModel = Matrix4::Translation(Vector3(70, 0, -30)) * Matrix4::Scale(Vector3(5, 5, 5));

	OGLShader* smileyShader = new OGLShader("RasterisationVert.glsl", "SmileyStaticFrag.glsl");
	RenderObject* smileyObject = new RenderObject(squareMesh, smileyModel);
	smileyObject->SetTexture(OGLTexture::RGBATextureFromFilename("smileyface.png"), 0);
	smileyObject->SetTexture(OGLTexture::RGBATextureFromFilename("static.png"), 1);
	smileyObject->SetShader(smileyShader);
	renderer.AddRenderObject(smileyObject);
}

void tutorial11(Renderer& renderer) {
	std::vector<Vector3> verts;


	for (int i = 0; i < 100; ++i) {
		float x = (float)(rand() % 100 - 50);
		float y = (float)(rand() % 100 - 50);
		float z = (float)(rand() % 100 - 50);
		verts.push_back(Vector3(x, y, z));
	}

	OGLMesh* pointSprites = new OGLMesh();
	pointSprites->SetVertexPositions(verts);
	pointSprites->SetPrimitiveType(GeometryPrimitive::Points);
	pointSprites->UploadToGPU();

	Matrix4 modelMat = Matrix4::Translation(Vector3(0, 0, -30));

	OGLShader* newShader = new OGLShader("RasterisationVert.glsl", "SimpleFrag.glsl", "PointGeom.glsl");
	RenderObject* object = new RenderObject(pointSprites, modelMat);
	object->SetTexture(OGLTexture::RGBATextureFromFilename("nyan.png"));
	object->SetShader(newShader);

	renderer.AddRenderObject(object);
}

void tutorial12(Renderer& renderer) {
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	std::vector<Vector3> verts = {
		Vector3(-1.0f,-1.0f,0.0f), Vector3(-1.0f,1.0f,0.0f),
		Vector3(1.0f,-1.0f,0.0f), Vector3(1.0f,1.0f,0.0f)
	};
	std::vector<Vector2> texCoords = {
		Vector2(0.0f,1.0f),Vector2(0.0f,0.0f),
		Vector2(1.0f,1.0f),Vector2(1.0f,0.0f)
	};

	OGLMesh* tessMesh = new OGLMesh();
	tessMesh->SetVertexPositions(verts);
	tessMesh->SetVertexTextureCoords(texCoords);
	tessMesh->SetPrimitiveType(GeometryPrimitive::Patches);
	tessMesh->UploadToGPU();

	TextureBase* noiseTex = OGLTexture::RGBATextureFromFilename("perlin_noise.PNG");
	OGLShader* tessShader = new OGLShader("TessVert.glsl", "DisplaceFrag.glsl", "", "DisplaceTCS.glsl", "DisplaceTES.glsl");

	Matrix4 modelMat = Matrix4::Rotation(90, Vector3(1, 0, 0)) * Matrix4::Scale(Vector3(20, 20, 20));
	RenderObject* tessObject = new RenderObject(tessMesh);
	tessObject->SetTransform(modelMat);
	tessObject->SetTexture(noiseTex, 2);
	tessObject->SetShader(tessShader);

	renderer.AddRenderObject(tessObject);
}


RenderObject* tutorial13(Renderer& renderer) {
	OGLShader* lightingShader = new OGLShader("LightingVertex.glsl", "LightingFragment.glsl");

	//Simple demo case, load a cube with normals precalculated!
	OGLMesh* cubeMesh = new OGLMesh("cube.msh");
	cubeMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	cubeMesh->UploadToGPU();
	RenderObject* cubeObject = new RenderObject(cubeMesh);
	cubeObject->SetShader(lightingShader);
	TextureBase* tex = OGLTexture::RGBATextureFromFilename("doge.PNG");
	cubeObject->SetTexture(tex);
	renderer.AddRenderObject(cubeObject);
	renderer.SetLightProperties(Vector3(0, 25, 0), Vector3(1.0f, 0.3f, 0.2f), 200.0f);

	OGLMesh* triMesh = new OGLMesh();
	std::vector<Vector3> verts = {
			Vector3(-1, -1, 0), Vector3(1, -1, 0), Vector3(0, 1, 0)
	};
	std::vector<Vector3> normals;
	//this is assuming its just triangles....
	for (int i = 0; i < verts.size(); i += 3) {
		Vector3& a = verts[i];
		Vector3& b = verts[i + 1];
		Vector3& c = verts[i + 2];
		Vector3 normal = Vector3::Cross(b - a, c - a);
		normal.Normalise();

		normals.emplace_back(normal);
		normals.emplace_back(normal);
		normals.emplace_back(normal);
	}

	triMesh->SetVertexPositions(verts);
	triMesh->SetVertexNormals(normals);

	triMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	triMesh->UploadToGPU();

	Matrix4 triMat = Matrix4::Translation(Vector3(20, 0, -70)) *
		Matrix4::Rotation(-10.0f, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-15.0f, Vector3(0, 1, 0)) *
		Matrix4::Scale(Vector3(15, 15, 15));

	RenderObject* triObject = new RenderObject(triMesh, triMat);
	triObject->SetShader(lightingShader);

	renderer.AddRenderObject(triObject);

	return cubeObject;
}

void tutorial13Update(RenderObject& litCube, float& cubeRotation, float time) {
	cubeRotation += time * 2;// *0.1f;
	litCube.SetTransform(
		Matrix4::Translation(Vector3(-25, 0, -70)) *
		Matrix4::Rotation(cubeRotation, Vector3(1, 1, 1)) *
		Matrix4::Scale(Vector3(6, 6, 6))
	);
}

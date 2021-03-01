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

void shaderControls(Renderer& r);

Vector2 tessInnerLevel = Vector2(32, 32);
Vector4 tessOuterLevel = Vector4(32, 32, 32, 32);

float sizeFactor = 10.0f;
float duration = 10.0f;

RenderObject* cube;
string mainTexture = "doge.png";
string secondaryTexture = "static.png";

void simpleShader(RenderObject& o);
void shrinkAndVanish(RenderObject& o);
void destroyTexture(RenderObject& o);
void fadeTexture(RenderObject& o);
void diceCube(RenderObject& o);
void brokenCube(RenderObject& o);
void shatteredCube(RenderObject& o);
void LaserOnCube(RenderObject& o);

RenderObject* makeCube(Renderer& renderer);
void updateCube(RenderObject& cube, float& rotation, float time);

int main() {
	Window* w = Window::CreateGameWindow("CSC3223 Coursework 2!", 1080, 720);

	if (!w->HasInitialised()) {
		return -1;
	}

	Controls* ctrls = Controls::getControls();
	Renderer* renderer = ctrls->initRenderer(*w);
	ctrls->setTessInnerLevel(tessInnerLevel);
	ctrls->setTessOuterLevel(tessOuterLevel);

	cube = makeCube(*renderer);
	cube->SetTexture(OGLTexture::RGBATextureFromFilename(mainTexture), 0);
	cube->SetTexture(OGLTexture::RGBATextureFromFilename(secondaryTexture), 1);

	float rotation = 0.0f;
	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		
		HACKYSLEEPER

		float time = w->GetTimer()->GetTimeDelta();
		renderer->Update(time);
		renderer->incrementTime(time);

		updateCube(*cube, rotation, time);

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

		shaderControls(*renderer);
		ctrls->renderControls(*renderer);
		ctrls->cameraControls(*renderer);

		w->SetTitle(std::to_string(time) + ctrls->getInfo());
	}

	delete renderer;

	Window::DestroyGameWindow();
}

void shaderControls(Renderer& r) {
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_NUMPAD1)) {
		r.setDuration(duration);
		r.resetTime();
		shrinkAndVanish(*cube);
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_NUMPAD2)) {
		r.setDuration(duration);
		r.resetTime();
		destroyTexture(*cube);
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_NUMPAD3)) {
		r.setDuration(duration);
		r.resetTime();
		fadeTexture(*cube);
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_NUMPAD4)) {
		r.setDuration(duration);
		r.setTessInnerLevel(50, 50);
		r.setTessOuterLevel(100, 100, 100, 100);
		r.resetTime();
		brokenCube(*cube);
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_NUMPAD5)) {
		r.setDuration(duration);
		r.resetTime();
		r.setTessInnerLevel(10, 5);
		r.setTessOuterLevel(10, 3, 5, 7);
		shatteredCube(*cube);
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_NUMPAD6)) {
		r.SetLightProperties(Vector3(0, 25, 0), Vector3(1.0f, 0.3f, 0.2f), 200.0f);
		r.setDuration(duration);
		r.resetTime();
		LaserOnCube(*cube);
	}
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_NUMPAD7)) {
		r.setDuration(duration);
		r.resetTime();
		diceCube(*cube);
	}


	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_NUMPAD0)) {
		sizeFactor = 1.0f;
		duration = 10.0f;
		r.setSizeFactor(sizeFactor);
		r.setDuration(duration);
		r.resetTime();
		simpleShader(*cube);
	}
}

RenderObject* makeCube(Renderer& renderer) {
	OGLMesh* m = new OGLMesh("cube.msh");
	m->SetPrimitiveType(GeometryPrimitive::Triangles);
	m->UploadToGPU();

	RenderObject* object = new RenderObject(m);
	renderer.AddRenderObject(object);

	return object;
}

void updateCube(RenderObject& cube, float& rotation, float time) {
	rotation += time * 30;
	Matrix4 modelMat = Matrix4::Translation(Vector3(0, 0, -10));
	modelMat = modelMat * Matrix4::Rotation(rotation, Vector3(1, 1, 1));
	cube.SetTransform(modelMat);
}

// will cause memory leaks with switching shaders

void simpleShader(RenderObject& o) {
	o.GetMesh()->SetPrimitiveType(GeometryPrimitive::Triangles);
	OGLShader* shader = new OGLShader("RasterisationVert.glsl", "RasterisationFrag.glsl");
	o.SetShader(shader);
}

void shrinkAndVanish(RenderObject& o) {
	o.GetMesh()->SetPrimitiveType(GeometryPrimitive::Triangles);
	OGLShader* shader = new OGLShader("ShrinkAndVanishVert.glsl", "RasterisationFrag.glsl");
	o.SetShader(shader);
}

void destroyTexture(RenderObject& o) {
	o.GetMesh()->SetPrimitiveType(GeometryPrimitive::Triangles);
	OGLShader* shader = new OGLShader("RasterisationVert.glsl", "DestroyTextureFrag.glsl");
	o.SetShader(shader);
}

void fadeTexture(RenderObject& o) {
	o.GetMesh()->SetPrimitiveType(GeometryPrimitive::Triangles);
	OGLShader* shader = new OGLShader("RasterisationVert.glsl", "FadeTextureFrag.glsl");
	o.SetShader(shader);
}

void diceCube(RenderObject& o) {
	o.GetMesh()->SetPrimitiveType(GeometryPrimitive::Triangles);
	OGLShader* shader = new OGLShader("RasterisationVert.glsl", "RasterisationFrag.glsl", "DiceCubeGeom.glsl");
	o.SetShader(shader);
}

void brokenCube(RenderObject& o) {
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	o.GetMesh()->SetPrimitiveType(GeometryPrimitive::Patches);
	OGLShader* shader = new OGLShader("BrokenCubeVert.glsl", "BrokenCubeFrag.glsl", "BrokenCubeGeom.glsl", "BrokenCubeTCS.glsl", "BrokenCubeTES.glsl");
	o.SetShader(shader);
}

void shatteredCube(RenderObject& o) {
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	o.GetMesh()->SetPrimitiveType(GeometryPrimitive::Patches);
	OGLShader* shader = new OGLShader("ShatteredCubeVert.glsl", "ShatteredCubeFrag.glsl", "ShatteredCubeGeom.glsl", "ShatteredCubeTCS.glsl", "ShatteredCubeTES.glsl");
	o.SetShader(shader);
}

void LaserOnCube(RenderObject& o) {
	o.GetMesh()->SetPrimitiveType(GeometryPrimitive::Triangles);
	OGLShader* lightingShader = new OGLShader("LightingVertex.glsl", "LightingFragment.glsl");

	OGLShader* shader = new OGLShader("LaserVert.glsl", "LaserFrag.glsl");// , "LaserGeom.glsl");
	o.SetShader(lightingShader);
}

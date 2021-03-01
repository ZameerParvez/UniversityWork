#pragma once
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

#include <sstream>

namespace NCL {
	namespace CSC3223 {
		enum class CameraType {
			FPS,
			NOTQUITEFREE
		};

		class Controls {
		public:
			static Controls* getControls() {
				static Controls c;
				return &c;
			}

			Renderer* initRenderer(Window& w) {
				Renderer* renderer = new Renderer(w);

				aspect = w.GetScreenAspect();
				proj = Matrix4::Perspective(nearPlane, farPlane, aspect, fov);
				renderer->SetProjectionMatrix(proj);
				renderer->EnableDepthBuffer(enableDepthBuffer);
				renderer->EnableAlphaBlend(enableAlphaBlend);
				renderer->ChangeBlendMode(-1);
				renderer->setSizeFactor(sizeFactor);
				renderer->ChangeDepthTestMode(-1);
				renderer->setTessInnerLevel(tessInnerLevel[0], tessInnerLevel[1]);
				renderer->setTessOuterLevel(tessOuterLevel[0], tessOuterLevel[1], tessOuterLevel[2], tessOuterLevel[3]);

				return renderer;
			}

			void renderControls(Renderer& r);
			void cameraControls(Renderer& r);

			Vector3 getViewPos() {
				return viewPosition;
			}
			Vector3 getViewRotation() {
				return viewRotation;
			}
			Matrix4 getRotationMatrix() {
				return allRotations;
			}
			void setViewPos(Vector3 viewPos) {
				this->viewPosition = viewPos;
			}
			void setViewRotation(Vector3 viewRotation) {
				this->viewRotation = viewRotation;
			}
			float getSpeed() {
				return this->speed;
			}

			void setTessInnerLevel(Vector2 t) {
				tessInnerLevel = t;
			}
			void setTessOuterLevel(Vector4 t) {
				tessOuterLevel = t;
			}

			void setCameraType(CameraType ctype) {
				this->cType = ctype;
			}

			std::string getInfo() {
				std::ostringstream os;
				os << time
					<< "{" << viewPosition.x << ", " << viewPosition.y << ", " << viewPosition.z << "} "
					<< "{" << viewRotation.x << ", " << viewRotation.y << ", " << viewRotation.z << "} "
					<< "speed: " << speed;
				return os.str();
			}

		private:
			Matrix4 proj;
			Vector3 viewPosition = Vector3(0, 0, 0);
			Vector3 viewRotation = Vector3(0, 0, 0);
			Matrix4 allRotations;
			Matrix4 invRotations;

			CameraType cType = CameraType::FPS;

			bool cameraControlIsDisabled = false;
			bool rotationToggle = true;
			bool drawAsWireFrame = false;
			bool enableDepthBuffer = true;
			bool enableAlphaBlend = true;
			int alphaBlendState = -1;
			float sizeFactor = 50.0f;
			Vector2 tessInnerLevel = Vector2(32, 32);
			Vector4 tessOuterLevel = Vector4(32, 32, 32, 32);

			float mouseSensitivity = 5;
			float speed = 1;
			float aspect = 0;
			float fov = 45.0f;
			float nearPlane = 1.0f;
			float farPlane = 1000.0f;

			Controls() {}
		};

		void Controls::renderControls(Renderer& r) {
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F)) {
				drawAsWireFrame = !drawAsWireFrame;
				r.EnableDrawAsWireFrame(drawAsWireFrame);
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F1)) {
				enableDepthBuffer = !enableDepthBuffer;
				r.EnableDepthBuffer(enableDepthBuffer);
			}
			if (enableDepthBuffer) {
				if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F2)) {
					r.ChangeDepthTestMode(-1);
				}
				if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F3)) {
					r.ChangeDepthTestMode(0);
				}
				if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F4)) {
					r.ChangeDepthTestMode(2);
				}
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F5)) {
				enableAlphaBlend = !enableAlphaBlend;
				r.EnableAlphaBlend(enableAlphaBlend);
			}
			if (enableAlphaBlend) {
				// This sorting should happen in the direction the camera is facing
				r.sortRenderObjects();
				if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F6)) {
					r.ChangeBlendMode(-1);
				}
				if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F7)) {
					r.ChangeBlendMode(0);
				}
				if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F8)) {
					r.ChangeBlendMode(1);
				}
			}

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_PERIOD)) {
				sizeFactor += 1.0f;
				r.setSizeFactor(sizeFactor);
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_COMMA)) {
				sizeFactor -= 1.0f;
				r.setSizeFactor(sizeFactor);
			}

			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_F11)) {
				r.WriteDepthBuffer("Depth.png");
			}

			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_1)) {
				drawAsWireFrame = false;
				enableDepthBuffer = true;
				enableAlphaBlend = true;
				alphaBlendState = -1;
				r.EnableDepthBuffer(enableDepthBuffer);
				r.EnableAlphaBlend(enableAlphaBlend);
				r.ChangeDepthTestMode(-1);
				r.ChangeBlendMode(-1);
			}
		}

		// this is not a free camera
		// it is like an fps camera, which makes it fairly intuitive to control
		void Controls::cameraControls(Renderer& r) {
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_R)) {
				rotationToggle = !rotationToggle;
			}
			if (rotationToggle) {
				Vector2 mouse = Window::GetMouse()->GetRelativePosition();
				viewRotation += Vector3(0, -1, 0) * mouseSensitivity * mouse[0];
				viewRotation += Vector3(-1, 0, 0) * mouseSensitivity * mouse[1];

			}

			// behaviour of roll is incorrect
			//if (Window::GetKeyboard()->KeyDown(KEYBOARD_Q)) {
			//	viewRotation += Vector3(0, 0, -1);
			//}
			//if (Window::GetKeyboard()->KeyDown(KEYBOARD_E)) {
			//	viewRotation -= Vector3(0, 0, -1);
			//}

			// bounds yaw
			viewRotation.x = max(-90, viewRotation.x);
			viewRotation.x = min(90, viewRotation.x);

			Matrix4 rotationZ = Matrix4::Rotation(viewRotation.z, Vector3({ 0, 0, -1 }));
			Matrix4 rotationY = Matrix4::Rotation(viewRotation.y, Vector3({ 0, 1, 0 }));
			Matrix4 rotationX = Matrix4::Rotation(viewRotation.x, Vector3({ 1, 0, 0 }));

			Matrix4 invRotationZ = Matrix4::Rotation(viewRotation.z, Vector3({ 0, 0, 1 }));
			Matrix4 invRotationY = Matrix4::Rotation(viewRotation.y, Vector3({ 0, -1, 0 }));
			Matrix4 invRotationX = Matrix4::Rotation(viewRotation.x, Vector3({ -1, 0, 0 }));

			// currently pitch and yaw work as expected, roll happens in world z from the cameras position

			allRotations = rotationZ * rotationY * rotationX;
			invRotations = invRotationX * invRotationY * invRotationZ;

			// These movements do happen in the correct directions 
			Vector3 stepForwards = (allRotations * Vector3({ 0, 0, -1 }));
			Vector3 stepUp = (allRotations * Vector3({ 0, 1, 0 }));
			Vector3 stepRight = (allRotations * Vector3({ 1, 0, 0 }));

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
				viewPosition += stepRight * speed;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
				viewPosition -= stepRight * speed;
			}

			if (cType == CameraType::FPS) {
				// normal fps behaviour up and down are always the same, not relative to where you are facing
				if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
					viewPosition += Vector3({ 0, 1, 0 }) * speed;
				}
				if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
					viewPosition -= Vector3({ 0, 1, 0 }) * speed;
				}
				// removes the y component of the forwards movement, to make normal fps controls
				if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
					viewPosition += stepForwards * Vector3({ 1, 0, 1 }) * speed;
				}
				if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
					viewPosition -= stepForwards * Vector3({ 1, 0, 1 }) * speed;
				}
			}
			else if (cType == CameraType::NOTQUITEFREE) {
				//slighlty odd camera behaviour, closer to a free camera, but becomes incorrect, not as simple to control
				if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
					viewPosition += stepUp * speed;
				}
				if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
					viewPosition -= stepUp * speed;
				}
				if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
					viewPosition += stepForwards * speed;
				}
				if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
					viewPosition -= stepForwards * speed;
				}
			}

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_ADD)) {
				fov += 1;
				proj = Matrix4::Perspective(nearPlane, farPlane, aspect, fov);
				r.SetProjectionMatrix(proj);
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_SUBTRACT)) {
				fov -= 1;
				proj = Matrix4::Perspective(nearPlane, farPlane, aspect, fov);
				r.SetProjectionMatrix(proj);
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_PLUS)) {
				speed += 0.01;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_MINUS)) {
				speed = (speed > 0) ? speed - 0.01 : 0;
			}
			if (Window::GetKeyboard()->KeyPressed(KEYBOARD_0)) {
				viewPosition.x = 0.0f;
				viewPosition.y = 0.0f;
				viewPosition.z = 0.0f;

				viewRotation.x = 0.0f;
				viewRotation.y = 0.0f;
				viewRotation.z = 0.0f;

				fov = 45;
				proj = Matrix4::Perspective(nearPlane, farPlane, aspect, fov);
				r.SetProjectionMatrix(proj);
			}
			// change camera type
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_2)) {
				cType = (cType == CameraType::FPS) ? CameraType::NOTQUITEFREE : CameraType::FPS;
			}

			r.SetViewMatrix(invRotations * Matrix4::Translation(-viewPosition));
		}
	}
}
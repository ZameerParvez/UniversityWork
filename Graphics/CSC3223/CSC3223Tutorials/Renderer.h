#pragma once
#include "../../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"

#include "RenderObject.h"
#include "../../Common/Vector2.h"

namespace NCL {
	namespace CSC3223 {

		struct Light {
			Vector3 position;
			Vector3 colour;
			float lightRadius;
		};

		class Renderer : public OGLRenderer
		{
		public:
			Renderer(Window& w);
			~Renderer();
			

			void AddRenderObject(RenderObject* ro) {
				renderObjects.emplace_back(ro);
			}

			void DeleteAllRenderObjects() {
				for (const RenderObject* object : renderObjects) {
					delete object;
				}
				renderObjects.clear();
			}
			
			void SetProjectionMatrix(const Matrix4&m) {
				projMatrix = m;
			}

			void SetViewMatrix(const Matrix4&m) {
				viewMatrix = m;
			}

			void EnableDepthBuffer(bool state) {
				if (state) {
					glEnable(GL_DEPTH_TEST);
				}
				else {
					glDisable(GL_DEPTH_TEST);
				}
			}

			void ChangeDepthTestMode(int i) {
				switch (i)
				{
				case 0:
					glDepthFunc(GL_ALWAYS);
					break;
				case 1:
					glDepthFunc(GL_EQUAL);
					break;
				case 2:
					glDepthFunc(GL_GREATER);
					break;
				default:
					glDepthFunc(GL_LEQUAL);
					break;
				}
			}

			void EnableDrawAsWireFrame(bool state) {
				drawAsWireframe = state;
			}

			void EnableAlphaBlend(bool state) {
				if (state) {
					glEnable(GL_BLEND);
				}
				else {
					glDisable(GL_BLEND);
				}
			}

			void ChangeBlendMode(int i) {
				switch (i)
				{
				case 0:
					glBlendFunc(GL_ONE, GL_ONE);
					break;
				case 1:
					glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_DST_COLOR);
					break;
				case 2:
					glBlendFunc(GL_ONE_MINUS_CONSTANT_ALPHA, GL_ONE_MINUS_DST_COLOR);
					break;
				default:
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					break;
				}
			}

			void incrementTime(float dx) {
				time += dx;
			}

			void resetTime() {
				time = 0.0f;
			}

			void setDuration(float x) {
				if (x > 0) duration = x;
			}

			void setSizeFactor(float x) {
				if (x > 0) sizeFactor = x;
			}

			void setTessInnerLevel(float x1, float x2 = 0) {
				tessInnerLevel[0] = x1;
				tessInnerLevel[1] = x2;
			}

			void setTessOuterLevel(float x1, float x2, float x3 = 0, float x4 = 0) {
				tessOuterLevel[0] = x1;
				tessOuterLevel[1] = x2;
				tessOuterLevel[2] = x3;
				tessOuterLevel[3] = x4;
			}

			void sortRenderObjects() {
				std::sort(renderObjects.begin(), renderObjects.end());
			}

			void WriteDepthBuffer(const string& filepath) const;
			void EnableBilinearFiltering(OGLTexture& t);
			void EnableMipMapFiltering(OGLTexture& t);
			void EnableTextureRepeating(OGLTexture& t, bool uRepeat, bool vRepeat);

			void SetLightProperties(Vector3 position, Vector3 colour, float lightRadius) {
				activeLight.position = position;
				activeLight.colour = colour;
				activeLight.lightRadius = lightRadius;
			}

		protected:
			void RenderNode(RenderObject* root);
	
			void OnWindowResize(int w, int h)	override;

			void RenderFrame() override;
			void ApplyLightToShader(const Light& l, const OGLShader* s);

			OGLShader*		defaultShader;

			Matrix4		projMatrix;
			Matrix4		viewMatrix;

			vector<RenderObject*> renderObjects;
			GameTimer frameTimer;

			Light activeLight;
			bool drawAsWireframe = false;
			float time = 0.0f;
			float duration = 10.0f;
			float sizeFactor = 10.0f;
			Vector2 tessInnerLevel = Vector2(10, 10);
			Vector4 tessOuterLevel = Vector4(10, 10, 10, 10);
		};
	}
}


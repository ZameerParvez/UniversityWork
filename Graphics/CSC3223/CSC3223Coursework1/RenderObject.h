#pragma once
#include "../../Common/Matrix4.h"
#include "../../Common/TextureBase.h"
#include "../../Common/ShaderBase.h"

#include <vector>
namespace NCL {
	using namespace NCL::Rendering;
	class MeshGeometry;
	namespace CSC3223 {
		using namespace Maths;

		const int NUMBER_OF_TEXTURES = 3;
		
		class RenderObject
		{
		public:

			RenderObject(MeshGeometry* mesh, Matrix4 m = Matrix4(), ShaderBase* shader = nullptr );
			~RenderObject();

			MeshGeometry* GetMesh() const {
				return mesh;
			}

			void  SetTransform(Matrix4 mat) {
				transform = mat;
			}

			Matrix4 GetTransform() const {
				return transform;
			}

			void SetTexture(TextureBase* t, int i = 0) {
				if (i >= NUMBER_OF_TEXTURES) i = 0;
				textures[i] = t;
			}

			TextureBase* GetTexture(int i = 0) const {
				if (i >= NUMBER_OF_TEXTURES) i = 0;
				return textures[i];
			}

			void SetShader(ShaderBase* s) {
				shader = s;
			}

			ShaderBase* GetShader() const {
				return shader;
			}

		protected:
			MeshGeometry*	mesh;
			TextureBase*	textures[NUMBER_OF_TEXTURES];
			ShaderBase*		shader;
			Matrix4			transform;
		};
	}
}


#pragma once

#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Common/Maths.h"
#include "../../Common/Vector4.h"
#include "../../Common/Vector3.h"


namespace NCL {
	namespace CSC3223 {

		enum class LineRasteriser {
			LINEQ,
			BRESENHAM
		};

		enum class TriangleRasteriser {
			LINEQ,
			BARYCENTRIC,
			SPAN
		};

		class RasterisationMesh : public Rendering::OGLMesh {
		public:

			static RasterisationMesh* CreateTriangleFromPoints(const std::vector<Vector3>& inVerts, const std::vector<Vector4>& inColours, TriangleRasteriser type = TriangleRasteriser::BARYCENTRIC);

			static RasterisationMesh* CreateLineFromPoints(const std::vector<Vector3>& inVerts, const std::vector<Vector4>& inColours, LineRasteriser type = LineRasteriser::BRESENHAM);

		protected:
			void RasterLineEquationTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector4& c0, const Vector4& c1, const Vector4& c2);
			void RasterSpanTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector4& c0, const Vector4& c1, const Vector4& c2);
			void RasterBarycentricTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector4& c0, const Vector4& c1, const Vector4& c2);
			void RasterBasicLine(const Vector3& p0, const Vector3& p1, const Vector4& c0, const Vector4& c1);
			void RasterBresnhamLine(const Vector3& p0, const Vector3& p1, const Vector4& c0, const Vector4& c1);
		};
	}
}
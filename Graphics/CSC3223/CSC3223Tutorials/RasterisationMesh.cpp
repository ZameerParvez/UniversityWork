#include "RasterisationMesh.h"

using namespace NCL;
using namespace CSC3223;

// places the lines and triangles on the z plane with z coord given by the first vertex
// it should be possible to get some f(x,y)=z
RasterisationMesh* RasterisationMesh::CreateLineFromPoints(const std::vector<Vector3>& inVerts, const std::vector<Vector4>& inColours, LineRasteriser type) {
	RasterisationMesh* line = new RasterisationMesh();

	switch (type) {
	case NCL::CSC3223::LineRasteriser::LINEQ:
		for (int i = 0; i < inVerts.size() - 1; i += 2) {
			line->RasterBasicLine(inVerts[i], inVerts[i + 1], inColours[i], inColours[i + 1]);
		}
		break;
	case NCL::CSC3223::LineRasteriser::BRESENHAM:
		for (int i = 0; i < inVerts.size() - 1; i += 2) {
			line->RasterBresnhamLine(inVerts[i], inVerts[i + 1], inColours[i], inColours[i + 1]);
		}
		break;
	}

	line->SetPrimitiveType(GeometryPrimitive::Points);
	line->UploadToGPU();

	return line;
}

RasterisationMesh* RasterisationMesh::CreateTriangleFromPoints(const std::vector<Vector3>& inVerts, const std::vector<Vector4>& inColours, TriangleRasteriser type) {
	RasterisationMesh* tri = new RasterisationMesh();
	switch (type) {
	case NCL::CSC3223::TriangleRasteriser::LINEQ:
		for (size_t i = 0; i < inVerts.size() - 2; i += 3) {
			tri->RasterLineEquationTriangle(inVerts[i], inVerts[i + 1], inVerts[i + 2], inColours[i], inColours[i + 1], inColours[i + 2]);
		}
		break;
	case NCL::CSC3223::TriangleRasteriser::BARYCENTRIC:
		for (size_t i = 0; i < inVerts.size() - 2; i += 3) {
			tri->RasterBarycentricTriangle(inVerts[i], inVerts[i + 1], inVerts[i + 2], inColours[i], inColours[i + 1], inColours[i + 2]);
		}
		break;
	case NCL::CSC3223::TriangleRasteriser::SPAN:
		for (size_t i = 0; i < inVerts.size() - 2; i += 3) {
			tri->RasterSpanTriangle(inVerts[i], inVerts[i + 1], inVerts[i + 2], inColours[i], inColours[i + 1], inColours[i + 2]);
		}
		break;
	}

	tri->SetPrimitiveType(GeometryPrimitive::Points);
	tri->UploadToGPU();
	return tri;
}

void RasterisationMesh::RasterBasicLine(const Vector3& v0, const Vector3& v1, const Vector4& c0, const Vector4& c1) {
	float lineLength = (v0 - v1).Length();
	int numPixels = (int)lineLength;
	float lerpAmount = 1.0f / (numPixels - 1);
	float t = 0.0f;
	for (int i = 0; i < numPixels; ++i) {
		positions.emplace_back(Maths::Lerp<Vector3>(v0, v1, t));
		colours.emplace_back(Maths::Lerp < Vector4 >(c0, c1, t));
		t += lerpAmount;
	}
}

void RasterisationMesh::RasterBresnhamLine(const Vector3& v0, const Vector3& v1, const Vector4& c0, const Vector4& c1) {
	Vector3 dir = v1 - v0;
	Vector3 currentPos = v0;
	int scanRange = 0;
	float slope = 0.0f;
	float* periodAxis = nullptr;
	float* scanAxis = nullptr;
	int scanDelta = 0;
	int targetDelta = 0;

	if (abs(dir.y) > abs(dir.x)) { // steep !
		slope = (dir.x / dir.y);
		scanRange = (int)abs(dir.y);
		periodAxis = &currentPos.x;
		scanAxis = &currentPos.y;
		scanDelta = (dir.y < 0.0f) ? -1 : 1;
		targetDelta = (dir.x < 0.0f) ? -1 : 1;
	}
	else {
		slope = (dir.y / dir.x);
		scanRange = (int)abs(dir.x);
		periodAxis = &currentPos.y;
		scanAxis = &currentPos.x;
		scanDelta = (dir.x < 0.0f) ? -1 : 1;
		targetDelta = (dir.y < 0.0f) ? -1 : 1;
	}

	Vector4 colourDiff = c1 - c0; // how much the colour changes by
	Vector4 colourDelta = colourDiff / scanRange; // change per pixe
	Vector4 currentColour = c0; // starting colour to move from

	float absSlope = abs(slope);
	float error = 0.0f;
	for (int i = 0; i < scanRange; ++i) {
		positions.emplace_back(currentPos);
		colours.emplace_back(currentColour);
		currentColour += colourDelta;
		error += absSlope;
		if (error > 0.5f) {
			error -= 1.0f;
			(*periodAxis) += targetDelta;
		}
		(*scanAxis) += scanDelta;
	}
}

void RasterisationMesh::RasterLineEquationTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector4& c0, const Vector4& c1, const Vector4& c2) {
	Vector2 topLeft, bottomRight;
	Maths::ScreenBoxOfTri(p0, p1, p2, topLeft, bottomRight);

	Vector3 linev0v1 = Vector3::Cross(Vector3(p0.x, p0.y, 1), Vector3(p1.x, p1.y, 1));
	Vector3 linev1v2 = Vector3::Cross(Vector3(p1.x, p1.y, 1), Vector3(p2.x, p2.y, 1));
	Vector3 linev2v0 = Vector3::Cross(Vector3(p2.x, p2.y, 1), Vector3(p0.x, p0.y, 1));

	for (int y = (int)topLeft.y; y < bottomRight.y; ++y) {
		for (int x = (int)topLeft.x; x < bottomRight.x; ++x) {
			Vector3 screenPos((float)x, (float)y, 1.0f);
			float line01 = Vector3::Dot(linev0v1, screenPos);
			float line12 = Vector3::Dot(linev1v2, screenPos);
			float line20 = Vector3::Dot(linev2v0, screenPos);
			if (line01 <= 0 && line12 <= 0 && line20 <= 0) {
				positions.emplace_back(screenPos);
				float lineSum = line01 + line12 + line20; // new lines here !
				line01 /= lineSum;
				line12 /= lineSum;
				line20 /= lineSum;
				Vector4 colour = (c0 * line12) + (c1 * line20) + (c2 * line01);
				colours.emplace_back(colour);

			}
		}
	}

}

// Currently has missing implementation for colour interpolation
void RasterisationMesh::RasterSpanTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector4& c0, const Vector4& c1, const Vector4& c2) {
	const Vector3* vertices[3] = { &v0 ,&v1 ,&v2 };
	const Vector4* colours[3] = { &c0 ,&c1 ,&c2 };
	
	// get the ’ highest ’ vertex into slot 0
	if (vertices[1]->y < vertices[0]->y) {
		std::swap(vertices[1], vertices[0]);
		std::swap(colours[1], colours[0]);
	}
	if (vertices[2]->y < vertices[0]->y) {
		std::swap(vertices[2], vertices[0]);
		std::swap(colours[2], colours[0]);
	}
	// slot 0 is the highest , now make 2 the lowest
	if (vertices[2]->y < vertices[1]->y) {
		std::swap(vertices[2], vertices[1]);
		std::swap(colours[2], colours[1]);
	}
	if (vertices[0]->y == vertices[1]->y) {
		// make [0] the highest ,
		if (vertices[0]->x > vertices[1]->x) { // left - most vertex
			std::swap(vertices[1], vertices[0]);
			std::swap(colours[1], colours[0]);
		}
	}

	// the long axis will now always be vertices [0] and [2]
	Vector3 longDir = *vertices[2] - *vertices[0];
	Vector3 longDelta = longDir / longDir.y;
	Vector3 shortDir = *vertices[1] - *vertices[0];
	Vector3 shortDelta = shortDir / shortDir.y;

	float longX = vertices[0]->x;
	float shortX = vertices[0]->x;

	for (int y = vertices[0]->y; y < vertices[1]->y; ++y) {
		int startX = std::min(longX, shortX);
		int endX = std::max(longX, shortX);
		for (int x = startX; x < endX; ++x) {
			positions.emplace_back(Vector3(x, y, 1.0f));
			this->colours.emplace_back(Vector4{c0});
		}
		longX += longDelta.x;
		shortX += shortDelta.x;
	}

	if (vertices[0]->y == vertices[1]->y) {
		shortX = vertices[1]->x; // previously sorted !
		longX = vertices[0]->x;
	}
	shortDir = *vertices[2] - *vertices[1];
	shortDelta = shortDir / shortDir.y;
	for (int y = vertices[1]->y; y < vertices[2]->y; ++y) {
		int startX = std::min(longX, shortX);
		int endX = std::max(longX, shortX);
		for (int x = startX; x < endX; ++x) {
			positions.emplace_back(Vector3(x, y, 1.0f));
			this->colours.emplace_back(Vector4(c0));
		}
		longX += longDelta.x;
		shortX += shortDelta.x;
	}
}

void RasterisationMesh::RasterBarycentricTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector4& c0, const Vector4& c1, const Vector4& c2) {
	Vector2 topLeft, bottomRight;
	Maths::ScreenBoxOfTri(p0, p1, p2, topLeft, bottomRight);
	float triArea = abs(Maths::ScreenAreaOfTri(p0, p1, p2));
	float areaRecip = 1.0f / triArea;

	int	subTriArea[3];
	for (float y = topLeft.y; y < bottomRight.y; y += 1) {
		for (float x = topLeft.x; x < bottomRight.x; x += 1) {
			Vector3 screenPos((float)x, (float)y, 0.0f);
			subTriArea[0] = abs(ScreenAreaOfTri(p0, screenPos, p1));
			subTriArea[1] = abs(ScreenAreaOfTri(p1, screenPos, p2));
			subTriArea[2] = abs(ScreenAreaOfTri(p2, screenPos, p0));
			float triSum = subTriArea[0] + subTriArea[1] + subTriArea[2];
			if ((int)triSum > abs(triArea)) {
				continue;
			}
			float alpha = subTriArea[1] * areaRecip;
			float beta = subTriArea[2] * areaRecip;
			float gamma = subTriArea[0] * areaRecip;
			Vector4 colour = ((c0 * alpha) + (c1 * beta) + (c2 * gamma));
			colour.w = 1;
			positions.emplace_back(screenPos);
			colours.emplace_back(colour);
		}
	}
}

#include "RenderObject.h"
#include "../../Common/MeshGeometry.h"

using namespace NCL;
using namespace CSC3223;

RenderObject::RenderObject(MeshGeometry* inMesh, Matrix4 m)
{
	mesh = inMesh;
	transform = m;
	shader = nullptr;
}

RenderObject::~RenderObject()
{
}
#include "RenderObject.h"
#include "../../Common/MeshGeometry.h"

using namespace NCL;
using namespace CSC3223;

RenderObject::RenderObject(MeshGeometry* inMesh, Matrix4 m, ShaderBase* shader)
{
	mesh = inMesh;
	transform = m;
	this->shader = shader;
}

RenderObject::~RenderObject()
{
}
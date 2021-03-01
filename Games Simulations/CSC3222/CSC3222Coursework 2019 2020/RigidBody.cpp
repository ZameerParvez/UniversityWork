#include "RigidBody.h"

using namespace NCL;
using namespace CSC3222;

RigidBody::RigidBody(float invMass, float elasticity)	{
	inverseMass = invMass;
	elasticity  = elasticity;
}


RigidBody::~RigidBody()	{

}

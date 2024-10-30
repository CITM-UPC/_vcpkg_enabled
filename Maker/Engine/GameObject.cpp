#include "GameObject.h"
#include <GL/glew.h>
#include <iostream>
using namespace std;

GameObject::GameObject(const std::string& name) : name(name), cachedComponentType(typeid(Component))
{
	_transform = AddComponent<TransformComponent>();
}

GameObject::~GameObject()
{
	
}

void GameObject::draw() const {

	glPushMatrix();
	glMultMatrixd(_transform->GetData());

	if (auto meshRenderer = GetComponent<MeshLoader>())
	{
		meshRenderer->Render();
	}

	for (const auto& child : children())
	{
		child.draw();
	}

	glPopMatrix();
}

std::string GameObject::GetName() const
{
	return name;
}

void GameObject::SetName(const std::string& name)
{
	this->name = name;
}

bool GameObject::CompareTag(const std::string& tag) const
{
	return this->tag == tag;
}

BoundingBox GameObject::boundingBox() const {
	BoundingBox bbox = localBoundingBox();
	if (!_mesh_ptr && children().size()) bbox = children().front().boundingBox();
	for (const auto& child : children()) bbox = bbox + child.boundingBox();
	return _transform->GetMatrix() * bbox;
}

void GameObject::drawAxis(double size) {
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(size, 0, 0);
	glColor3ub(0, 255, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, size, 0);
	glColor3ub(0, 0, 255);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, size);
	glEnd();
}

inline static void glVertex3(const vec3& v) { glVertex3dv(&v.x); }
void GameObject::drawWiredQuad(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& v3) {
	glBegin(GL_LINE_LOOP);
	glVertex3(v0);
	glVertex3(v1);
	glVertex3(v2);
	glVertex3(v3);
	glEnd();
}

void GameObject::drawBoundingBox(const BoundingBox& bbox) {
	glLineWidth(2.0);
	drawWiredQuad(bbox.v000(), bbox.v001(), bbox.v011(), bbox.v010());
	drawWiredQuad(bbox.v100(), bbox.v101(), bbox.v111(), bbox.v110());
	drawWiredQuad(bbox.v000(), bbox.v001(), bbox.v101(), bbox.v100());
	drawWiredQuad(bbox.v010(), bbox.v011(), bbox.v111(), bbox.v110());
	drawWiredQuad(bbox.v000(), bbox.v010(), bbox.v110(), bbox.v100());
	drawWiredQuad(bbox.v001(), bbox.v011(), bbox.v111(), bbox.v101());

}

void GameObject::drawDebug(const GameObject& obj) {
	glPushMatrix();
	glColor3ub(255, 255, 0);
	drawBoundingBox(obj.boundingBox());
	glMultMatrixd(obj.GetTransform()->GetData());
	drawAxis(0.5);
	glColor3ub(255, 255, 255);
	drawBoundingBox(obj.localBoundingBox());
	for (const auto& child : obj.children()) drawDebug(child);
	glPopMatrix();
}

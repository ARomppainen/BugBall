#include "Transform.h"

#include "IwGeomFVec4.h"
#include "IwGeomFQuat.h"

typedef CIwFVec4 vec4;
typedef CIwFQuat quat;

Transform::Transform()
{
	position = vec3(0.0f, 0.0f, 0.0f);
    offset   = vec3(0.0f, 0.0f, 0.0f);
	rotation = vec3(0.0f, 0.0f, 0.0f);
	scale    = vec3(1.0f, 1.0f, 1.0f);
    modified = true;
}

Transform::~Transform()
{
}

void Transform::setPosition(float x, float y, float z)
{
	position = vec3(x, y, z);
    modified = true;
}

void Transform::setOffset(float x, float y, float z)
{
    offset = vec3(x, y, z);
    modified = true;
}

void Transform::setRotation(float x, float y, float z)
{
	rotation = vec3(x, y, z);
    modified = true;
}

void Transform::setScale(float x, float y, float z)
{
	scale = vec3(x, y, z);
    modified = true;
}

void Transform::setPosition(vec3 p)
{
    position = p;
    modified = true;
}

void Transform::setOffset(vec3 o)
{
    offset = o;
    modified = true;
}

void Transform::setRotation(vec3 r)
{
    rotation = r;
    modified = true;
}

void Transform::setScale(vec3 s)
{
    scale = s;
    modified = true;
}

void Transform::Translate(float x, float y, float z)
{
	position = position + vec3(x, y, z);
    modified = true;
}

void Transform::Rotate(const vec3& axis, float angle)
{
	quat rotor = quat(axis, angle);
    rotation = vec3(rotor.RotateVector(rotation));
}

void Transform::Scale(float x, float y, float z)
{
	scale = vec3(scale.x * x, scale.y * y, scale.z * z);
    modified = true;
}

vec3 Transform::right()
{
    Matrix4 mat = Matrix4::RotationMatrix(rotation);
    mat.transpose();
    vec4 v = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    v = mat * v;

    return vec3(v.x, v.y, v.z);
}

vec3 Transform::up()
{
    Matrix4 mat = Matrix4::RotationMatrix(rotation);
    mat.transpose();
    vec4 v = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    v = mat * v;

    return vec3(v.x, v.y, v.z);
}

vec3 Transform::forward()
{
    Matrix4 mat = Matrix4::RotationMatrix(rotation);
    mat.transpose();
    vec4 v = vec4(0.0f, 0.0f, -1.0f, 1.0f);
    v = mat * v;

    return vec3(v.x, v.y, v.z);
}

Matrix4& Transform::getModelMatrix()
{
    if (modified) {
        modified = false;
        
        Matrix4 t = Matrix4::TranslationMatrix(this->position + this->offset);
        Matrix4 r = Matrix4::RotationMatrix(this->rotation);
        Matrix4 s = Matrix4::ScalingMatrix(this->scale);

        modelMatrix = t * r * s;
    }

    return modelMatrix;
}
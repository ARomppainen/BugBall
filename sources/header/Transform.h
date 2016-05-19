#pragma once

#include "IwGeomFVec3.h"

#include "Matrix4.h"

typedef CIwFVec3 vec3;

class Transform
{
public:
	Transform();
	~Transform();

	void setPosition(float x, float y, float z);
    void setOffset(float x, float y, float z);
    void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);

    void setPosition(vec3 p);
    void setOffset(vec3 o);
    void setRotation(vec3 r);
    void setScale(vec3 s);

	void Translate(float x, float y, float z);
    void Rotate(const vec3& axis, float angle);
	void Scale(float x, float y, float z);

    vec3 right();
    vec3 up();
    vec3 forward();

    Matrix4& getModelMatrix();

	vec3 position;
    vec3 offset;
	vec3 rotation;
	vec3 scale;

private:

    Matrix4 modelMatrix;
    bool modified;
};
#pragma once

#include "IwGL.h"

#include "IwGeomFVec3.h"
#include "IwGeomFVec4.h"

typedef CIwFVec3 vec3;
typedef CIwFVec4 vec4;

class Matrix4
{
public:

    Matrix4();
    Matrix4(float values[]);
    Matrix4(float a11, float a12, float a13, float a14,
            float a21, float a22, float a23, float a24,
            float a31, float a32, float a33, float a34,
            float a41, float a42, float a43, float a44);
    Matrix4(const Matrix4& other);
    ~Matrix4();

    const GLfloat* getValues();
    float determinant();
    void transpose();

    static Matrix4 Identity();
    static Matrix4 ScalingMatrix(const vec3& scale);
    static Matrix4 TranslationMatrix(const vec3& translation);
    static Matrix4 RotationMatrix(const vec3& rotation);
    static Matrix4 PerspectiveM(float fieldOfView, float aspectRatio, float near, float far);
    static Matrix4 OrthoM(float left, float right, float top, float bottom, float near, float far);

    const Matrix4 operator*(float value) const;
    const vec4 operator*(const vec4& vec) const;
    const Matrix4 operator*(const Matrix4& other) const;

    Matrix4& operator=(const Matrix4& other);
    bool operator==(const Matrix4& other) const;

private:

    GLfloat* matrix;

    static bool equals(float f1, float f2);
};
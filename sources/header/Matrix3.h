#pragma once

#include "IwGL.h"

#include "IwGeomFVec3.h"

typedef CIwFVec3 vec3;

class Matrix3
{
public:
    Matrix3();
    Matrix3(float values[]);
    Matrix3(float a11, float a12, float a13,
            float a21, float a22, float a23,
            float a31, float a32, float a33);
    Matrix3(const Matrix3& other);
    ~Matrix3();

    const GLfloat* getValues();
    float determinant();
    void transpose();

    static Matrix3 Identity();

    const Matrix3 operator*(float value) const;
    const vec3 operator*(const vec3& vec) const;
    const Matrix3 operator*(const Matrix3& other) const;

    Matrix3& operator=(const Matrix3& other);
    bool operator==(const Matrix3& other) const;

private:

    GLfloat* matrix;

    static bool equals(float f1, float f2);
};
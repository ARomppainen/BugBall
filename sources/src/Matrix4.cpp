#include "Matrix4.h"

#include <cmath>
#include "IwGeomFQuat.h"
#include "Matrix3.h"

typedef CIwFQuat quat;

Matrix4::Matrix4()
{
    matrix = new float[16];

    for (int i = 0; i < 16; ++i) {
        matrix[i] = 0.0f;
    }
}

// creates a new 4x4 matrix from an array of floats
// the array must have exactly 16 values
Matrix4::Matrix4(float values[])
{
    matrix = new float[16];

    for (int i = 0; i < 16; ++i) {
        matrix[i] = values[i];
    }
}

Matrix4::Matrix4(float a11, float a12, float a13, float a14,
                 float a21, float a22, float a23, float a24,
                 float a31, float a32, float a33, float a34,
                 float a41, float a42, float a43, float a44)
{
    matrix     = new float[16];
    matrix[0]  = a11;
    matrix[1]  = a21;
    matrix[2]  = a31;
    matrix[3]  = a41;
    matrix[4]  = a12;
    matrix[5]  = a22;
    matrix[6]  = a32;
    matrix[7]  = a42;
    matrix[8]  = a13;
    matrix[9]  = a23;
    matrix[10] = a33;
    matrix[11] = a43;
    matrix[12] = a41;
    matrix[13] = a42;
    matrix[14] = a43;
    matrix[15] = a44;
}
Matrix4::Matrix4(const Matrix4& other)
{
    matrix = new float[16];

    for (int i = 0; i < 16; ++i) {
        matrix[i] = other.matrix[i];
    }
}

Matrix4::~Matrix4()
{
    delete[] matrix;
}

const GLfloat* Matrix4::getValues()
{
    return matrix;
}

float Matrix4::determinant()
{
    float det = 0;
    float* m = this->matrix;

    Matrix3 m1 = Matrix3(
        m[5], m[9], m[13],
        m[6], m[10], m[14],
        m[7], m[11], m[15]);
    Matrix3 m2 = Matrix3(
        m[1], m[9], m[13],
        m[2], m[10], m[14],
        m[3], m[11], m[15]);
    Matrix3 m3 = Matrix3(
        m[1], m[5], m[13],
        m[2], m[6], m[14],
        m[3], m[7], m[15]);
    Matrix3 m4 = Matrix3(
        m[1], m[5], m[9],
        m[2], m[6], m[10],
        m[3], m[7], m[11]);

    det += m[0]  * m1.determinant();
    det -= m[4]  * m2.determinant();
    det += m[8]  * m3.determinant();
    det -= m[12] * m4.determinant();
    
    return det;

    // 1, 1 = 0 | 1, 2 = 4 | 1, 3 = 8  | 1, 4 = 12
    // 2, 1 = 1 | 2, 2 = 5 | 2, 3 = 9  | 2, 4 = 13
    // 3, 1 = 2 | 3, 2 = 6 | 3, 3 = 10 | 3, 4 = 14
    // 4, 1 = 3 | 4, 2 = 7 | 4, 3 = 11 | 4, 4 = 15

    /*
    det += element(1, 1) * new Matrix3(
				element(2, 2), element(2, 3), element(2, 4),
				element(3, 2), element(3, 3), element(3, 4),
				element(4, 2), element(4, 3), element(4, 4)).determinant();
		
		det -= element(1, 2) * new Matrix3(
				element(2, 1), element(2, 3), element(2, 4),
				element(3, 1), element(3, 3), element(3, 4),
				element(4, 1), element(4, 3), element(4, 4)).determinant();
		
		det += element(1, 3) * new Matrix3(
				element(2, 1), element(2, 2), element(2, 4),
				element(3, 1), element(3, 2), element(3, 4),
				element(4, 1), element(4, 2), element(4, 4)).determinant();
		
		det -= element(1, 4) * new Matrix3(
				element(2, 1), element(2, 2), element(2, 3),
				element(3, 1), element(3, 2), element(3, 3),
				element(4, 1), element(4, 2), element(4, 3)).determinant();
        */
}

void Matrix4::transpose()
{
    // 1, 1 = 0 | 1, 2 = 4 | 1, 3 = 8  | 1, 4 = 12
    // 2, 1 = 1 | 2, 2 = 5 | 2, 3 = 9  | 2, 4 = 13
    // 3, 1 = 2 | 3, 2 = 6 | 3, 3 = 10 | 3, 4 = 14
    // 4, 1 = 3 | 4, 2 = 7 | 4, 3 = 11 | 4, 4 = 15

    float temp;
    float* m = this->matrix;

    temp = m[1];
    m[1] = m[4];
    m[4] = temp;

    temp = m[2];
    m[2] = m[8];
    m[8] = m[2];
    
    temp = m[6];
    m[6] = m[9];
    m[9] = temp;

    temp = m[3];
    m[3] = m[12];
    m[12] = temp;

    temp = m[7];
    m[7] = m[13];
    m[13] = temp;

    temp = m[11];
    m[11] = m[14];
    m[14] = temp;
}

Matrix4 Matrix4::Identity()
{
    Matrix4 result    = Matrix4();
    result.matrix[0]  = 1.0f;
    result.matrix[5]  = 1.0f;
    result.matrix[10] = 1.0f;
    result.matrix[15] = 1.0f;

    return result;
}

Matrix4 Matrix4::ScalingMatrix(const vec3& scale)
{
    Matrix4 mat = Matrix4();
    mat.matrix[0]  = scale.x;
    mat.matrix[5]  = scale.y;
    mat.matrix[10] = scale.z;
    mat.matrix[15] = 1.0f;

    return mat;
}

Matrix4 Matrix4::TranslationMatrix(const vec3& translation)
{
    Matrix4 mat = Matrix4::Identity();
    mat.matrix[12] = translation.x;
    mat.matrix[13] = translation.y;
    mat.matrix[14] = translation.z;

    return mat;
}

// http://www.cs.princeton.edu/~gewang/projects/darth/stuff/quat_faq.html#Q36
Matrix4 Matrix4::RotationMatrix(const vec3& rotation)
{
    Matrix4 mat = Matrix4();

    float toRadians = 3.14159265f / 180.0f;

    float A = cos(rotation.x * toRadians);
    float B = sin(rotation.x * toRadians);
    float C = cos(rotation.y * toRadians);
    float D = sin(rotation.y * toRadians);
    float E = cos(rotation.z * toRadians);
    float F = sin(rotation.z * toRadians);

    float AD = A * D;
    float BD = B * D;

    mat.matrix[0]  =   C * E;
    mat.matrix[1]  =  -C * F;
    mat.matrix[2]  =   D;
    mat.matrix[4]  =  BD * E + A * F;
    mat.matrix[5]  = -BD * F + A * E;
    mat.matrix[6]  =  -B * C;
    mat.matrix[8]  = -AD * E + B * F;
    mat.matrix[9]  =  AD * F + B * E;
    mat.matrix[10] =  A * C;

    mat.matrix[15] = 1;

    return mat;
}

// http://stackoverflow.com/questions/18404890/how-to-build-perspective-projection-matrix-no-api
// http://www.songho.ca/opengl/gl_projectionmatrix.html
// http://unspecified.wordpress.com/2012/06/21/calculating-the-gluperspective-matrix-and-other-opengl-matrix-maths/
Matrix4 Matrix4::PerspectiveM(float fieldOfView, float aspectRatio, float near, float far)
{
    Matrix4 mat = Matrix4();

    float toRadians = 3.14159265f / 180.0f;

    float f = 1.0f / tan(0.5f * fieldOfView * toRadians);
    mat.matrix[0] = f / aspectRatio;
    mat.matrix[5] = f;
    mat.matrix[10] = (far + near) / (far - near);
    mat.matrix[14] = (-2.0f * far * near) / (far - near);
    mat.matrix[11] = 1.0f;

    return mat;
}

// http://www.songho.ca/opengl/gl_projectionmatrix.html
Matrix4 Matrix4::OrthoM(float left, float right, float top, float bottom, float near, float far)
{
    Matrix4 mat = Matrix4();

    mat.matrix[0] = 2.0f / (right - left);
    mat.matrix[5] = 2.0f / (top - bottom);
    mat.matrix[10] = -2.0f / (far - near);
    mat.matrix[12] = -(right + left) / (right - left);
    mat.matrix[13] = -(top + bottom) / (top - bottom);
    mat.matrix[14] = -(far + near) / (far - near);
    mat.matrix[15] = 1.0f;

    return mat;
}

const Matrix4 Matrix4::operator*(float value) const
{
    Matrix4 result = *this;

    for (int i = 0; i < 16; ++i) {
        result.matrix[i] *= value;
    }

    return result;
}

const vec4 Matrix4::operator*(const vec4& vec) const
{
    float x =
        this->matrix[0]  * vec.x +
        this->matrix[4]  * vec.y +
        this->matrix[8]  * vec.z +
        this->matrix[12] * vec.w;

    float y =
        this->matrix[1]  * vec.x +
        this->matrix[4]  * vec.y +
        this->matrix[9]  * vec.z +
        this->matrix[13] * vec.w;

    float z =
        this->matrix[2]  * vec.x +
        this->matrix[6]  * vec.y +
        this->matrix[10] * vec.z +
        this->matrix[14] * vec.w;

    float w =
        this->matrix[3]  * vec.x +
        this->matrix[7]  * vec.y +
        this->matrix[11] * vec.z +
        this->matrix[15] * vec.w;

    vec4 v = vec4(x, y, z, w);

    return v;
}

const Matrix4 Matrix4::operator*(const Matrix4& other) const
{
    Matrix4 result = Matrix4();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                result.matrix[j * 4 + i] += this->matrix[k * 4 + i] * other.matrix[j * 4 + k];
            }
        }
    }

    return result;
}

Matrix4& Matrix4::operator=(const Matrix4& other)
{
    if (this == &other)
        return *this;

    for (int i = 0; i < 16; ++i) {
        this->matrix[i] = other.matrix[i];
    }

    return *this;
}

bool Matrix4::operator==(const Matrix4& other) const
{
    for (int i = 0; i < 16; ++i) {
        if ( !equals(this->matrix[i], other.matrix[i]) ) {
            return false;
        }
    }

    return true;
}

bool Matrix4::equals(float f1, float f2)
{
    const float delta = 0.001f;
    return ((f1 < f2 + delta) && (f1 > f2 - delta)) ? true : false;
}
#include "Matrix3.h"

Matrix3::Matrix3()
{
    matrix = new float[9];

    for (int i = 0; i < 9; ++i) {
        matrix[i] = 0.0f;
    }
}

Matrix3::Matrix3(float values[])
{
    matrix = new float[9];
    
    for (int i = 0; i < 9; ++i) {
        matrix[i] = values[i];
    }
}

Matrix3::Matrix3(float a11, float a12, float a13,
                 float a21, float a22, float a23,
                 float a31, float a32, float a33)
{
    matrix    = new float[9];
    matrix[0] = a11;
    matrix[1] = a21;
    matrix[2] = a31;
    matrix[3] = a12;
    matrix[4] = a22;
    matrix[5] = a32;
    matrix[6] = a13;
    matrix[7] = a23;
    matrix[8] = a33;
}

Matrix3::Matrix3(const Matrix3& other)
{
    matrix = new float[9];

    for (int i = 0; i < 9; ++i) {
        matrix[i] = other.matrix[i];
    }
}

Matrix3::~Matrix3()
{
    delete[] matrix;
}

const GLfloat* Matrix3::getValues()
{
    return matrix;
}

float Matrix3::determinant()
{
    float* m = this->matrix;

    float det = 
        m[0] * m[4] * m[8] +
        m[3] * m[7] * m[2] +
        m[6] * m[1] * m[5] -
        m[6] * m[4] * m[2] -
        m[3] * m[1] * m[8] -
        m[0] * m[7] * m[5];

    return det;

    // 1, 1 = 0 | 1, 2 = 3 | 1, 3 = 6
    // 2, 1 = 1 | 2, 2 = 4 | 2, 3 = 7
    // 3, 1 = 2 | 3, 2 = 5 | 3, 3 = 8

    /*
    return
    element(1, 1) * element(2, 2) * element(3, 3) + CHECK
    element(1, 2) * element(2, 3) * element(3, 1) + CHECK
    element(1, 3) * element(2, 1) * element(3, 2) - CHECK
    element(1, 3) * element(2, 2) * element(3, 1) - CHECK
    element(1, 2) * element(2, 1) * element(3, 3) - CHECK
    element(1, 1) * element(2, 3) * element(3, 2);  CHECK
    */
}

void Matrix3::transpose()
{
    float temp;
    float* m = this->matrix;

    temp = m[1];
    m[1] = m[3];
    m[3] = temp;

    temp = m[2];
    m[2] = m[6];
    m[6] = temp;

    temp = m[5];
    m[5] = m[7];
    m[7] = temp;
}

Matrix3 Matrix3::Identity()
{
    Matrix3 result = Matrix3();
    result.matrix[0] = 1.0f;
    result.matrix[4] = 1.0f;
    result.matrix[8] = 1.0f;

    return result;
}

const Matrix3 Matrix3::operator*(float value) const
{
    Matrix3 result = *this;

    for (int i = 0; i < 9; ++i) {
        result.matrix[i] *= value;
    }

    return result;
}

const vec3 Matrix3::operator*(const vec3& vec) const
{
    float x =
        this->matrix[0] * vec.x +
        this->matrix[3] * vec.y +
        this->matrix[6] * vec.z;

    float y =
        this->matrix[1] * vec.x +
        this->matrix[4] * vec.y +
        this->matrix[7] * vec.z;

    float z =
        this->matrix[2] * vec.x +
        this->matrix[5] * vec.y +
        this->matrix[8] * vec.z;

    vec3 v = vec3(x, y, z);

    return v;
}

const Matrix3 Matrix3::operator*(const Matrix3& other) const
{
    Matrix3 result = Matrix3();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                result.matrix[j * 3 + i] += this->matrix[k * 3 + i] * other.matrix[j * 3 + k];
            }
        }
    }

    return result;
}

Matrix3& Matrix3::operator=(const Matrix3& other)
{
    if (this == &other)
        return *this;

    for (int i = 0; i < 9; ++i) {
        this->matrix[i] = other.matrix[i];
    }

    return *this;
}

bool Matrix3::operator==(const Matrix3& other) const
{
    for (int i = 0; i < 9; ++i) {
        if ( !equals(this->matrix[i], other.matrix[i]) ) {
            return false;
        }
    }

    return true;
}

bool Matrix3::equals(float f1, float f2)
{
    const float delta = 0.001f;
    return ((f1 < f2 + delta) && (f1 > f2 - delta)) ? true : false;
}
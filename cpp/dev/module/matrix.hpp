
#pragma once

#include <cstring>

namespace np {

#define datatype float

template<class T>
void add(T& c, const T& a, const T& b){
    for (int i=0; i < c.rows*c.cols; ++i) c[i] = a[i] + b[i];
}

class Matrix {
public:
    Matrix(int r, int c): rows(r), cols(c) {}

    // void add(float* a, float* b, int row, int col){
    //     for (int i=0; i < row*col; ++i) array[i] = a[i] + b[i];
    // }

    // void operator*(const Matrix& c, const Matrix& a, const Matrix& b); // c = a * b
    // void operator*(const Matrix& c, const float a, const Matrix& b); // c = a * b
    // inline const float operator[](const int r, const int c){return array[rows*r+c];}
    inline const float operator[](const int i) const {return array[i];}
    inline float& operator[](const int i) {return array[i];}
    // inline float* data() const {return array;}

    const int rows;
    const int cols;

// protected:
    float array[16];
    // float scratch[16];
};

class Matrix3x3: public Matrix {
public:
    Matrix3x3(): Matrix(3,3) {}

    friend Matrix3x3& operator+(const Matrix3x3& a, const Matrix3x3& b){
        Matrix3x3 c = Matrix3x3();
        add(c,a,b);
        return c;
    }

    // Matrix3x3 operator*(const Matrix3x3& a, const Matrix3x3& b){
    //     Matrix3x3 c = Matrix3x3();
    //     multmm(c.array, a.array, b.array);
    //     return c;
    // }

    // Matrix3x3 operator*(const float a, const Matrix3x3& b){
    //     Matrix3x3 c = Matrix3x3();
    //     for (int i=0; i<9; ++i) c[i] = a*b[i];
    //     return c;
    // }

    // void operator=(const Matrix3x3& a) {
    //     std::memcpy(array, a.data(), sizeof(array));
    // }

// protected:
    // float array[9];
};

// class Matrix4x4;


} // end namespace np

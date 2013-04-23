/***************************************************************************//**
 * @file matrix.h
 ******************************************************************************/

#ifndef MATRIX_H
#define MATRIX_H

#include <qtimagelib.h>


/******************************************************************************
 * Typedefs
 ******************************************************************************/
typedef unsigned int uint;



/***************************************************************************//**
 * @enum MATRIX_ERROR
 * @par Description
 * Error types that can be thrown by the class
 ******************************************************************************/
enum MATRIX_ERROR
{
    MATRIX_ERROR_OUT_OF_BOUNDS = 0,
    MATRIX_ERROR_MISMATCHED_SIZE
};



/***************************************************************************//**
 * @class Matrix
 * @author David Jarman and Hayden Waisanen
 ******************************************************************************/
class Matrix
{
public:
    Matrix();
    Matrix(uint rows, uint cols);
    Matrix(const Matrix &toCopy);
    void SetFrom(Image &image);

    ~Matrix();

    void Print();

    uint NumRows() const;
    uint NumCols() const;

    Matrix GetRow(uint row) const;
    Matrix GetCol(uint col) const;
    double* ToVector() const;
    double At(uint i, uint j) const;

    void Set(double value, uint i, uint j);
    void SetColumn(Matrix& column, uint col);
    void Normalize();
    double Magnitude();
    void Scale();
    Image ToImage(int height, int width);

    Matrix operator+(const Matrix& other);
    Matrix operator-(const Matrix& other);
    Matrix operator/(double scalar);
    Matrix operator*(uint scalar);
    Matrix operator=(const Matrix& toCopy);
    double* operator[](const uint i);

    static Matrix Add(Matrix left, Matrix right);
    void Add(Matrix & toAdd);
    void Subtract(Matrix & toSub);
    void Divide(double scalar);
    void Multiply(double scalar);
    static Matrix Subtract(Matrix left, Matrix right);
    static Matrix Divide(Matrix matrix, double scalar);
    static Matrix Multiply(Matrix left, Matrix right);
    static Matrix Multiply(Matrix matrix, double scalar);
    static Matrix Transpose(Matrix matrix);
    void eigen(Matrix & eigen_vectors, Matrix & eigen_values);

private:
    uint _rows;
    uint _cols;
    double** _data;
    double* _data1D;
};

#endif // MATRIX_H

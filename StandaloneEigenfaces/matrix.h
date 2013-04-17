#ifndef MATRIX_H
#define MATRIX_H

typedef unsigned int uint;

enum MATRIX_ERROR
{
    MATRIX_ERROR_OUT_OF_BOUNDS = 0,
    MATRIX_ERROR_MISMATCHED_SIZE
};

class Matrix
{
public:
    Matrix();
    Matrix(uint rows, uint cols);
    Matrix(const Matrix &toCopy);

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

    Matrix operator+(const Matrix& other);
    Matrix operator-(const Matrix& other);
    Matrix operator/(uint scalar);
    Matrix operator*(uint scalar);
    double* operator[](const uint i);

    static Matrix Add(Matrix left, Matrix right);
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

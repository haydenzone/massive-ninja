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

    uint NumRows() const;
    uint NumCols() const;

    Matrix GetRow(uint row) const;
    Matrix GetCol(uint col) const;
    double At(uint i, uint j) const;

    void Set(double value, uint i, uint j);

    static Matrix Add(Matrix left, Matrix right);
    static Matrix Subtract(Matrix left, Matrix right);
    static Matrix Divide(Matrix matrix, double scalar);
    static Matrix Multiply(Matrix left, Matrix right);
    static Matrix Multiply(Matrix matrix, double scalar);
    static Matrix Transpose(Matrix matrix);

private:
    uint _rows;
    uint _cols;
    double** _data;
};

#endif // MATRIX_H

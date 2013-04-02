#include "matrix.h"

Matrix::Matrix()
{
    _rows = _cols = 0;
    _data = 0;
}

Matrix::Matrix(uint rows, uint cols)
{
    _rows = rows;
    _cols = cols;

    _data = new double*[_rows];
    for (uint row = 0; row < _rows; row++)
    {
        _data[_cols] = new double[_cols];
    }
}

Matrix::Matrix(const Matrix &toCopy)
{
    this->_rows = toCopy.NumRows();
    this->_cols = toCopy.NumCols();

    _data = new double*[_rows];

    for (uint row = 0; row < _rows; row++)
    {
        _data[row] = new double[_cols];
        for (uint col = 0; col < _cols; col++)
        {
            _data[row][col] = toCopy.At(row, col);
        }
    }
}

Matrix::~Matrix()
{
    for (uint i = 0; i < _rows; i++)
    {
        delete _data[i];
    }

    delete _data;
}

uint Matrix::NumRows() const
{
    return this->_rows;
}

uint Matrix::NumCols() const
{
    return this->_cols;
}

Matrix Matrix::GetRow(uint row) const
{
    if (row >= _rows)
    {
        throw MATRIX_ERROR_OUT_OF_BOUNDS;
    }

    Matrix toReturn(1, 0);

    for (uint row = 0; row < _rows; row++)
    {
        toReturn.Set(_data[row][0], row, 0);
    }

    return toReturn;
}

Matrix Matrix::Matrix::GetCol(uint col) const
{
    if (col >= _cols)
    {
        throw MATRIX_ERROR_OUT_OF_BOUNDS;
    }

    Matrix toReturn(0, 1);

    for (uint col = 0; col < _cols; col++)
    {
        toReturn.Set(_data[0][col], 0, col);
    }

    return toReturn;
}

double Matrix::At(uint i, uint j) const
{
    if (i >= _rows || j >= _cols)
    {
        throw MATRIX_ERROR_OUT_OF_BOUNDS;
    }

    return _data[i][j];
}

void Matrix::Set(double value, uint i, uint j)
{
    if (i >= _rows || j >= _cols)
    {
        throw MATRIX_ERROR_OUT_OF_BOUNDS;
    }

    _data[i][j] = value;
}

Matrix Matrix::Add(Matrix left, Matrix right)
{
    if (left.NumRows() != right.NumRows() || left.NumCols() != right.NumCols())
    {
        throw MATRIX_ERROR_MISMATCHED_SIZE;
    }

    Matrix toReturn(left);

    for (uint i = 0; i < toReturn.NumRows(); i++)
    {
        for (uint j = 0; j < toReturn.NumCols(); j++)
        {
            double val = toReturn.At(i, j);
            val += right.At(i, j);

            toReturn.Set(val, i, j);
        }
    }

    return toReturn;
}

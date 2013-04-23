/***************************************************************************//**
 * @file matrix.cpp
 ******************************************************************************/



/******************************************************************************
 * Includes
 ******************************************************************************/
#include "matrix.h"
#include <iostream>
#include <math.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>




/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Default Matrix constructor
 ******************************************************************************/
Matrix::Matrix()
{
    //set rows and columns to 0 and null the data
    _rows = _cols = 0;
    _data = NULL;
    _data1D = NULL;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Matrix constructor with rows and columns
 ******************************************************************************/
Matrix::Matrix(uint rows, uint cols)
{
    _rows = rows;
    _cols = cols;

    _data1D = new double[_rows * _cols];
    _data = new double*[_rows];

    //Set rows and columns to 0.0
    for (uint row = 0; row < _rows; row++)
    {
        _data[row] = (_data1D + (row * _cols));  //new double[_cols];

        for (uint col = 0; col < _cols; col++)
        {
            _data[row][col] = 0.0;
        }
    }
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Matrix copy constructor
 ******************************************************************************/
Matrix::Matrix(const Matrix &toCopy)
{
    _rows = toCopy.NumRows();
    _cols = toCopy.NumCols();

    _data1D = new double[_rows * _cols];
    _data = new double*[_rows];

    //Copy each element to the nex matrix
    for (uint row = 0; row < _rows; row++)
    {
        _data[row] = (_data1D + (row * _cols));
        for (uint col = 0; col < _cols; col++)
        {
            _data[row][col] = toCopy.At(row, col);
        }
    }
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Matrix deconstructor
 ******************************************************************************/
Matrix::~Matrix()
{
    for(int i = 0; i < _rows; i++)
    {
        _data[i] = NULL;
    }

    if (_data1D != NULL)
    {
        delete[] _data1D;
        _data1D = NULL;
    }


    if (_data != NULL)
    {
        delete[] _data;
        _data = NULL;
    }

}


/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Convert an Image to a Matrix as a 1d column
 ******************************************************************************/
void Matrix::SetFrom(Image &image)
{
   //Clear allocated memory 
   if(_rows != image.Height()*image.Width() && _cols != 1)
   {
      std::cout << "Matrix: incorrect matrix dimensions" << std::endl;
      return;
   }


    for (uint i = 0; i  < image.Height(); i++)
    {
        for (uint j = 0; j < image.Width(); j++)
        {
            _data[i*image.Width() + j] = _data1D + i*image.Width() + j;
            Pixel pix = image[i][j];
            uint val = (pix.Blue() + pix.Red() + pix.Green()) / 3.0;
            this->Set(val,i*image.Width()+j,0);
        }
    }
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Converts the matrix to an image with height and width dimensions
 ******************************************************************************/
Image Matrix::ToImage(int height, int width)
{
   Image toReturn = Image(height, width);
   if(_cols != 1)
   {
      std::cout << "Matrix::ToImage: matrix must be a 1-d col vector" << std::endl;
      return toReturn;
   }
   if(height*width != _rows)
   {
      std::cout << "Matrix::ToImage: image dimensions not valid" << std::endl;
      return toReturn;
   }

    for (uint i = 0; i < height; i++)
    {
        for (uint j = 0; j < width; j++)
        {
            uint val = this->At(i*width + j, 0);
            toReturn[i][j].SetGray(val);
        }
    }

    return toReturn;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Prints the matrix for debugging purposes
 ******************************************************************************/
void Matrix::Print()
{
    for (uint row = 0; row < _rows; row++)
    {
        for (uint col = 0; col < _cols; col++)
        {
            std::cout << _data[row][col] << " ";
        }
        std::cout << std::endl;
    }
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Gets the number of rows of the matrix
 ******************************************************************************/
uint Matrix::NumRows() const
{
    return this->_rows;
}


/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Gets the number of columns of the matrix
 ******************************************************************************/
uint Matrix::NumCols() const
{
    return this->_cols;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Returns a new matrix that is the row of this matrix
 ******************************************************************************/
Matrix Matrix::GetRow(uint row) const
{
    if (row >= _rows)
    {
        throw MATRIX_ERROR_OUT_OF_BOUNDS;
    }

    Matrix toReturn(1, _cols);

    for (uint col = 0; col < _cols; col++)
    {
        toReturn.Set(_data[row][col], 0, col);
    }

    return toReturn;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Returns a new matrix that is the specified columns of this matrix
 ******************************************************************************/
Matrix Matrix::Matrix::GetCol(uint col) const
{
    if (col >= _cols)
    {
        throw MATRIX_ERROR_OUT_OF_BOUNDS;
    }

    Matrix toReturn(_rows, 1);

    for (uint row = 0; row < _rows; row++)
    {
        toReturn.Set(_data[row][col], row, 0);
    }

    return toReturn;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Returns an element of the matrix
 ******************************************************************************/
double Matrix::At(uint i, uint j) const
{
    if (i >= _rows || j >= _cols)
    {
        throw MATRIX_ERROR_OUT_OF_BOUNDS;
    }

    return _data[i][j];
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Sets an element in the matrix
 ******************************************************************************/
void Matrix::Set(double value, uint i, uint j)
{
    if (i >= _rows || j >= _cols)
    {
        std::cout << "Out of bounds\n";
        throw MATRIX_ERROR_OUT_OF_BOUNDS;
    }

    _data[i][j] = value;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Adds two matrices together and returns the result
 ******************************************************************************/
Matrix Matrix::Add(Matrix left, Matrix right)
{
    if (left.NumRows() != right.NumRows() || left.NumCols() != right.NumCols())
    {
        throw MATRIX_ERROR_MISMATCHED_SIZE;
    }

    uint rows = left.NumRows();
    uint cols = left.NumCols();
    Matrix toReturn(rows, cols);

    for (uint i = 0; i < toReturn.NumRows(); i++)
    {
        for (uint j = 0; j < toReturn.NumCols(); j++)
        {
            double value = left.At(i, j) + right.At(i, j);

            toReturn.Set(value, i, j);
        }
    }
    return toReturn;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Adds a matrix to this matrix
 ******************************************************************************/
void Matrix::Add(Matrix & toAdd)
{
    if (this->NumRows() != toAdd.NumRows() || this->NumCols() != toAdd.NumCols())
    {
        throw MATRIX_ERROR_MISMATCHED_SIZE;
    }

    uint rows = this->NumRows();
    uint cols = this->NumCols();

    for (uint i = 0; i < rows; i++)
    {
        for (uint j = 0; j < cols; j++)
        {
            Set(toAdd.At(i,j)+At(i,j), i, j);
        }
    }
    return;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Subtracts a matrix from this matrix
 ******************************************************************************/
void Matrix::Subtract(Matrix & toSub)
{
    if (this->NumRows() != toSub.NumRows() || this->NumCols() != toSub.NumCols())
    {
        throw MATRIX_ERROR_MISMATCHED_SIZE;
    }

    uint rows = this->NumRows();
    uint cols = this->NumCols();

    for (uint i = 0; i < rows; i++)
    {
        for (uint j = 0; j < cols; j++)
        {
            Set(At(i,j)-toSub.At(i,j), i, j);
        }
    }
    return;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Divides this matrix by a scalar
 ******************************************************************************/
void Matrix::Divide(double scalar)
{
    for (uint i = 0; i < _rows; i++)
    {
        for (uint j = 0; j < _cols; j++)
        {
            Set(At(i,j)/scalar, i, j);
        }
    }
    return;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Multiplies this matrix by a scalar
 ******************************************************************************/
void Matrix::Multiply(double scalar)
{
    for (uint i = 0; i < _rows; i++)
    {
        for (uint j = 0; j < _cols; j++)
        {
            Set(At(i,j)*scalar, i, j);
        }
    }
    return;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Subtracts two matricies and returns the result
 ******************************************************************************/
Matrix Matrix::Subtract(Matrix left, Matrix right)
{
    if (left.NumRows() != right.NumRows() || left.NumCols() != right.NumCols())
    {
        throw MATRIX_ERROR_MISMATCHED_SIZE;
    }

    uint rows = left.NumRows();
    uint cols = left.NumCols();
    Matrix toReturn(rows, cols);

    for (uint i = 0; i < rows; i++)
    {
        for (uint j = 0; j < cols; j++)
        {
            double value = left.At(i, j) - right.At(i, j);

            toReturn.Set(value, i, j);
        }
    }

    return toReturn;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Divides a matrix by a scalar and returns the result
 ******************************************************************************/
Matrix Matrix::Divide(Matrix matrix, double scalar)
{
    uint rows = matrix.NumRows();
    uint cols = matrix.NumCols();
    Matrix toReturn(rows, cols);

    for (uint i = 0; i < rows; i++)
    {
        for (uint j = 0; j < cols; j++)
        {
            double value = matrix.At(i, j) / scalar;

            toReturn.Set(value, i, j);
        }
    }

    return toReturn;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Multiplies a matrix by a scalar value and returns the result
 ******************************************************************************/
Matrix Matrix::Multiply(Matrix matrix, double scalar)
{
    uint rows = matrix.NumRows();
    uint cols = matrix.NumCols();
    Matrix toReturn(rows, cols);

    for (uint i = 0; i < rows; i++)
    {
        for (uint j = 0; j < cols; j++)
        {
            double value = matrix.At(i, j) * scalar;

            toReturn.Set(value, i, j);
        }
    }

    return toReturn;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Multiplies two matrices and returns the result
 ******************************************************************************/
Matrix Matrix::Multiply(Matrix left, Matrix right)
{
    if (left.NumCols() != right.NumRows())
    {
        std::cout << "Mismatched dimensions\n";
        throw MATRIX_ERROR_MISMATCHED_SIZE;
    }



    Matrix toReturn(left.NumRows(), right.NumCols());
    uint m = left.NumCols();

    for (uint i = 0; i < toReturn.NumRows(); i++)
    {
        for (uint j = 0; j < toReturn.NumCols(); j++)
        {
            double value = 0;
            for (uint k = 0; k < m; k++)
            {
                value += left.At(i, k) * right.At(k, j);
            }

            toReturn.Set(value, i, j);
        }
    }

    return toReturn;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Transposes a matrix and returns the result
 ******************************************************************************/
Matrix Matrix::Transpose(Matrix matrix)
{
    uint rows = matrix.NumCols();
    uint cols = matrix.NumRows();

    Matrix toReturn(rows, cols);

    for (uint i = 0; i < rows; i++)
    {
        for (uint j = 0; j < cols; j++)
        {
            toReturn.Set(matrix.At(j, i), i, j);
        }
    }

    return toReturn;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Overloads the [] operator to return a double* which is the specified row
 ******************************************************************************/
double* Matrix::operator [](const uint i)
{
    if (i >= _rows)
    {
        throw MATRIX_ERROR_OUT_OF_BOUNDS;
    }

    return _data[i];
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Overloads the = operator to copy a matrix and returns the result
 ******************************************************************************/
Matrix Matrix::operator =(const Matrix& toCopy)
{
    return Matrix(toCopy);
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Concatanates the rows together of this matrix and returns the result
 ******************************************************************************/
double* Matrix::ToVector() const
{
    uint totalSize = _rows * _cols;
    double* toReturn = new double[totalSize];

    for (uint i = 0; i < totalSize; i++)
    {
        toReturn[i] = this->At((uint)floor(i / _rows), i % _cols );
    }

    return toReturn;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Normalizes the matrix
 ******************************************************************************/
void Matrix::Normalize()
{
    double magnitude = Magnitude();
    for( int i = 0; i < _rows; i++)
        Set(At(i, 0)/magnitude, i, 0);
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Returns the magnitude of the matrix
 ******************************************************************************/
double Matrix::Magnitude()
{
    double magnitude = 0.0;
    for( int i = 0; i < _rows; i++)
        magnitude += At(i, 0)*At(i,0);
    magnitude = sqrt(magnitude);
    return magnitude;
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Scales each element in the matrix from 0 to 1
 ******************************************************************************/
void Matrix::Scale()
{
    double maxVal = 0.0;
    double minVal =  100000000.0;
    for (uint i = 0; i < _rows; i++)
    {
        for (uint j = 0; j < _cols; j++)
        {
            double temp = this->At(i, j);
            if (temp > maxVal)
                maxVal = temp;
            if (temp < minVal)
                minVal = temp;
        }
    }

    for (uint i = 0; i < _rows; i++)
    {
        for (uint j = 0; j < _cols; j++)
        {
            double temp = this->At(i, j);
            this->Set((temp - minVal)/(maxVal-minVal), i, j);
        }
    }
}



/***************************************************************************//**
 * @author David Jarman
 * @par Description:
 * Sets a column of the matrix
 ******************************************************************************/
void Matrix::SetColumn(Matrix &column, uint col)
{
    if (column.NumCols() != 1)
        throw MATRIX_ERROR_MISMATCHED_SIZE;
    uint rows = column.NumRows();
    for (uint i = 0; i < rows; i++)
    {
        double val = column[i][0];
        this->Set(val, i, col);
    }
}



/***************************************************************************//**
 * @author Hayden Waisanen
 * @par Description:
 * Gets the eigen vectors and eigen values of this matrix
 ******************************************************************************/
void Matrix::eigen(Matrix & eigen_vectors, Matrix & eigen_values)
{
   if(!(eigen_vectors.NumCols() == _rows && eigen_vectors.NumRows() == _rows))
    {
       std::cout << "param eigen_vectors has incorrect dimensions" << std::endl;
       return;
    }
    if(!(eigen_values.NumRows() == _rows && eigen_values.NumCols() == 1))
    {
       std::cout << "param eigen_values has incorrect dimensions" << std::endl;
       return;
    }

    double * temp_data = new double[_rows*_rows];
    for( uint i = 0; i < _rows*_rows; i++)
    {
        temp_data[i] = _data[0][i];
    }
   gsl_matrix_view m
           = gsl_matrix_view_array (temp_data, _rows, _rows);

   gsl_vector_complex *eval = gsl_vector_complex_alloc (_rows);
   gsl_matrix_complex *evec = gsl_matrix_complex_alloc (_rows, _rows);

   gsl_eigen_nonsymmv_workspace * w =
      gsl_eigen_nonsymmv_alloc (_rows);

   gsl_eigen_nonsymmv (&m.matrix, eval, evec, w);

   gsl_eigen_nonsymmv_free (w);

   gsl_eigen_nonsymmv_sort (eval, evec,
         GSL_EIGEN_SORT_ABS_DESC);

   {
      uint i, j;

      for (i = 0; i < _rows; i++)
      {
         gsl_complex eval_i
            = gsl_vector_complex_get (eval, i);
         gsl_vector_complex_view evec_i
            = gsl_matrix_complex_column (evec, i);

         //printf("eigenvalue = %g + %gi\n",
         //      GSL_REAL(eval_i), GSL_IMAG(eval_i));
         eigen_values.Set(GSL_REAL(eval_i), i, 0);
         //printf("eigenvector = \n");
         for (j = 0; j < _rows; ++j)
         {
            gsl_complex z =
               gsl_vector_complex_get(&evec_i.vector, j);
            //printf("%g + %gi\n", GSL_REAL(z), GSL_IMAG(z));
            eigen_vectors.Set(GSL_REAL(z), j, i ); //Each column vector is an eigen vector
         }
      }
   }

   delete temp_data;
   gsl_vector_complex_free(eval);
   gsl_matrix_complex_free(evec);
}

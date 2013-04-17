#include <QtGui/QApplication>
#include "mainwindow.h"
#include "matrix.h"
#include "iostream"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Matrix blank(10, 15);

    Matrix left(3, 5);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            left.Set(i*j, i, j);
        }
    }

    Matrix right(5, 7);
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            right.Set(j*i, i, j);
        }
    }

    Matrix test = Matrix::Multiply(left, right);

    left.Print();
    std::cout << "Done printing left\n";
    right.Print();
    std::cout << "Done printing right\n";
    test.Print();
    std::cout << "Done printing result\n";


    std::cout << "Printing row\n";
    Matrix row = test.GetRow(1);
    row.Print();

    std::cout << "Printing col\n";
    Matrix col = test.GetCol(1);
    col.Print();

    std::cout << "Printing transpose\n";

    Matrix test_transpose = Matrix::Transpose(test);
    test_transpose.Print();

    std::cout << "Printing Normalized transpose\n";

    test_transpose.Normalize();
    test_transpose.Print();

    Matrix columnMatrix(7, 1);
    for (uint i = 0; i < 7; i++)
    {
        columnMatrix.Set(1, i, 0);
    }

    std::cout << "Printing normalized transpose set column\n";
    test_transpose.SetColumn(columnMatrix, 0);
    test_transpose.Print();

    //double *vect = test_transpose.ToVector();

    //uint size = test_transpose.NumCols() * test_transpose.NumRows();

    //std::cout << "Test vect\n";

    //for (uint i = 0; i < size; i++)
    //{
        //std::cout << "Number " << vect[i] << "\n";
    //}

    return a.exec();
}


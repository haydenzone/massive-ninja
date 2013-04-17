#include <QtGui/QApplication>
#include "mainwindow.h"
#include "matrix.h"
#include "iostream"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

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

    Matrix test_transpose = Matrix::Transpose(test);
    test_transpose.Print();

    
    return a.exec();
}

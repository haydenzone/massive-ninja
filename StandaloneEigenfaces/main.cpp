#include <QtGui/QApplication>
#include "mainwindow.h"
#include "matrix.h"
#include "iostream"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Matrix test(3, 3);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            test.Set(i*j, i, j);
        }
    }

    test = Matrix::Multiply(test, 5.0);

    test.Print();
    
    return a.exec();
}

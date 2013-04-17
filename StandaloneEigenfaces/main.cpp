#include <QtGui/QApplication>
#include "mainwindow.h"
#include "matrix.h"
#include "iostream"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    Image mytest[5];


    mytest[0] = Image("/home/student/1905008/IP/faces94/female/anpage/anpage.10.jpg");
    std::cout << mytest[0].Width() << std::endl;
    std::cout << mytest[0].Height() << std::endl;

    mytest[1] = Image("/home/student/1905008/IP/faces94/female/asewil/asewil.14.jpg");
    std::cout << mytest[1].Width() << std::endl;
    std::cout << mytest[1].Height() << std::endl;
    mytest[2] = Image("/home/student/1905008/IP/faces94/female/kaknig/kaknig.14.jpg");
    std::cout << mytest[2].Width() << std::endl;
    std::cout << mytest[2].Height() << std::endl;
    mytest[3] = Image("/home/student/1905008/IP/faces94/female/lfso/lfso.10.jpg");
    std::cout << mytest[3].Width() << std::endl;
    std::cout << mytest[3].Height() << std::endl;
    mytest[4] = Image("/home/student/1905008/IP/faces94/female/anpage/anpage.11.jpg");
    std::cout << mytest[4].Width() << std::endl;
    std::cout << mytest[4].Height() << std::endl;

    return a.exec();
}


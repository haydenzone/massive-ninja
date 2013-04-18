#include <QtGui/QApplication>
#include "mainwindow.h"
#include "matrix.h"
#include "iostream"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //MainWindow w2;
    //w2.show();

    Image mytest[5];


    mytest[0] = Image("/home/student/1905008/IP/faces94/female/anpage/anpage.10.jpg");
    std::cout << mytest[0].Width() << std::endl;
    std::cout << mytest[0].Height() << std::endl;

    mytest[1] = Image("/home/student/1905008/IP/faces94/female/asewil/asewil.14.jpg");
    mytest[2] = Image("/home/student/1905008/IP/faces94/female/kaknig/kaknig.14.jpg");
    mytest[3] = Image("/home/student/1905008/IP/faces94/female/lfso/lfso.10.jpg");
    mytest[4] = Image("/home/student/1905008/IP/faces94/female/anpage/anpage.11.jpg");
    //std::cout << mytest[4].Width() << std::endl;
    //std::cout << mytest[4].Height() << std::endl;


    int height =mytest[4].Height();
    int width = mytest[4].Width();
    Matrix am(2, 2);
    Matrix b(2, 6);
    am.Set(1,0, 0);
    am.Set(1,0, 1);
    am.Set(1,1, 0);
    am.Set(1,1, 1);
    am.Print();
    b = am;
    b.Print();
    std::cout << b.At(0,0) << std::endl;


    QImage m = mytest[2].ToQImage();

    Matrix * mat[5];
    for( int i = 0; i < 4; i++)
    {
       //Matrix temp = Matrix(mytest[i].Width() *mytest[i].Height(),1);
       mat[i] = new Matrix(mytest[i].Width() *mytest[i].Height(),1);
       mat[i]->SetFrom(mytest[i]);
      //mat[i].Print();
    }

       //std::cout << "go here "<< mat[0]->NumRows() << std::endl;
    //mat[0]->Print();
    Image t = mat[0]->ToImage(mytest[0].Height(), mytest[0].Width());
    

    m = t.ToQImage();
    w.SetImage(m);
    std::cout << mat[0]->NumRows() << std::endl;
    std::cout << mat[0]->NumCols() << std::endl;

    for( int i = 0; i < 4; i++)
    {
       //Matrix temp = Matrix(mytest[i].Width() *mytest[i].Height(),1);
       delete mat[i];
    }

    //QImage image("/home/student/1900263/Pictures/cagef.png");
    //m = mytest[3].ToQImage();
    //w2.SetImage(m);

    return a.exec();
}


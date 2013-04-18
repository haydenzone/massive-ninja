#include <QtGui/QApplication>
#include "mainwindow.h"
#include "matrix.h"
#include "iostream"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MainWindow w2;
    w.show();
    w2.show();

    Image mytest[5];

    //Input images
    mytest[2] = Image("/home/student/1905008/IP/faces94/female/anpage/anpage.10.jpg");
    mytest[1] = Image("/home/student/1905008/IP/faces94/female/asewil/asewil.14.jpg");
    mytest[0] = Image("/home/student/1905008/IP/faces94/female/kaknig/kaknig.14.jpg");
    mytest[3] = Image("/home/student/1905008/IP/faces94/female/lfso/lfso.10.jpg");
    mytest[4] = Image("/home/student/1905008/IP/faces94/female/kaknig/kaknig.11.jpg");


    //Allocate matrices to store images
    Matrix * mat[5];
    Matrix * mean_adjusted[5];
    Matrix * eigenfaces[5];
    Matrix * weights[5];
    for( int i = 0; i < 5; i++)
    {
       mat[i] = new Matrix(mytest[i].Width() *mytest[i].Height(),1);
       mat[i]->SetFrom(mytest[i]);
       mean_adjusted[i] = new Matrix(mytest[i].Width() *mytest[i].Height(),1);
       eigenfaces[i] = new Matrix(mytest[i].Width() *mytest[i].Height(),1);
       weights[i] = new Matrix(5,5);
    }

    //Calculate the average face
    Matrix mean_face(mytest[0].Width()*mytest[1].Height(),1);
    for(int i = 0; i < 4; i++)
    {
        mean_face.Add(*mat[i]);
    }
    mean_face.Divide(4.0);
    Matrix A(mytest[0].Width()*mytest[1].Height(),4);
    for(int i = 0; i < 5; i++)
    {
        mean_adjusted[i]->Add(*mat[i]);
        mean_adjusted[i]->Subtract(mean_face);
        if(i < 4)
            A.SetColumn(*mean_adjusted[i], i);
    }

    Matrix At(Matrix::Transpose(A));
    Matrix L = Matrix::Multiply(At,A);
    Matrix eig_vals(4, 1);
    Matrix eig_vecs(4, 4);
    Matrix U(mytest[0].Width()*mytest[1].Height(), 4);
    L.eigen(eig_vecs, eig_vals);
    for( int i = 0; i < 4; i++)
    {
        Matrix temp(eig_vecs.GetCol(i));
        Matrix temp2(Matrix::Multiply(A, temp));
        eigenfaces[i]->SetColumn(temp2 ,0);
        eigenfaces[i]->Normalize();
        U.SetColumn(*eigenfaces[i], i);
        eigenfaces[i]->Scale();
        eigenfaces[i]->Multiply(255.0);
    }
    Matrix Ut(Matrix::Transpose(U));

    //Calculate weights for each input image (including test)
    for( int i = 0; i < 5; i++)
    {
        Matrix temp = Matrix::Multiply(Ut,*mean_adjusted[i]);
        weights[i]->SetColumn(temp,0);
        weights[i]->Print();
    }
    //Find the face with the closest response
    double min_mag = 100000.0;
    int min_face_i = -1;
    for(int i = 0; i < 4; i++)
    {
        weights[i]->Subtract(*weights[4]);
        double mag = weights[i]->Magnitude();
        if(mag < min_mag)
        {
            min_mag = mag;
            min_face_i = i;
        }
    }

    std::cout << "The closest matching face is: " << min_face_i << std::endl;
    QImage m = mytest[min_face_i].ToQImage();
    w2.SetImage(m);

    m = mytest[4].ToQImage();
    w.SetImage(m);



    //Display the image in mytest at index 0
    /*
    Image t = eigenfaces[0]->ToImage(mytest[0].Height(), mytest[0].Width());
    m = t.ToQImage();
    w.SetImage(m);
    */

    //Free matrices
    for( int i = 0; i < 5; i++)
    {
       delete mat[i];
        delete mean_adjusted[i];
        delete eigenfaces[i];
        delete weights[i];
    }

    return a.exec();
}


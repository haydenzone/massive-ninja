#include <QtGui/QApplication>
#include "mainwindow.h"
#include "matrix.h"
#include "iostream"
#include<dirent.h>
#include<string>
#include<vector>
int eigenface_match( Image faces[], const int IMAGE_COUNT );
typedef std::vector<std::string> strvec;
typedef std::vector<std::string>::iterator strveciter;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MainWindow w2;
    QImage m;
    int min_face_i;
    int IMAGE_COUNT;
    Image mytest[100];
    strvec image_files;
    std::string dir_str = "/home/student/1905008/IP/faces94/female/training";

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dir_str.c_str())) != NULL) {
      while ((ent = readdir (dir)) != NULL) {
          if(ent->d_name[0] == '.')
              continue;
        image_files.push_back(ent->d_name);
      }
      closedir (dir);
    } else {
      perror ("");
      return EXIT_FAILURE;
    }




    //Input images
    IMAGE_COUNT = image_files.size();
    int i = 0;
    //Swap test image to the end
    for( strveciter it = image_files.begin(); it != image_files.end(); it++)
    {
        if((*it) == "training.jpg")
        {
            image_files.erase(it);
            image_files.push_back("training.jpg");
            break;
        }
    }
    for( strveciter it = image_files.begin(); it != image_files.end(); it++)
    {
        mytest[i++] = Image(dir_str+"/"+(*it));
    }


    //Set display 1 to matching training face
    min_face_i = eigenface_match(mytest, IMAGE_COUNT);
    m = mytest[min_face_i].ToQImage();
    w2.SetImage(m);

    //Set display 2 to test face
    m = mytest[IMAGE_COUNT-1].ToQImage();
    w.SetImage(m);

    //Show the images
    w.show();
    w2.show();

    return a.exec();
}

int eigenface_match( Image faces[], const int IMAGE_COUNT )
{
    //Allocate matrices to store images
    Matrix * face_matrices[IMAGE_COUNT];
    Matrix * mean_adjusted[IMAGE_COUNT];
    Matrix * eigenfaces[IMAGE_COUNT];
    Matrix * weights[IMAGE_COUNT];
    for( int i = 0; i < IMAGE_COUNT; i++)
    {
        face_matrices[i] = new Matrix(faces[i].Width() *faces[i].Height(),1);
        face_matrices[i]->SetFrom(faces[i]);
        mean_adjusted[i] = new Matrix(faces[i].Width() *faces[i].Height(),1);
        eigenfaces[i] = new Matrix(faces[i].Width() *faces[i].Height(),1);
        weights[i] = new Matrix(IMAGE_COUNT,IMAGE_COUNT);
    }

    //Calculate the average face
    Matrix mean_face(faces[0].Width()*faces[1].Height(),1);
    for(int i = 0; i < IMAGE_COUNT-1; i++)
    {
        mean_face.Add(*face_matrices[i]);
    }
    mean_face.Divide(IMAGE_COUNT-1);
    Matrix A(faces[0].Width()*faces[0].Height(),IMAGE_COUNT-1);
    for(int i = 0; i < IMAGE_COUNT; i++)
    {
        mean_adjusted[i]->Add(*face_matrices[i]);
        mean_adjusted[i]->Subtract(mean_face);
        if(i < IMAGE_COUNT-1)
            A.SetColumn(*mean_adjusted[i], i);
    }

    Matrix At(Matrix::Transpose(A));
    Matrix L = Matrix::Multiply(At,A);
    Matrix eig_vals(IMAGE_COUNT-1, 1);
    Matrix eig_vecs(IMAGE_COUNT-1, IMAGE_COUNT-1);
    Matrix U(faces[0].Width()*faces[1].Height(), IMAGE_COUNT-1);
    L.eigen(eig_vecs, eig_vals);
    for( int i = 0; i < IMAGE_COUNT-1; i++)
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
    for( int i = 0; i < IMAGE_COUNT; i++)
    {
        Matrix temp = Matrix::Multiply(Ut,*mean_adjusted[i]);
        weights[i]->SetColumn(temp,0);
    }
    //Find the face with the closest response
    double min_mag = 100000.0;
    int min_face_i = -1;
    for(int i = 0; i < IMAGE_COUNT-1; i++)
    {
        weights[i]->Subtract(*weights[IMAGE_COUNT-1]);
        double mag = weights[i]->Magnitude();
        if(mag < min_mag)
        {
            min_mag = mag;
            min_face_i = i;
        }
    }

    //Free matrices
    for( int i = 0; i < IMAGE_COUNT; i++)
    {
        delete face_matrices[i];
        delete mean_adjusted[i];
        delete eigenfaces[i];
        delete weights[i];
    }

    return min_face_i;
}

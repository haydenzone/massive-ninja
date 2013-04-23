/***************************************************************************//**
 * @file facerecognitionapp.cpp
 ******************************************************************************/

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "facerecognitionapp.h"
#include "matrix.h"
#include "iostream"
#include<dirent.h>
#include<string>
#include<vector>
#include <QString>
#include <QFileDialog>
#include <float.h>

/******************************************************************************
 * Typedefs
 ******************************************************************************/
typedef std::vector<std::string> strvec;
typedef std::vector<std::string>::iterator strveciter;

/******************************************************************************
 * Prototypes
 ******************************************************************************/
int eigenface_match( Image faces[], const int IMAGE_COUNT,Image eigen_faces[], int e_count );
bool get_files_dir(std::string dir_str, strvec & image_files);


/***************************************************************************//**
 * @par Description:
 * App constructor
 ******************************************************************************/
FaceRecognitionApp::FaceRecognitionApp(QObject *parent) :
    QObject(parent)
{
}

/***************************************************************************//**
 * @author Hayden Waisanen
 *
 * @par Description:
 * This function adds facial recognition with eigenfaces to the QtImageLib
 * application. The user is first prompted for a directory containing a
 * set of images to be used for training. Eigenfaces are constructed and
 * used to find the closest matching training face to the image that is currently
 * open. All images must have the same dimensions.
 *
 * @param[in] image
 *
 * @returns bool
 *
 ******************************************************************************/
bool FaceRecognitionApp::Menu_FacialRecognition_Eigenfaces(Image &image)
{
    int min_face_i;            //Face image with the closest match
    int IMAGE_COUNT;           //Number of images used for training
    Image training_images[100];//Training images for eigenfaces
    Image eigen_faces[10];     //Used to display a sampling of eigenfaces
    strvec image_files;        //Image files in selected directory
    std::string dir_str =      //Path to directory containing training images
            QFileDialog::getExistingDirectory(NULL,
                        "Please choose a directory",
                        "/home/",
                        QFileDialog::ShowDirsOnly).toStdString();

    //Get the files in this directroy
    if(!get_files_dir( dir_str, image_files))
        return false;

    //Input images
    IMAGE_COUNT = image_files.size();
    if(IMAGE_COUNT > 100)
    {
        std::cout << "Training set too large" << std::endl;
        return false;
    }
    int i = 0;
    for( strveciter it = image_files.begin(); it != image_files.end(); it++)
    {
        training_images[i++] = Image(dir_str+"/"+(*it));
    }

    //Place test image file at last index
    training_images[i] = image;
    IMAGE_COUNT++;

    //Using eigenfaces calculate a matching face
    min_face_i = eigenface_match(training_images, IMAGE_COUNT, eigen_faces, 3);

    //Display a sampling of eigenfaces
    for(int i = 0; i < 3; i++)
        displayImage(eigen_faces[i], "Eigen Face");

    //Display matching training face
    displayImage(training_images[min_face_i], "Matching face");

    return true;
}

int eigenface_match( Image faces[], const int IMAGE_COUNT, Image eigen_faces[], int e_count )
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
        weights[i] = new Matrix((IMAGE_COUNT-1),1);
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

        //Scale eigne faces to display
        eigenfaces[i]->Scale();
        eigenfaces[i]->Multiply(255.0);
        if( i < e_count)
            eigen_faces[i] = eigenfaces[i]->ToImage(faces[0].Height(),faces[0].Width());
    }
    Matrix Ut(Matrix::Transpose(U));

    //Calculate weights for each input image (including test)
    for( int i = 0; i < IMAGE_COUNT; i++)
    {
        Matrix temp = Matrix::Multiply(Ut,*mean_adjusted[i]);
        weights[i]->SetColumn(temp,0);
    }
    //Find the face with the closest response
    double min_mag = DBL_MAX;
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
/***************************************************************************//**
 * @author Hayden Waisanen
 *
 * @par Description:
 * Get files contained in directory
 * Source: http://stackoverflow.com/questions/612097/how-can-i-get-a-list-of-files-in-a-directory-using-c-or-c
 *
 * @param[in] image
 *
 * @returns bool
 *
 ******************************************************************************/
bool get_files_dir(std::string dir_str, strvec & image_files)
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dir_str.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if(ent->d_name[0] == '.')
                continue;
            image_files.push_back(ent->d_name);
        }
        closedir (dir);
    }
    else
    {
        perror ("");
        return false;
    }
    return true;
}

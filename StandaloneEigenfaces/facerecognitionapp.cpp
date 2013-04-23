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

/***************************************************************************//**
 * @author Hayden Waisanen
 *
 * @par Description:
 * Use eigenfaces to calculate the image in faces array from index 0 to
 * IMAGE_COUNT-2 closest to faces[IMAGE_COUNT-1].
 *
 * Algorithm translated from:
 *    http://blog.zabarauskas.com/eigenfaces-tutorial/
 *
 * @param[in] Image faces[] -- Testing face is at index IMAGE_COUNT-1
 *                          -- All other images are training
 * @param[in] const int IMAGE_COUNT -- number of images in faces[]
 * @param[in,out] Image eigen_faces[] -- a sampling of eigenfaces to be returned
 * @param[in] int e_count -- number of eigen_faces to return
 *
 * @returns int -- image with closest match to test image
 *
 ******************************************************************************/
int eigenface_match( Image faces[], const int IMAGE_COUNT, Image eigen_faces[], int e_count )
{
    //Allocate matrices to store images
    Matrix * face_matrices[IMAGE_COUNT]; //Column vector conversions of training images
    Matrix * mean_adjusted[IMAGE_COUNT]; //Training images less the mean face
    Matrix * eigenfaces[IMAGE_COUNT];    //Calculated eigenfaces
    Matrix * weights[IMAGE_COUNT];       //Matrix containing contributions of each eigenface
                                         //to the current face
    const int NUM_PIXELS = faces[0].Width() *faces[0].Height(); //Number of pixels in images
    Matrix mean_face(NUM_PIXELS,1);      //Average face
    Matrix A(NUM_PIXELS,IMAGE_COUNT-1);  //A matrix
    Matrix eig_vals(IMAGE_COUNT-1, 1);              //Eigen values matrix
    Matrix eig_vecs(IMAGE_COUNT-1, IMAGE_COUNT-1);  //Eigen vectors matrix
    Matrix U(NUM_PIXELS, IMAGE_COUNT-1); //U matrix

    //Dynamically allocate the matrices
    for( int i = 0; i < IMAGE_COUNT; i++)
    {
        //Allocate
        face_matrices[i] = new Matrix(NUM_PIXELS,1);
        mean_adjusted[i] = new Matrix(NUM_PIXELS,1);
        eigenfaces[i] = new Matrix(NUM_PIXELS,1);
        weights[i] = new Matrix((IMAGE_COUNT-1),1);

        //Set face matrices to inputted images (including test image)
        face_matrices[i]->SetFrom(faces[i]);
    }

    //Calculate the average face
    for(int i = 0; i < IMAGE_COUNT-1; i++)
    {
        mean_face.Add(*face_matrices[i]);
    }
    mean_face.Divide(IMAGE_COUNT-1);

    //Calculate A matrix
    for(int i = 0; i < IMAGE_COUNT; i++)
    {
        mean_adjusted[i]->Add(*face_matrices[i]);
        mean_adjusted[i]->Subtract(mean_face);
        if(i < IMAGE_COUNT-1)
            A.SetColumn(*mean_adjusted[i], i);
    }

    //Calculate L = At * A
    Matrix At(Matrix::Transpose(A));
    Matrix L = Matrix::Multiply(At,A);

    //Find the eigenvalues and vectors
    L.eigen(eig_vecs, eig_vals);

    //Create eigenfaces (U matrix)
    for( int i = 0; i < IMAGE_COUNT-1; i++)
    {
        //Eigen values for C = A * At can be calculated  by
        //multiplying eigenvectors from L by A
        Matrix temp(eig_vecs.GetCol(i));
        Matrix temp2(Matrix::Multiply(A, temp));

        //Store and normalize eigenfaces
        eigenfaces[i]->SetColumn(temp2 ,0);
        eigenfaces[i]->Normalize();
        U.SetColumn(*eigenfaces[i], i);

        //Scale e_count number of eigenfaces and convert to an image
        if( i < e_count)
        {
            eigenfaces[i]->Scale();
            eigenfaces[i]->Multiply(255.0);
            eigen_faces[i] = eigenfaces[i]->ToImage(faces[0].Height(),faces[0].Width());
        }
    }

    //Create transpose of U matrix
    Matrix Ut(Matrix::Transpose(U));

    //Calculate weights for each input image (including test)
    for( int i = 0; i < IMAGE_COUNT; i++)
    {
        Matrix temp = Matrix::Multiply(Ut,*mean_adjusted[i]);
        weights[i]->SetColumn(temp,0);
    }

    //Find the face with the closest response
    //(considering weights[i] to be a point in "facespace",
    //find the closest point, this is the training image with the closest match)
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
 * Source:
 * http://stackoverflow.com/questions/612097/how-can-i-get-a-list-of-files-in-a-directory-using-c-or-c
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
            //Ignore hidden files
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

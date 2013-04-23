/***************************************************************************//**
 * @file main.cpp
 ******************************************************************************/



/******************************************************************************
 * Includes
 ******************************************************************************/
#include <QtGui/QApplication>
#include "mainwindow.h"
#include "facerecognitionapp.h"



/***************************************************************************//**
 * @name main
 * @par Description
 * Starts app up that allows a user to perform facial recognition
 ******************************************************************************/
int main(int argc, char *argv[])
{
    //Creates a facial recognition app
    FaceRecognitionApp myApp(NULL);
    ImageApp app(argc, argv);

    //Adds the face recognition app to the main app
    app.AddActions(&myApp);

    //Starts up the app
    return app.Start();
}

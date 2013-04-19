#include <QtGui/QApplication>
#include "mainwindow.h"
#include "facerecognitionapp.h"

int main(int argc, char *argv[])
{
    FaceRecognitionApp myApp(NULL);
    ImageApp app(argc, argv);

    app.AddActions(&myApp);

    return app.Start();
}

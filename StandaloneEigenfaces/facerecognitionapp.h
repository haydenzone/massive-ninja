/***************************************************************************//**
 * @file facerecognitionapp.h
 ******************************************************************************/
#ifndef FACERECOGNITIONAPP_H
#define FACERECOGNITIONAPP_H

#include <qtimagelib.h>
#include <QObject>

class FaceRecognitionApp : public QObject
{
    Q_OBJECT
public:
    explicit FaceRecognitionApp(QObject *parent = 0);
    
signals:
    
public slots:
    bool Menu_FacialRecognition_Eigenfaces(Image &image);
    
};

#endif // FACERECOGNITIONAPP_H

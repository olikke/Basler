#pragma once

#include <QObject>
#include <pylon/ImageEventHandler.h>
#include <pylon/ImageFormatConverter.h>
#include <pylon/PylonImage.h>
#include "VideoTract/samplequeue.h"
#include "VideoTract/abstractsurface.h"
#include "../CPP/log.h"


namespace Pylon
{
    class CameraImageHandler : public QObject,public CImageEventHandler
    {
        Q_OBJECT
   signals:
        void NewSample(void* buffer, int Stride, int ImageSize);
        void NewSample2(QImage im);
        void Message(LoggerMsg lm,  QString message);
    private:
        bool isColoredCamera=false;
        SampleQueue* sampleQueue=nullptr;        
    public:
        CameraImageHandler(bool _isColoredCamera,  SampleQueue *_sampleQueue=nullptr,  QObject *parent = nullptr);
        void OnImageGrabbed( CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult);
        quint64 baslerError=0;
    };
}

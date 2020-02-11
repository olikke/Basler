#include "cameraimagehandler.h"

Pylon::CameraImageHandler::CameraImageHandler(bool _isColoredCamera,  SampleQueue *_sampleQueue,  QObject *parent):
    QObject(parent),
    isColoredCamera(_isColoredCamera),
    sampleQueue(_sampleQueue)
{

}

void Pylon::CameraImageHandler::OnImageGrabbed(Pylon::CInstantCamera &camera, const Pylon::CGrabResultPtr &ptrGrabResult)
{
    Q_UNUSED(camera)
    if (!ptrGrabResult->GrabSucceeded())
    {
        baslerError++;
        emit Message(LoggerMsg::Warning,QString("CIH: ошибка получения данных от камеры. Error=%1.").arg(QString::number(ptrGrabResult->GetErrorCode())));
        //ptrGrabResult->GetErrorDescription();
        return;
    }
    if (sampleQueue)
    {
        static quint64 BaslerCount=0;
        void *buffer=ptrGrabResult->GetBuffer();
        unsigned long imageSize=static_cast<unsigned long>(ptrGrabResult->GetImageSize());
        GstBuffer *buffer2;
        GstMapInfo map;
        buffer2 = gst_buffer_new_allocate(nullptr,imageSize,nullptr);
        gst_buffer_map (buffer2, &map, GST_MAP_WRITE);
        memcpy(map.data,buffer,imageSize);
        gst_buffer_unmap(buffer2,&map);
        sampleQueue->Enqueue(buffer2);
        BaslerCount++;
    } else
    {
        if (isColoredCamera)
        {
            void *buffer=ptrGrabResult->GetBuffer();
            int imageSize=static_cast<int>(ptrGrabResult->GetImageSize());
            size_t Stride;
            ptrGrabResult->GetStride(Stride);
            int stride2=static_cast<int>(Stride);
            emit NewSample(buffer,stride2,imageSize);
        } else
        {
            int width   = int(ptrGrabResult->GetWidth());
            int height  = int(ptrGrabResult->GetHeight());
            void *buffer= ptrGrabResult->GetBuffer();
            QImage img(static_cast<uchar*>(buffer), width, height, QImage::Format_Grayscale8);
            QImage img2=img.convertToFormat(QImage::Format_RGB32);
            emit NewSample2(img2);
        }
    }
}

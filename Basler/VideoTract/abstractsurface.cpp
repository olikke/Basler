#include "abstractsurface.h"

AbstractSurface::AbstractSurface(QObject *parent): QObject(parent),mSurface(nullptr)
{

}

void AbstractSurface::NewSample2(QImage im)
{
     QVideoFrame frame=QVideoFrame(im);
     if (!mSurface->present(frame))
     {
         qCritical("AbstractSurface::NewSample(). Can't present frame.");
     }
}

void  AbstractSurface::NewSample(void* buffer, int Stride, int ImageSize)
{
    QVideoFrame frame=QVideoFrame(ImageSize,VideoSize,Stride,QVideoFrame::PixelFormat::Format_UYVY);
    frame.map(QAbstractVideoBuffer::WriteOnly);
    memcpy(frame.bits(),buffer,ImageSize);
    frame.unmap();
    if (!mSurface->present(frame))
    {
        qCritical("AbstractSurface::NewSample(). Can't present frame.");
    }
}

void AbstractSurface::SetVideoSize(QSize sz)
{
    VideoSize=sz;
    initSurface();
   //    AllSuportedFormats();
}

void AbstractSurface::ClearSurface()
{
    if (mSurface && mSurface->isActive())
        mSurface->stop();
}

QAbstractVideoSurface* AbstractSurface::videoSurface()const
{
    return mSurface;
}

void AbstractSurface::setVideoSurface(QAbstractVideoSurface *s)
{
   if (mSurface==nullptr)
   {
       mSurface=s;
       return;
   }
   if (mSurface != s && mSurface && mSurface->isActive())
   {
       mSurface->stop();
   }
   mSurface=s;
    QVideoFrame mn ;

}

void AbstractSurface::CloseSurface()
{
    if (mSurface && mSurface->isActive())
        mSurface->stop();
}

void AbstractSurface::AllSuportedFormats()
{
    qDebug("Supported formats:");
    QList<QVideoFrame::PixelFormat> formats = mSurface->supportedPixelFormats();
    for (int i = 0; i < formats.length(); i++)
        qDebug()<<formats[i];
}

void AbstractSurface::initSurface()
{
    if (mSurface && mSurface->isActive())
        mSurface->stop();
    //именно формат кадра, а не поверхности
    QVideoSurfaceFormat format = QVideoSurfaceFormat(VideoSize,QVideoFrame::PixelFormat::Format_UYVY);
    if (!mSurface)
    {
        qDebug("AbstractSurface::initSurface !mSurface");
        return;
    }
    if (!mSurface->start(format))
    {
        qDebug("AbstractSurface::initSurface ! some problem");
        return;
    }
}

void AbstractSurface::RefreshSurface()
{
    initSurface();
}

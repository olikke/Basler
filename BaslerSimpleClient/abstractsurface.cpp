#include "abstractsurface.h"

AbstractSurface::AbstractSurface(QObject *parent): QObject(parent),mSurface(nullptr)
{

}

void AbstractSurface::NewSample()
{
        if (!mSampleQueue | (!mSurface && mSurface->isActive() )) return;
        if (mSampleQueue->CountQueue()>0)
        {
            QImage img = *mSampleQueue->dequeue().get();

            QVideoFrame frame=QVideoFrame(img);
            mSurface->present(frame);
        }
}

void AbstractSurface::setSampleQueue(sampleQueue *_SampleQueue)
{
    mSampleQueue=_SampleQueue;
}

void AbstractSurface::SetVideoSize(QSize sz)
{
    VideoSize=sz;
    initSurface();
   //qDebug("AbstractSurface::SetVideoSize succ %d %d",VideoSize.width(),VideoSize.height());
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
}

void AbstractSurface::CloseSurface()
{
    if (mSurface && mSurface->isActive())
        mSurface->stop();
}

void AbstractSurface::AllSuportedFormats()
{
    printf("Supported formats:");
    QList<QVideoFrame::PixelFormat> formats = mSurface->supportedPixelFormats();
    for (int i = 0; i < formats.length(); i++)
        printf(" %d", formats[i]);
    printf("\n");
}

void AbstractSurface::initSurface()
{
    if (mSurface && mSurface->isActive())
        mSurface->stop();
    //именно формат кадра, а не поверхности
    QVideoSurfaceFormat format = QVideoSurfaceFormat(VideoSize,QVideoFrame::PixelFormat::Format_RGB32);
    if (!mSurface)
    {
        qDebug("AbstractSurface::SetVideoSize !mSurface");
        return;
    }
    mSurface->start(format);
}

void AbstractSurface::RefreshSurface()
{
    initSurface();
}

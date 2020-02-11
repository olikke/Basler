#include "samplequeue.h"

SampleQueue::SampleQueue(QObject *parent):
    QObject(parent),
    mMutex(new QMutex()),
    MaxSampleCount(5)
{
}

void SampleQueue::enqueue(GstBuffer * value)
{
    {
        QMutexLocker locker(mMutex);
        mQueue.enqueue(value);
    }
   emit NewSample();
}

void SampleQueue::Enqueue(GstBuffer * value)
{    
    if (mQueue.size()<MaxSampleCount)
    {
        enqueue(value);
    }
    else
    {
        //если очередь переполняется, тупо выкидываем самый старый обьект в очереди
        if (!mQueue.empty())
        {
            GstBuffer * value=mQueue.dequeue();
            gst_buffer_unref (value);
        }
        qDebug("RemoveFirst");
        enqueue(value);
    }
}

GstBuffer * SampleQueue::dequeue()
{
    GstBuffer * value;
    QMutexLocker locker(mMutex);
    if (!mQueue.empty())
    {
        value=mQueue.dequeue();
    } else value=nullptr;
    return value;
}

void SampleQueue::Clear()
{
    QMutexLocker locker(mMutex);
    mQueue.clear();
}

int SampleQueue::CountQueue()
{
    int value;
   QMutexLocker locker(mMutex);
    value=mQueue.count();
    return value;
}

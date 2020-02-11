#include "samplequeue.h"

sampleQueue::sampleQueue(QObject *parent):
    QObject(parent),
    mMutex(new QMutex()),
    MaxSampleCount(5)
{

}

void sampleQueue::enqueue(std::shared_ptr<QImage> value)
{
    mMutex->lock();
    mQueue.enqueue(std::move(value));
    mMutex->unlock();
}

void sampleQueue::Enqueue(std::shared_ptr<QImage> value)
{    
    if (mQueue.size()<MaxSampleCount)
    {
        enqueue(value);
    }
    else
    {
        //если очередь переполняется, тупо выкидываем самый старый обьект в очереди
        if (!mQueue.empty())  mQueue.removeFirst();
        qDebug("RemoveFirst");
        enqueue(value);
    }
    emit NewSample();
}

std::shared_ptr<QImage> sampleQueue::dequeue()
{
    std::shared_ptr<QImage> value;
    mMutex->lock();
    if (!mQueue.empty())
    {
        value=mQueue.dequeue();
    } else
    {
        value=nullptr;
    }
    mMutex->unlock();
    return value;
}

void sampleQueue::Clear()
{
    mMutex->lock();
    mQueue.clear();
    mMutex->unlock();
}

int sampleQueue::CountQueue()
{
    int value;
    mMutex->lock();
    value=mQueue.count();
    mMutex->unlock();
    return value;
}

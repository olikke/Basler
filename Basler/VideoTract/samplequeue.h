#pragma once

#include <QObject>
#include <QMutex>
#include <QQueue>
#include <QImage>
#include <iostream>
#include <memory>
#include <QDebug>
#include <gst/gst.h>

class SampleQueue : public QObject
{
    Q_OBJECT
private:
    QMutex* mMutex;
    int MaxSampleCount;
    void enqueue(GstBuffer * value);
protected:
    QQueue<GstBuffer *> mQueue;
public:
    explicit SampleQueue(QObject *parent = nullptr);
    void Enqueue(GstBuffer * value);
    GstBuffer * dequeue();
    void Clear();
    int CountQueue();
signals:
    void NewSample();
public slots:
};



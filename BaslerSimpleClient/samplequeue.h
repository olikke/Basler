#pragma once

#include <QObject>
#include <QMutex>
#include <QQueue>
#include <QImage>
#include <iostream>
#include <memory>
#include <QDebug>

class sampleQueue : public QObject
{
    Q_OBJECT
private:
    QMutex* mMutex;
    int MaxSampleCount;
    void enqueue(std::shared_ptr<QImage> value);
protected:
    QQueue<std::shared_ptr<QImage>> mQueue;
public:
    explicit sampleQueue(QObject *parent = nullptr);
    void Enqueue(std::shared_ptr<QImage> value);
    std::shared_ptr<QImage> dequeue();
    void Clear();
    int CountQueue();
signals:
    void NewSample();
public slots:
};


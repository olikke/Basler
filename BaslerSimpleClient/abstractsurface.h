#pragma once

#include <QObject>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QImage>
#include <QSize>
#include <QDebug>
#include <memory>
#include "samplequeue.h"

class AbstractSurface : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSurface(QObject *parent = nullptr);
    Q_PROPERTY(QAbstractVideoSurface* videoSurface READ videoSurface WRITE setVideoSurface )
    QAbstractVideoSurface* videoSurface() const;
    void setVideoSurface(QAbstractVideoSurface* s );
    void setSampleQueue(sampleQueue* _SampleQueue);
public slots:
    void ClearSurface();
    void RefreshSurface();
    void SetVideoSize(QSize sz);
    void NewSample();


private:
    QAbstractVideoSurface* mSurface;
    void AllSuportedFormats();
    void CloseSurface();
    QSize VideoSize;
    void initSurface();
    sampleQueue* mSampleQueue;
};


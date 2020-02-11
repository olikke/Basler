#pragma once

#include <QObject>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QSize>
#include <QDebug>

class AbstractSurface : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSurface(QObject *parent = nullptr);
    Q_PROPERTY(QAbstractVideoSurface* videoSurface READ videoSurface WRITE setVideoSurface )
    QAbstractVideoSurface* videoSurface() const;
    void setVideoSurface(QAbstractVideoSurface* s );
public slots:
    void ClearSurface();
    void RefreshSurface();
    void SetVideoSize(QSize sz);
    void NewSample(void* buffer, int Stride, int ImageSize);
    void NewSample2(QImage im);
private:
    QAbstractVideoSurface* mSurface;
    void AllSuportedFormats();
    void CloseSurface();
    QSize VideoSize;
    void initSurface();
};

#pragma once

#include <QObject>
#include <QDebug>
#include <gst/gst.h>
#include <chrono>
#include <gst/app/gstappsink.h>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include "samplequeue.h"

class GstPlayer : public QObject
{
    Q_OBJECT
public:
    explicit GstPlayer(QObject *parent = nullptr);
    void SetSize(QSize sz) ;
    void Prepare(QString multicastIP, int multicastPort) ;
    void Stop();
    void setSampleQueue(sampleQueue* _sampleQueue);
    void getFrameRate(unsigned long &sampleCount, double &FrameRate);
signals:
    void GetSize(QSize sz);
    void NewSample(std::shared_ptr<QVideoFrame> frame);
public slots:


protected:
    friend GstFlowReturn new_sample(GstAppSink *appsink, gpointer data);

private:
    bool prepare();
    GstElement*  createElement(const gchar *name);

    GstElement* Pipeline;
    GstElement* UdpSrc;
    GstElement* RtpJpegDepay;
    GstElement* JpegDec;
    GstElement* AppSink;
    GstElement* Caps;
    GstElement* Convert;

    bool NeedToInitialize;

    sampleQueue* mSampleQueue;
    QSize Size;

    QString MulticastIP;
    int MulticastPort;

    unsigned long SampleCount;
    unsigned long RememberCount;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};


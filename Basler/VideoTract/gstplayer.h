#pragma once
#include <QObject>
#include <QDebug>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/gstutils.h>
#include "samplequeue.h"
#include <gst/gstdebugutils.h>
#include "../CPP/global.h"
#include "../CPP/log.h"

#define Debug_Config true;

class GstPlayer: public QObject
{
    Q_OBJECT
public:
    GstPlayer(OneNode _fullServer, OneNode _halfServer, QObject *parent = nullptr);
    bool play();
    void stop();
    bool prepare();
    void getStatistic(quint64 & FullServerTotalBytes, quint64 & HalfServerTotalBytes, quint64 &TotalSampleCount);
signals:
    void Message(LoggerMsg lm,  QString message);
public slots:
    void newSample();
    void changeSize(int _width, int _height, bool _colorCamera);
private:
    unsigned long sampleCount;

    GstAppSrc*  AppSrc=nullptr;
    GstElement* Tee=nullptr;
    GstElement* Pipeline=nullptr;

    GstElement* QueueFull=nullptr;
    GstElement* UdpSinkFull=nullptr;
    GstElement* JpegEncFull=nullptr;
    GstElement* RtpJpegPayFull=nullptr;
    GstElement* VideoConvert=nullptr;

    GstElement* QueueHalf=nullptr;
    GstElement* CapsHalf=nullptr;
    GstElement* VideoScale=nullptr;
    GstElement* UdpSinkHalf=nullptr;
    GstElement* JpegEncHalf=nullptr;
    GstElement* RtpJpegPayHalf=nullptr;

    void pushBufferInGst(GstBuffer *buffer);
    GstElement*  createElement(const gchar *name);

    OneNode fullServer;
    OneNode halfServer;

    int width,height;
    bool pipelineReady;
    bool first;
    bool isColorCamera=true;
};


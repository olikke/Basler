#pragma once

#include <QDebug>
#include <QDataStream>
#include "../CPP/global.h"

const quint8 MODE_UNDEFINED=255;

const double TEMPERATURE_YELLOW = 65.;
//if temperature of Basler Board>=78, video stream will stop
const double TEMPERATURE_RED = 72.;

struct VideoServerStatistic
{
    bool    CameraDE = false;
    quint64 CameraSN = 0;
    double  CameraTemperature = 0;
    quint8  CurrentMode = MODE_UNDEFINED;
    quint64 TotalFrame=0;
    double  FramePerSec=0;
    quint64 FailedFrame=0;
    quint64 FullTotalFrame;
    double  FullFramePerSec;
    double  FullMBPerSec;
    quint64 HalfTotalFrame;
    double  HalfFramePerSec;
    double  HalfMBPerSec;
    friend QDebug operator<<(QDebug dbg, const VideoServerStatistic *v);
    friend QDataStream & operator << (QDataStream & ds, VideoServerStatistic &v);
    friend QDataStream & operator >> (QDataStream & ds, VideoServerStatistic &v);
    DeviceState state();
};



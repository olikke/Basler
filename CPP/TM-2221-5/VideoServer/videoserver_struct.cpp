#include "videoserver_struct.h"


QDebug operator<<(QDebug dbg, const VideoServerStatistic *v)
{
    dbg.nospace() <<"\n";
    dbg.nospace()<<"CameraSN"<<v->CameraSN<<"\t CameraTemperature "<<v->CameraTemperature<<"\n";
    dbg.nospace()<<"Camera\tTotalFrame "<<v->TotalFrame<<"\t FrameRate "<<v->FramePerSec<<"\t FailedFrame "<<v->FailedFrame<<"\n";
    dbg.nospace()<<"FullServer\tTotalFrame "<<v->FullTotalFrame<<"\t FrameRate "<<v->FullFramePerSec<<"\t MB/sec "<<v->FullMBPerSec<<"\n";
    dbg.nospace()<<"HalfServer\tTotalFrame "<<v->HalfTotalFrame<<"\t FrameRate "<<v->HalfFramePerSec<<"\t MB/sec "<<v->HalfMBPerSec<<"\n";
    return  dbg.space();
}

QDataStream &operator <<(QDataStream &ds, VideoServerStatistic &v)
{
    SetSingleRulesForDS(ds);
    ds<<v.CameraDE<<v.CameraSN<<v.CameraTemperature<<v.CurrentMode;
    ds<<v.TotalFrame<<v.FramePerSec<<v.FailedFrame;
    ds<<v.FullTotalFrame<<v.FullFramePerSec<<v.FullMBPerSec;
    ds<<v.HalfTotalFrame<<v.HalfFramePerSec<<v.HalfMBPerSec;
    return ds;
}

QDataStream &operator >>(QDataStream &ds, VideoServerStatistic &v)
{
    SetSingleRulesForDS(ds);
    ds>>v.CameraDE>>v.CameraSN>>v.CameraTemperature>>v.CurrentMode;
    ds>>v.TotalFrame>>v.FramePerSec>>v.FailedFrame;
    ds>>v.FullTotalFrame>>v.FullFramePerSec>>v.FullMBPerSec;
    ds>>v.HalfTotalFrame>>v.HalfFramePerSec>>v.HalfMBPerSec;
    return ds;
}


DeviceState VideoServerStatistic::state()
{
    if (!CameraDE) return DeviceState::Red;
    if (CameraTemperature>=TEMPERATURE_RED) return DeviceState::Red;
    if (CameraTemperature>=TEMPERATURE_YELLOW) return DeviceState::Yellow;
    return DeviceState::Green;
}

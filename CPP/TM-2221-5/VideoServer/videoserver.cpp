#include "videoserver.h"

VideoServer::VideoServer(QObject *parent) : QObject(parent),
    vsStruct(new VideoServerStatistic)
{

}

int VideoServer::getTemperatureColor()
{
    if (vsStruct->CameraTemperature<TEMPERATURE_YELLOW) return 0;
    if (vsStruct->CameraTemperature<TEMPERATURE_RED) return 1;
    return 2;
}

QDataStream &operator >>(QDataStream &ds, VideoServer &v)
{
    ds>>*v.vsStruct;
    //Uniqueness of this module - data is changing always
    //so we dont need save previous data and check with it - always emit all data emit :)
    v.emitAll();
    return ds;
}

void VideoServer::emitAll()
{
    if (remDataExh!=vsStruct->CameraDE)
    {
        remDataExh=vsStruct->CameraDE;
        emit dataExhChanged();
        return;
    }
    if (vsStruct->CameraSN!=remSerialNumber)
    {
        emit serialNumberChanged();
        //отследить изменение серийного номера в лог
        remSerialNumber=vsStruct->CameraSN;
    }
    if (vsStruct->CurrentMode!=remMode)
    {
        emit modeChanged();
        remMode=vsStruct->CurrentMode;
    }
    if (!parse) return;
    emit temperatureChanged();
    emit totalFrameChanged();
    emit framePerSecChanged();
    emit failedFrameChanged();
    emit fullTotalFrameChanged();
    emit fullFramePerSecChanged();
    emit fullMBPerSecChanged();
    emit halfTotalFrameChanged();
    emit halfFramePerSecChanged();
    emit halfMBPerSecChanged();
}


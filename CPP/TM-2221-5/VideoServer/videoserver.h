#pragma once

#include <QObject>
#include <chrono>
#include "videoserver_struct.h"

class VideoServer : public QObject
{
    Q_OBJECT
public:
    explicit VideoServer(QObject *parent = nullptr);

    Q_PROPERTY(int mode READ getMode NOTIFY modeChanged)
    int getMode() {return static_cast<int>(vsStruct->CurrentMode);}

    Q_PROPERTY(bool dataExh READ getDataExh NOTIFY dataExhChanged)
    bool getDataExh() {return vsStruct->CameraDE;}

    Q_PROPERTY(int serialNumber READ getSerialNumber NOTIFY serialNumberChanged)
    int getSerialNumber() {return vsStruct->CameraSN;}

    Q_PROPERTY(double temperature READ getTemperature  NOTIFY temperatureChanged)
    double getTemperature() {return vsStruct->CameraTemperature;}

    Q_PROPERTY(int totalFrame READ getTotalFrame NOTIFY totalFrameChanged)
    int getTotalFrame() {return vsStruct->TotalFrame;}

    Q_PROPERTY(double framePerSec READ getFramePerSec NOTIFY framePerSecChanged)
    double getFramePerSec() {return vsStruct->FramePerSec;}

    Q_PROPERTY(int failedFrame READ getFailedFrame NOTIFY failedFrameChanged)
    int getFailedFrame() {return vsStruct->FailedFrame;}

    Q_PROPERTY(int fullTotalFrame READ getFullTotalFrame NOTIFY fullTotalFrameChanged)
    int getFullTotalFrame() {return vsStruct->FullTotalFrame;}

    Q_PROPERTY(double fullFramePerSec READ getFullFramePerSec NOTIFY fullFramePerSecChanged)
    double getFullFramePerSec() {return vsStruct->FullFramePerSec;}

    Q_PROPERTY(double fullMBPerSec READ getFullMBPerSec NOTIFY fullMBPerSecChanged)
    double getFullMBPerSec() {return vsStruct->FullMBPerSec;}

    // === fullFramePerSec
    Q_PROPERTY(int halfTotalFrame READ getHalfTotalFrame NOTIFY halfTotalFrameChanged)
    int getHalfTotalFrame() {return vsStruct->HalfTotalFrame;}

    // === fullFramePerSec
    Q_PROPERTY(double halfFramePerSec READ getHalfFramePerSec NOTIFY halfFramePerSecChanged)
    double getHalfFramePerSec() {return vsStruct->HalfFramePerSec;}

    Q_PROPERTY(double halfMBPerSec READ getHalfMBPerSec NOTIFY halfMBPerSecChanged)
    double getHalfMBPerSec() {return vsStruct->HalfMBPerSec;}

    //чтобы запустить парс данных для вывода на форму
    Q_INVOKABLE void startParse(const bool p) {parse=p;}

    Q_INVOKABLE int getTemperatureColor();

    friend QDataStream & operator >> (QDataStream & ds, VideoServer &v);

signals:
    void modeChanged();
    void dataExhChanged();
    void serialNumberChanged();
    void temperatureChanged();
    void totalFrameChanged();
    void framePerSecChanged();
    void failedFrameChanged();
    void fullTotalFrameChanged();
    void fullFramePerSecChanged();
    void fullMBPerSecChanged();
    void halfTotalFrameChanged();
    void halfFramePerSecChanged();
    void halfMBPerSecChanged();
private:
    VideoServerStatistic* vsStruct;
    bool parse=false;
    quint64 remSerialNumber = 0;
    bool remDataExh = false;
    quint8 remMode=MODE_UNDEFINED;
    void emitAll();
};


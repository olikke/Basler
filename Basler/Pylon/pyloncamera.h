#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QThread>
#include <QtConcurrent>
#include <pylon/PylonIncludes.h>
#include <pylon/PylonBase.h>
#include <pylon/Device.h>
#include <pylon/gige/BaslerGigEInstantCamera.h>
#include <GenApi/GenApiNamespace.h>
#include <GenApi/IEnumeration.h>
#include <pylon/FeaturePersistence.h>
#include <Base/GCString.h>
#include <GenApi/INodeMap.h>
#include <QApplication>
#include <QTimer>
#include <QDir>
#include <QDataStream>
#include "cameraevents.h"
#include "cameraimagehandler.h"
#include "VideoTract/samplequeue.h"
#include "VideoTract/abstractsurface.h"
#include "pylonfeaturesenum.h"
#include "string"
#include "VideoTract/gstplayer.h"
#include <math.h>
#include "settings.h"
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>
#include <QFutureWatcher>

#include "../CPP/log.h"
#include "../CPP/TM-2221-5/VideoServer/videoserver_struct.h"

const int MTU_SIZE = 9000;

class PylonCamera : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList devices READ Devices NOTIFY devicesChanged)
    QStringList Devices();

    Q_PROPERTY(AbstractSurface* abs MEMBER abstractSurface CONSTANT)

    Q_PROPERTY(NameForPylon* nameSpace MEMBER nameSpace CONSTANT)

    Q_PROPERTY(bool gammaEnable READ getGammaEnable WRITE setGammaEnable)
    void setGammaEnable(bool _GammaEnable);
    bool getGammaEnable();

    Q_PROPERTY(bool rateControl READ getRateControl WRITE setRateControl)
    void setRateControl(bool _RateControl);
    bool getRateControl();

    Q_PROPERTY(bool xCenter READ getXCenter WRITE setXCenter)
    void setXCenter(bool _XCenter);
    bool getXCenter();

    Q_PROPERTY(bool yCenter READ getYCenter WRITE setYCenter)
    void setYCenter(bool _YCenter);
    bool getYCenter();

    Q_PROPERTY(bool opening MEMBER isCameraOpening NOTIFY openingChanged)

    Q_PROPERTY(bool playing MEMBER isCameraPlaying NOTIFY playingChanged)
    void setPlaying(bool _PylonPlaying);

    Q_PROPERTY(int choice MEMBER cameraNumber WRITE setChoice  NOTIFY choiceChanged)
    void setChoice(int Index);
public:
    PylonCamera(Settings* _settings, Log* _log, QObject *parent = nullptr);
    ~PylonCamera();

    //Function Control
    Q_INVOKABLE void open(bool m);
    Q_INVOKABLE void loadFeatures(int numb);
    Q_INVOKABLE void saveFeatures(int numb);
    Q_INVOKABLE void enumCameras();
    Q_INVOKABLE void startVideo(bool m);

    Q_INVOKABLE QList<int>  getIntPtrFeatures(int numb);
    Q_INVOKABLE bool            setIntPtrFeatures(int numb,int value);

    Q_INVOKABLE QList<double>   getFloatFeatures(int numb);
    Q_INVOKABLE bool                    setFloatFeatures(int numb, double value);

    Q_INVOKABLE int               getEnumCurrentValueToInt(int numb);
    Q_INVOKABLE bool            setEnumFeatures(int numb, int value);

    VideoServerStatistic* getVideoServer() {return videoServer;}


signals:
    void devicesChanged();
    void openingChanged(bool);
    void choiceChanged(int i);
    void playingChanged(bool);
    void Message(LoggerMsg lm,  QString message);
public slots:
    void pylonOpened(bool m);
    void pylonRemoved();
    void getStat();
    void command(QByteArray & comm);
    void loadFeaturesFinished();
    void saveFeaturesFinished();
private:
    void Open();
    void StartVideo();
    void StopVideo();
    void PylonClose();
    void SaveFeatures();
    void UpdateNodeMap();
    bool IsColorCamera();
    double calcValueGrowPerSec(quint64 &CurrentValue, quint64 &RemeberValue, quint64 & TimeIntervalMilli);
    Log* log;
    SampleQueue* sampleQueueForRTPServer=nullptr;
    Pylon::CBaslerGigEInstantCamera *camera=nullptr;
    AbstractSurface* abstractSurface=nullptr;
    NameForPylon* nameSpace=nullptr;
    Pylon::CameraImageHandler *cameraImageHandler=nullptr;
    Pylon::CameraEventPrinter* cameraEventPrinter=nullptr;
    QStringList devices;
    Pylon::DeviceInfoList_t lstDevices;
    GstPlayer* player=nullptr;
    QTimer *timer=nullptr;
    Settings* settings=nullptr;
    VideoServerStatistic* videoServer = nullptr;
    quint64 SerialNumber;
    QFutureWatcher<QPair<QString, int>> loadWatcher,saveWatcher;
    int cameraNumber;
    bool isCameraOpening;
    bool isCameraPlaying;
    bool isGuiNeed;

    bool getIntPtrFeatures(GenICam::gcstring &name,QList<int>&L);
    bool setIntPtrFeatures(GenICam::gcstring &name,int & value);

    bool getFloatFeatures(GenICam::gcstring &name,QList<double>&L);
    bool setFloatFeatures(GenICam::gcstring &name,double & value);


    bool getEnumFeatures(GenICam::gcstring &name,QStringList &Result);
    bool setEnumFeatures(GenICam::gcstring &name, int & value, QString & valToStr);

    bool getStringFeatures(GenICam::gcstring &name, QString & val);

    void findCamera();

    double getTemperatureABS();
};


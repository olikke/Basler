#include "pyloncamera.h"

using namespace Pylon;
using namespace GenApi;

PylonCamera::PylonCamera(Settings* _settings, Log* _log, QObject *parent) : QObject(parent),
      log(_log),
      devices{""},
      settings(_settings),
      videoServer(new VideoServerStatistic),
      cameraNumber(-1),
      isCameraOpening(false),
      isCameraPlaying(false),
      isGuiNeed(_settings->getNeedGUI())
{
    connect(this,&PylonCamera::Message,log,&Log::Message);
    connect(&loadWatcher,&QFutureWatcher<QString>::finished,this,&PylonCamera::loadFeaturesFinished);
    connect(&saveWatcher,&QFutureWatcher<QString>::finished,this,&PylonCamera::saveFeaturesFinished);

    PylonInitialize();
    nameSpace=new NameForPylon();
    lstDevices.clear();

    if (isGuiNeed)
    {
        emit Message(LoggerMsg::Info,"Загрузка приложения в оконном режиме.");
        abstractSurface=new AbstractSurface(this);
    }
    else
    {
        emit Message(LoggerMsg::Info,"Загрузка...");
        sampleQueueForRTPServer=new SampleQueue(this);
        player= new GstPlayer(settings->getFullServer(), settings->getHalfServer(),  this);
        emit Message(LoggerMsg::Info,"Full Server: "+settings->getFullServer().toString());
        emit Message(LoggerMsg::Info,"Half Server: "+settings->getHalfServer().toString());
        connect(sampleQueueForRTPServer, &SampleQueue::NewSample, player, &GstPlayer::newSample);
        connect(player,&GstPlayer::Message,this,&PylonCamera::Message);
        player->prepare();
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this,&PylonCamera::findCamera);
        timer->start(1000);
    }
}

void PylonCamera::findCamera()
{
    enumCameras();
    if (cameraNumber==0)
    {
        open(true);
    }
}

void PylonCamera::enumCameras()
{
    try
    {
        CTlFactory& TlFactory = CTlFactory::GetInstance();
        TlFactory.EnumerateDevices( lstDevices );
        devices.clear();
        cameraNumber=-1;
        if (lstDevices.empty())
        {
            emit Message(LoggerMsg::Warning,"BSL: камера не найдена");
            emit devicesChanged();
            return;
        }
        DeviceInfoList_t::const_iterator it;
        for (it = lstDevices.begin(); it != lstDevices.end(); ++it)
        {            
            //only serialNumber
            devices.push_back(QString().sprintf("№ %s",it->GetSerialNumber().c_str()));
            //devices.push_back(QString().sprintf("%s (%s)",it->GetModelName().c_str(),it->GetSerialNumber().c_str()));
        }
        if (!lstDevices.empty())
        {
            cameraNumber=0;
            emit Message(LoggerMsg::Info,QString("BSL: камера найдена: "+ QString().sprintf("%s (%s)", lstDevices.begin()->GetModelName().c_str(), lstDevices.begin()->GetSerialNumber().c_str())));
        }
        emit devicesChanged();
    }
    catch (GenICam::GenericException &e)
    {
        emit Message(LoggerMsg::Warning,QString("BSL: Ошибка при поиске камеры %1").arg(e.GetDescription()));
    }
}

void PylonCamera::startVideo(bool m)
{
    if (m)  StartVideo();
    else StopVideo();
}

void PylonCamera::StopVideo()
{
   try {
        if (!camera->IsCameraDeviceRemoved()) camera->StopGrabbing();
        camera->DeregisterConfiguration(cameraEventPrinter);
        camera->DeregisterImageEventHandler(cameraImageHandler);
    }
    catch (RuntimeException ex)
    {

        emit Message(LoggerMsg::Warning,QString("BSL: ошибка остановки камеры: %1").arg(ex.what()));
    }
    isCameraPlaying=false;
    emit playingChanged(isCameraPlaying);
}

void PylonCamera::getStat()
{
    static quint64 deliver=0;    
    //1 time per sec - enougth
    if (deliver++ % 2 !=0) return;
    quint64 CameraFailedFrame, CameraSampleCount;
    quint64 FullTotalBytes, HalfTotalBytes, PlayerSampleCount;
    if (camera)
    {
        videoServer->CameraDE=true;
        videoServer->CameraSN=SerialNumber;
        videoServer->CameraTemperature=getTemperatureABS();
        try{
            CameraFailedFrame = camera->GetStreamGrabberParams().Statistic_Failed_Buffer_Count.GetValue();
            CameraSampleCount=camera->GetStreamGrabberParams().Statistic_Total_Buffer_Count.GetValue();
        }
        catch (GenICam_3_1_Basler_pylon_v5_1::RuntimeException ex)
        {
            qWarning()<<"olikke good error"<<ex.what();
        }
    }
    else
    {
        videoServer->CameraDE=false;
        videoServer->CameraSN=0;
        videoServer->CameraTemperature=0;
        CameraFailedFrame=0; CameraSampleCount=0;
    }
    if (player)
    {
        player->getStatistic(FullTotalBytes, HalfTotalBytes, PlayerSampleCount);
    }
    else
    {
        FullTotalBytes=0; HalfTotalBytes=0; PlayerSampleCount=0;
    }
    static std::chrono::high_resolution_clock::time_point timestamp;
    quint64 timeInterval_milli=getMilliSecSince(timestamp);
    getTime(timestamp);
    if (timeInterval_milli==0) return;
    static quint64 rem_CameraSampleCount=0;
    static quint64 rem_FullTotalBytes=0, rem_HalfTotalBytes=0, rem_PlayerSampleCount=0;

    videoServer->TotalFrame=CameraSampleCount;
    videoServer->FramePerSec=calcValueGrowPerSec(CameraSampleCount,rem_CameraSampleCount,timeInterval_milli);
    videoServer->FailedFrame=CameraFailedFrame;

    videoServer->FullTotalFrame=PlayerSampleCount;
    videoServer->FullFramePerSec=calcValueGrowPerSec(PlayerSampleCount,rem_PlayerSampleCount,timeInterval_milli);
    videoServer->FullMBPerSec=calcValueGrowPerSec(FullTotalBytes,rem_FullTotalBytes,timeInterval_milli)/1024./1024.;

    videoServer->HalfTotalFrame=videoServer->FullTotalFrame;
    videoServer->HalfFramePerSec=videoServer->FullFramePerSec;
    videoServer->HalfMBPerSec=calcValueGrowPerSec(HalfTotalBytes,rem_HalfTotalBytes,timeInterval_milli)/1024./1024.;
}

void PylonCamera::command(QByteArray &comm)
{
    if (comm.size()!=1) return;
    char mode=comm.at(0);
    if (mode>2) return;
    loadFeatures(mode);
}

bool PylonCamera::IsColorCamera()
{
    QStringList list;
    GenICam::gcstring  temp=QString("PixelFormat").toUtf8().constData();
    getEnumFeatures(temp,list);
    for (auto it:list)
    {
        if  (it.indexOf("Bayer")!=-1)
            return true;
    }
    return false;
}

double PylonCamera::calcValueGrowPerSec(quint64 &CurrentValue, quint64 &RemeberValue, quint64 &TimeIntervalMilli)
{
    double temp=static_cast<double>(CurrentValue-RemeberValue)/TimeIntervalMilli*1000.;
    RemeberValue=CurrentValue;
    return temp;
}

void PylonCamera::StartVideo()
{
    if (!camera || !camera->IsOpen()) Open();
    try
    {
        bool isCameraColor=IsColorCamera();
        cameraImageHandler=new CameraImageHandler(isCameraColor,sampleQueueForRTPServer);
        QThread* thread = new QThread();
        cameraImageHandler->moveToThread(thread);
        if (isGuiNeed)
        {
            connect(cameraImageHandler,&CameraImageHandler::NewSample,abstractSurface,&AbstractSurface::NewSample);
            connect(cameraImageHandler,&CameraImageHandler::NewSample2,abstractSurface,&AbstractSurface::NewSample2);
        }
        camera->RegisterImageEventHandler(cameraImageHandler, RegistrationMode_Append, Cleanup_Delete);
        camera->MaxNumBuffer = 5;
        if (isCameraColor)
        {
            camera->PixelFormat.SetValue(Basler_GigECamera::PixelFormatEnums::PixelFormat_YUV422Packed);
        } else
        {
            camera->PixelFormat.SetValue(Basler_GigECamera::PixelFormatEnums::PixelFormat_Mono8);
        }
        camera->Height.SetValue(VIDEO_HEIGHT);
        camera->Width.SetValue(VIDEO_WIDTH);
        if (player) player->changeSize(static_cast<int>(camera->Width()),static_cast<int>(camera->Height()),isCameraColor);
        if (abstractSurface) abstractSurface->SetVideoSize(QSize(static_cast<int>(camera->Width()),static_cast<int>(camera->Height())));
        loadFeatures(0);
        //very important change default MTU for Balser and PC eth interface
       // короче не на всех компах, видимо. ПРОВЕРИТЬ
        camera->GevStreamChannelSelector.SetValue(Basler_GigECamera::GevStreamChannelSelector_StreamChannel0);
        camera->GevSCPSPacketSize.SetValue(MTU_SIZE);
    }
    catch (GenICam::GenericException &e)
    {
        emit Message(LoggerMsg::Fatal, QString("BSL: PylonCamera::StartVideo(). Ошибка камеры: %1").arg(e.GetDescription()));
        return;
    }
    if (camera->CanWaitForFrameTriggerReady())
    {
        camera->StartGrabbing(GrabStrategy_OneByOne, GrabLoop_ProvidedByInstantCamera);
        emit Message(LoggerMsg::Info, "BSL: Камера подключена успешно.");
    } else
    {
        emit Message(LoggerMsg::Warning, "BSL: Камера не работает в триггерном режиме.");
        return;
    }
    isCameraPlaying=true;
    if (isGuiNeed)   emit playingChanged(isCameraPlaying);
}

void PylonCamera::pylonOpened(bool m)
{
    isCameraOpening=m;
    if (!isGuiNeed) if (timer && m)
    {
        timer->stop();
    }
    if (isGuiNeed)  emit openingChanged(isCameraOpening);
    else if (m) StartVideo();
    if (m) loadFeatures(0);
}

void PylonCamera::Open()
{
   if (lstDevices.empty()) return;
   if (cameraNumber==-1) return;
   if (cameraNumber>int(lstDevices.size())) return;
   try
   {
       camera=new CBaslerGigEInstantCamera(CTlFactory::GetInstance().CreateDevice(lstDevices[size_t(cameraNumber)]));
       cameraEventPrinter=new CameraEventPrinter();
       connect(cameraEventPrinter,&CameraEventPrinter::cameraOpenedChanged,this,&PylonCamera::pylonOpened);
       connect(cameraEventPrinter,&CameraEventPrinter::cameraRemovedChanged,this,&PylonCamera::pylonRemoved);
       connect(cameraEventPrinter,&CameraEventPrinter::Message,this,&PylonCamera::Message);
       camera->RegisterConfiguration(cameraEventPrinter, RegistrationMode_ReplaceAll, Cleanup_Delete);
       camera->Open();
       SerialNumber= QString(lstDevices[cameraNumber].GetSerialNumber()).toULongLong();
       //  PrintAllCameraNodes();
   } catch (GenICam::GenericException &e)
   {
       emit Message(LoggerMsg::Fatal, QString("BSL: PylonCamera::PylonOpen(). Ошибка камеры: %1").arg(e.GetDescription()));
       if (camera) PylonClose();
   }
}

void PylonCamera::pylonRemoved()
{
   // StopVideo();
    PylonClose();
    cameraNumber=-1;
    lstDevices.clear();
    devices.clear();
    pylonOpened(false);
    if (isGuiNeed)
    {
        emit devicesChanged();
        emit choiceChanged(cameraNumber);
    }
    else
    {
        timer->start();
    }
}

void PylonCamera::PylonClose()
{
    StopVideo();
    if (camera)
    try
    {
        camera->DestroyDevice();
        delete camera;
        camera=nullptr;
    }
    catch (RuntimeException ex)
    {
        emit Message(LoggerMsg::Warning, QString("BSL: PylonCamera::PylonClose() %1").arg(ex.what()));
    }
    pylonOpened(false);

}

void PylonCamera::setChoice(int Index)
{
    if (Index==-1) return;
    cameraNumber=Index;
    emit choiceChanged(cameraNumber);
}

void PylonCamera::open(bool m)
{
   if (!m) {PylonClose(); return;}
   Open();
}

QStringList PylonCamera::Devices()
{
    return devices;
}

 PylonCamera::  ~PylonCamera()
{
   PylonClose();
   PylonTerminate();
}

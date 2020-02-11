#include "tm2221_5.h"

Tm2221_5::Tm2221_5(Log *log, PylonCamera* pylonCamera, Settings* set, QString name, QObject *parent):
    em5215(new EM5215(name,this)),
    comPort(new BlockComPort(set->getComPort(),this)),
    em5215_parser(new EM5215parser(name,this)),
    udpclient(new UDPclient(set->getControl(),this)),
    videoServer(pylonCamera->getVideoServer())
{
    connect(this,&Tm2221_5::sendDataTo,udpclient,&UDPclient::sendDataTo);

    this->setParent(parent);
    id=name;

    connect(comPort,&BlockComPort::answer,em5215_parser,&EM5215parser::answer);
    connect(em5215_parser,&EM5215parser::getData,em5215,&EM5215::setDataFromController);
    connect(em5215_parser,&EM5215parser::dataNoChanged,em5215,&EM5215::dataNoChanged);
    connect(em5215,&EM5215::executeCommand,comPort,&BlockComPort::executeCommand);

    //комманды контроллера и камеры обрабатываются отдельно
    connect(udpclient,&UDPclient::receiveData,em5215,&EM5215::command);
    connect(udpclient,&UDPclient::receiveData,pylonCamera,&PylonCamera::command);


    connect(comPort,&BlockComPort::Message,log,&Log::Message);
    connect(em5215_parser,&EM5215parser::Message,log,&Log::Message);
    connect(em5215,&EM5215::Message,log,&Log::Message);
    connect(udpclient,&UDPclient::Message,log,&Log::Message);

    connect(em5215,&EM5215::dataExhChanged,this,&Tm2221_5::dataExhChanged);
}

void Tm2221_5::startParse(const bool p)
{
    em5215->setParse(p);
}

QObject* Tm2221_5::getEM5215()
{
    return static_cast<QObject*> (em5215);
}

void Tm2221_5::sendDataPacket()
{
  QByteArray ar;
  QDataStream ds(&ar,QIODevice::ReadWrite);
  DeviceState FullState=checkFullState();
  ds<<FullState;
  ds<<*videoServer;
  ds<<*em5215;
  emit sendDataTo(ar);
}

DeviceState Tm2221_5::checkFullState()
{
    return videoServer->state()+em5215->state();
}

void Tm2221_5::checkPortsState()
{
    em5215->poll();
    em5215->checkPortsState();
    sendDataPacket();
}

void Tm2221_5::dataExhChanged()
{
    if (em5215->getDataExh()) em5215->askData();
}

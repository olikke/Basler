
#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent),
    Set(new QSettings("/BaslerSimpleClient",nullptr,this)),
    RingCounter(0)
{
    ReadMe();
    WriteMe();
}

void Settings::WriteMe()
{
    Set->beginGroup("Server FullHD");
    Set->setValue("Server IP", RingVector.at(0).sIP);
    Set->setValue("Port",RingVector.at(0).Port);
    Set->value("Resolution",RingVector.at(0).Resolution);
    Set->endGroup();
    Set->beginGroup("Server HalfHD");
    Set->setValue("Server IP",RingVector.at(1).sIP);
    Set->setValue("Port",RingVector.at(1).Port);
    Set->setValue("Resolution",RingVector.at(1).Resolution);
    Set->endGroup();
    Set->beginGroup("Own Socket");
    Set->setValue("IP",ControlOwnIP);
    Set->setValue("Port",QString::number(ControlOwnPort));
    Set->endGroup();
    Set->sync();
}

void Settings::ReadMe()
{
    OneNode Node ;
    RingVector.clear();
    Set->beginGroup("Server FullHD");
    Node.Port=Set->value("Port",5000).toString();
    Node.sIP=Set->value("Server IP","224.1.1.5").toString();
    Node.Resolution = Set->value("Resolution","1920x1080").toString();
    Set->endGroup();
    RingVector.push_back(Node);
    Set->beginGroup("Server HalfHD");
    Node.Port=Set->value("Port",5000).toString();
    Node.sIP=Set->value("Server IP","224.1.1.5").toString();
    Node.Resolution = Set->value("Resolution","960x540").toString();
    Set->endGroup();
    RingVector.push_back(Node);
    Set->beginGroup("Own Socket");
    ControlOwnIP=Set->value("IP","192.168.3.14").toString();
    ControlOwnPort=Set->value("Port",6000).toInt();
    Set->endGroup();
}

QString Settings::resolution()
{
    return RingVector.at(RingCounter).Resolution;
}

QString Settings::ownIP()
{
    return ControlOwnIP;
}

QString Settings::multiIP()
{
    return RingVector.at(RingCounter).sIP;
}

int Settings::ownPort()
{
    return ControlOwnPort;
}

int Settings::multiPort()
{
    return RingVector.at(RingCounter).Port.toInt();
}

void Settings::Next()
{
    RingCounter==0? RingCounter=1: RingCounter=0;
}

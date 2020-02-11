#include "settings.h"

// /home/user/.config

Settings::Settings(QObject *parent) :
    QObject(parent),
    set(new QSettings("/tm2221-5-1",nullptr,this))
{
    qWarning()<<set->fileName();
    readMe();
    writeMe();
}

Settings::~Settings()
{
    writeMe();
}

void Settings::readMe()
{
    needGUI=set->value("NeedGUI").toBool();
    comPort=set->value("ComPortName","/dev/ttyS4").toString();
    set->beginGroup("Server FullHD");
    fullServer.oIP=set->value("Own IP","192.168.3.12").toString();
    fullServer.Port=set->value("Port",5000).toString();
    fullServer.sIP=set->value("Server IP","224.1.1.5").toString();
    fullServer.quality=set->value("Quality",65).toInt();
    set->endGroup();
    set->beginGroup("Server HalfHD");
    halfServer.oIP=set->value("Own IP","192.168.3.12").toString();
    halfServer.Port=set->value("Port",5002).toString();
    halfServer.sIP=set->value("Server IP","224.1.1.5").toString();
    halfServer.quality=set->value("Quality",65).toInt();
    set->endGroup();
    set->beginGroup("Control");
    control.oIP=set->value("Own IP","192.168.3.12").toString();
    control.Port=set->value("Port",6000).toString();
    control.sIP=set->value("Server IP","192.168.3.255").toString();
    set->endGroup();
}

void Settings::writeMe()
{
    set->setValue("NeedGUI",needGUI);
    set->setValue("ComPortName",comPort);
    set->beginGroup("Server FullHD");
    set->setValue("Own IP",fullServer.oIP);
    set->setValue("Port",fullServer.Port);
    set->setValue("Server IP",fullServer.sIP);
    set->setValue("Quality",fullServer.quality);
    set->endGroup();
    set->beginGroup("Server HalfHD");
    set->setValue("Own IP",halfServer.oIP);
    set->setValue("Port",halfServer.Port);
    set->setValue("Server IP",halfServer.sIP);
    set->setValue("Quality",halfServer.quality);
    set->endGroup();
    set->beginGroup("Control");
    set->setValue("Own IP",control.oIP);
    set->setValue("Port",control.Port);
    set->setValue("Server IP",control.sIP);
    set->endGroup();
    set->sync();
}

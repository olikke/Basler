#include "tm2221_5.h"

Tm2221_5::Tm2221_5(cameraControl &_control, QObject *parent):
    control(_control),
    em5215(new EM5215(_control.name,this)),
    videoServer(new VideoServer(this))
{
    this->setParent(parent);
    connect(em5215,&EM5215::executeCommand,this,&Tm2221_5::sendData);
    connect(em5215,&EM5215::Message,this,&Tm2221_5::Message);
    id=control.name;
    getTime(timeStamp);
    waitingTime=1000;
}

void Tm2221_5::startParse(const bool p)
{
    em5215->setParse(p);
    videoServer->setParse(p);
}

void Tm2221_5::changeKuMode(char mode)
{
    QByteArray ba;
    ba.push_back(mode);
    emit sendDataTo(ba,control.ip);
}

void Tm2221_5::checkPortsState()
{
    bool temp=getMilliSecSince(timeStamp) <waitingTime;
    if (temp!=dataExchange)
    {
        dataExchange=temp;
        emit dataExhChanged();
        if (!dataExchange)
        {
            _state=DeviceState::Red;
            emit stateChanged();
        }
    }
}

void Tm2221_5::dataFromKU(QString &senderIp, int &senderPort, QByteArray &ba)
{
    if (senderIp!=control.ip || senderPort!=control.port) return;
    getTime(timeStamp);
    QDataStream ds(&ba,QIODevice::ReadOnly);
    static DeviceState temp=DeviceState::Unknown;
    ds>>_state;
    if (_state!=temp) emit stateChanged();
    ds>>*videoServer;
    ds>>*em5215;
}

void Tm2221_5::sendData(QByteArray &ba)
{
    emit sendDataTo(ba,control.ip);
}

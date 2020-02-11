#include "udpserver.h"

UDPserver::UDPserver(QString _ownIp, int _port, QObject *parent) :
   QObject(parent),
   IamReady(false),
   ownIp(_ownIp),
   port(_port)
{
    sock=new QUdpSocket(this);
    timer=new QTimer(this);
    timer->setInterval(500);
    connect(timer,&QTimer::timeout,this,&UDPserver::Connect);
    timer->start();
}

bool UDPserver::Connect()
{    
    if (!sock->bind(QHostAddress(ownIp),static_cast<quint16>(port),QAbstractSocket::DefaultForPlatform))
    {
        emit Message(LoggerMsg::Warning,QString("UDP: не удалось привязаться к порту: %1").arg(sock->errorString()));
        return false;
    }
    emit Message(LoggerMsg::Warning,"UDP: подключение открыто");
    connect(sock,&QUdpSocket::readyRead,this,&UDPserver::read);
    if (timer)timer->stop();
    return true;
}

void UDPserver::sendDataTo(QByteArray &ds, QString &kuIp)
{
    bool result;
    result=(ds.length()==sock->writeDatagram(ds,QHostAddress(kuIp),port));
    if (!result)
        emit Message(LoggerMsg::Warning,QString("UDP: не удалось отправить пакет: %1").arg(sock->errorString()));
}

void UDPserver::read()
{
    while (sock->hasPendingDatagrams())
    {
        QNetworkDatagram datagram=sock->receiveDatagram();
        QString SenderIP=datagram.senderAddress().toString();
        int SenderPort=datagram.senderPort();
        QByteArray data=datagram.data();
        emit receiveData(SenderIP,SenderPort, data);
    }
}

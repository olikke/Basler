#include "udpclient.h"

UDPclient::UDPclient( OneNode _control,QObject *parent) :
   QObject(parent),
   IamReady(false),
   control(_control)
{
    sock=new QUdpSocket(this);
}

bool UDPclient::Connect()
{    
    if (!sock->bind(QHostAddress(control.oIP),control.Port.toUShort(),QAbstractSocket::DefaultForPlatform))
    {
        emit Message(LoggerMsg::Warning,QString("UDP: не удалось привязаться к порту: %1").arg(sock->errorString()));
        return false;
    }
    emit Message(LoggerMsg::Warning,"UDP: подключение открыто");
    connect(sock,&QUdpSocket::readyRead,this,&UDPclient::read);
    return true;
}

void UDPclient::sendDataTo(QByteArray &ds)
{
    bool result;
    if (!IamReady) IamReady=Connect();
    if (IamReady)
    {
        result=(ds.length()==sock->writeDatagram(ds,QHostAddress(control.sIP),control.Port.toUShort()));
        if (!result)
            emit Message(LoggerMsg::Warning,QString("UDP: не удалось отправить пакет: %1").arg(sock->errorString()));
    }
}

void UDPclient::read()
{
    while (sock->hasPendingDatagrams())
    {
        QNetworkDatagram datagram=sock->receiveDatagram();
        //здесь можно забацать проверку, что данные пришли с допустимого адреса
//        QString SenderIP=datagram.senderAddress().toString();
    //    int SenderPort=datagram.senderPort();
        QByteArray data=datagram.data();
        if (data.length()>0)
            emit receiveData(data);
    }
}

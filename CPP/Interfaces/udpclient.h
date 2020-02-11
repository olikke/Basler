#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include "../CPP/global.h"
#include "../CPP/log.h"

class UDPclient : public QObject
{
    Q_OBJECT
public:
    UDPclient(OneNode _control, QObject *parent = nullptr);
    void Prepare();
signals:
    void receiveData(QByteArray &ba);
    void Message(LoggerMsg lm,  QString message);
public slots:
    void read();
    void sendDataTo(QByteArray &ds);
private:
    QUdpSocket *sock=nullptr;
    bool Connect();
    bool IamReady;
    OneNode control;
};


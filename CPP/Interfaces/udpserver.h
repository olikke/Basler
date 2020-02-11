#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QTimer>
#include "../CPP/log.h"

//olikke мне не нравится, что не обработаны ошибки совсем. Разрыв соединения, например


class UDPserver : public QObject
{
    Q_OBJECT
public:
    UDPserver(QString _ownIp, int _port, QObject *parent = nullptr);
    void Prepare();
signals:
    void receiveData(QString &senderIp, int &senderPort, QByteArray &ba);
    void Message(LoggerMsg lm,  QString message);
public slots:
    void read();
    void sendDataTo(QByteArray &ds, QString &kuIp);
private:
    QUdpSocket *sock=nullptr;
    bool Connect();
    bool IamReady;
    QString ownIp;
    int port;
    QTimer* timer=nullptr;
};


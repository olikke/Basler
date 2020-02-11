#pragma once

#include <QObject>
#include <QTimer>
#include <QDataStream>
#include "../CPP/global.h"
#include "../CPP/unitsparent.h"
#include "../CPP/log.h"
#include "../CPP/TM-2221-5/EM5215-2/em5215_2.h"
#include "../CPP/TM-2221-5/VideoServer/videoserver.h"

struct cameraControl
{
    QString ip; //tm2221 ip
    int port;
    QString name;
    cameraControl(QString _ip, int _port, QString _name): ip(_ip), port(_port), name(_name)
    {
    }
};

class Tm2221_5 : public UnitsParent
{
    Q_OBJECT
public:
    //в Главном операторе прописать схемное обозначение чтобы отличать в логе записи от разных устройств
    Tm2221_5(cameraControl &_control, QObject *parent = nullptr);

    Q_PROPERTY(bool dataExh READ getDataExh NOTIFY dataExhChanged)
    bool getDataExh() {return dataExchange;}

    Q_PROPERTY(DeviceState state READ getState NOTIFY stateChanged)
    DeviceState getState() {return _state;}

    Q_INVOKABLE void startParse(const bool p);

    Q_INVOKABLE QObject* getEM5215() {return static_cast<QObject*> (em5215);}

    Q_INVOKABLE QObject* getVideoServer() {return static_cast<QObject*> (videoServer);}

    Q_INVOKABLE void changeKuMode(char mode);

signals:
    void Message(LoggerMsg lm,  QString message);
    void dataExhChanged();
    void sendDataTo(QByteArray &ds, QString &kuIp);
    void stateChanged();
public slots:
    virtual void checkPortsState() override;
    void dataFromKU(QString &senderIp, int &senderPort, QByteArray &ba);
    void sendData(QByteArray &ba);
private:
    cameraControl control;
    EM5215* em5215;
    VideoServer* videoServer;
    std::chrono::high_resolution_clock::time_point timeStamp;
    bool dataExchange=false;
};


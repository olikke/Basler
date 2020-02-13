#pragma once

#include <QObject>
#include <QTimer>
#include "../CPP/unitsparent.h"
#include "../CPP/log.h"
#include "../CPP/TM-2221-5/EM5215-2/em5215_2.h"
#include "../CPP/Interfaces/blockcomport.h"
#include "../CPP/TM-2221-5/EM5215-2/em5215_parser.h"
#include "Pylon/pyloncamera.h"
#include "../CPP/Interfaces/udpclient.h"
#include "../CPP/TM-2221-5/VideoServer/videoserver_struct.h"


class Tm2221_5 : public UnitsParent
{
    Q_OBJECT
public:
    //в Главном операторе прописать схемное обозначение чтобы отличать в логе записи от разных устройств
    Tm2221_5(Log* log, PylonCamera* pylonCamera, Settings* set, QString name=" ",QObject *parent = nullptr);
signals:
    void Message(LoggerMsg lm,  QString message);
    void sendDataTo(QByteArray &ba);
public slots:
    virtual void checkPortsState() override;
    void dataExhChanged();
    QObject* getEM5215();
private:
    EM5215* em5215;
    BlockComPort* comPort;
    EM5215parser* em5215_parser;
    UDPclient *udpclient=nullptr;
    void sendDataPacket();
    DeviceState checkFullState();
    VideoServerStatistic* videoServer;
};



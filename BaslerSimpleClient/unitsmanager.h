#pragma once

#include <QObject>
#include <QTimer>
#include "../CPP/log.h"
#include "../CPP/unitsparent.h"
#include "TM2221-5/tm2221_5.h"
#include "../CPP/Interfaces/udpserver.h"

class UnitsManager : public QObject
{
    Q_OBJECT
public:
    explicit UnitsManager(QObject *parent = nullptr);

private:
    QTimer* timer;
    QList<UnitsParent*> unitsList; // список хранящий все обьекты (ты можешь переделать его как хочешь, только соблюдай интерфейс получения ссылки getUnitPtr)
    Log* log;
    //один сервак для приёма сообщений от 4 КУ (кст на отправку сообщений можно будет использовать broadcast (например, выключить все, сменить всем режим и т.д.)
    //в АРМЦ таких будет 2 - по 4 камеры в своей подсети
    UDPserver* kuServer;
    void tm2221Create();
public slots:
    QObject* getUnitPtr(QString key); // интерфейс сопряжения с QML
    QObject* getLog();
};

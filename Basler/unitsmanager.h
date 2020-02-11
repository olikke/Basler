#pragma once

#include <QObject>
#include <QTimer>
#include <QThread>


#include "../CPP/log.h"
#include "Pylon/pyloncamera.h"
#include "../CPP/unitsparent.h"
#include "TM2221-5/tm2221_5.h"



class UnitsManager : public QObject
{
    Q_OBJECT
public:
    explicit UnitsManager(Settings* set, QObject *parent = nullptr);

private:
    QTimer* timer;
    QList<UnitsParent*> unitsList; // список хранящий все обьекты (ты можешь переделать его как хочешь, только соблюдай интерфейс получения ссылки getUnitPtr)
    Log* log;
    PylonCamera* pylonCamera;
    Tm2221_5* tm2221_5;
public slots:
    QObject* getUnitPtr(QString key); // интерфейс сопряжения с QML
    QObject* getCamera();
    QObject* getLog();
};

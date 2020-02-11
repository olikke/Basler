#include "unitsmanager.h"

UnitsManager::UnitsManager(Settings* set, QObject *parent) : QObject(parent),
    timer (new QTimer(this)),
    log(new Log(set->getNeedGUI(),"TM2221_5_",this)),
    pylonCamera(new PylonCamera(set,log,this)),
    tm2221_5(new Tm2221_5(log, pylonCamera,set,"TM-2221-5",this))
{    

    connect(timer,&QTimer::timeout,pylonCamera,&PylonCamera::getStat);
    connect(timer,&QTimer::timeout,tm2221_5,&Tm2221_5::checkPortsState);

    unitsList.push_back(tm2221_5);


    timer->setInterval(500);
    timer->start();
}

QObject *UnitsManager::getUnitPtr(QString id)
{
    for(auto d : unitsList){
        if(d->getId() == id) return static_cast<QObject*>(d);
    }
    return nullptr;
}

QObject *UnitsManager::getCamera()
{
    return static_cast<QObject*> (pylonCamera);
}

QObject* UnitsManager::getLog()
{
    return static_cast<QObject*> (log);
}

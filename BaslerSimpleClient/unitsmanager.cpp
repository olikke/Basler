#include "unitsmanager.h"

UnitsManager::UnitsManager( QObject *parent) : QObject(parent),
    timer (new QTimer(this)),
    log(new Log(true,QString("TM2221_5_SimpleClient_"),this)),
    //это адрес привязки для сервера. Он единый на подсеть
    kuServer(new UDPserver("192.168.3.33",6000,this))
{    
    tm2221Create();
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

QObject* UnitsManager::getLog()
{
    return static_cast<QObject*> (log);
}

void UnitsManager::tm2221Create()
{
    //для каждого созданного тм-2221 прописать свой IP здесь Пока забьем жестко
    QList<cameraControl> tm2221List;
    //а это адрес КУ, с которой получаем данные. По этому адресу осуществляется фильтрация данных от КУ подсети
    tm2221List.push_back(cameraControl(QString("192.168.3.3"),6000,QString("TM-2221-5")));

    //создаём 1 подключение
    int count = 1;
    for (int i=0; i<count; i++)
    {
        Tm2221_5* tm2221 = new Tm2221_5(tm2221List[i], this);
        unitsList.push_back(tm2221);
        connect(timer,&QTimer::timeout,tm2221,&Tm2221_5::checkPortsState);
        connect(kuServer,&UDPserver::receiveData,tm2221,&Tm2221_5::dataFromKU);
        connect(tm2221,&Tm2221_5::sendDataTo,kuServer,&UDPserver::sendDataTo);
        connect(tm2221,&Tm2221_5::Message,log,&Log::Message);
    }
}

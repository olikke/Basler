#pragma once

#include <QObject>
#include "global.h"

class UnitsParent : public QObject
{
    Q_OBJECT
    Q_ENUM(DeviceState)
public:
    explicit UnitsParent(QObject *parent = nullptr) ;
protected:
    QString id;
    int waitingTime; // время при котором будет принято решение что данные старые и обмен данными не производится, определяется потомками (по умолчанию 500мс)

    //чтобы запустить парс данных для вывода на форму
    inline void setParse(bool _parse) {parse=_parse;}
    bool parse=false;
    DeviceState _state=DeviceState::Unknown;

public slots:
    QString getId()  const; //метод возвращает id блока, для получения ссылки на него из QML (потомки должны определить id)
    virtual void checkPortsState() = 0;


private:

};

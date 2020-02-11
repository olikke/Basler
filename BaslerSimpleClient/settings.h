#pragma once

#include <QObject>
#include <QSettings>
#include <QVector>
#include "../CPP/global.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
    QString resolution();
    QString ownIP();
    QString multiIP();
    int ownPort();
    int multiPort();
    void Next();
private:
    QSettings* Set;
    QVector<OneNode> RingVector;
    QString ControlOwnIP;
    int ControlOwnPort;
    int RingCounter;
    void ReadMe();
    void WriteMe();
};


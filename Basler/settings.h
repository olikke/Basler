#pragma once

#include <QObject>
#include <QSettings>
#include "../CPP/global.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    Settings(QObject *parent = nullptr);
    ~Settings();
    OneNode getFullServer() const {return fullServer;}
    OneNode getHalfServer() const {return halfServer;}
    OneNode getControl() const {return control;}
    bool getNeedGUI() const {return needGUI;}
    QString getComPort() const {return comPort;}
private:
    OneNode fullServer;
    OneNode halfServer;
    OneNode control;
    QSettings* set=nullptr;
    void readMe();
    void writeMe();
    bool needGUI;
    QString comPort;
};


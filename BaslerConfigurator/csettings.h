#ifndef CSETTINGS_H
#define CSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QFile>
#include <QList>
#include <QHostAddress>
#include "global.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
    Q_INVOKABLE bool openFile();
    Q_INVOKABLE void loadDefault();
    Q_INVOKABLE QList<QString> allValue();
    Q_INVOKABLE void allUpdate(QList<QString> arr);
    Q_INVOKABLE QString findBroadcast(QString currentIP);
    Q_PROPERTY(bool needGUI MEMBER NeedGUI NOTIFY needGUIChanged)
signals:
    void readyChanged(bool ready);
    void messages(QString mess, bool goodNews);
    void needGUIChanged();
public slots:

private:
    OneNode* FullServer;
    OneNode* HalfServer;
    OneNode* Control;
    QSettings* Set;
    void ReadMe();
    void WriteMe();
    bool CheckIpAddress(QString arr);
    bool NeedGUI;
};

#endif // CSETTINGS_H

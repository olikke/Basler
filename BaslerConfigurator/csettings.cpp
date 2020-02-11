#include "csettings.h"

Settings::Settings(QObject *parent) : QObject(parent),
    FullServer(new OneNode()),
    HalfServer(new OneNode()),
    Control(new OneNode()),
    Set(nullptr)
{

}

bool Settings::openFile()
{
    Set=new QSettings("/tm2221-5-1",nullptr,this);
    QFile FSettin(Set->fileName().toLocal8Bit().constData());
    if (!FSettin.exists())
    {
        emit messages("Файл с настройками открыть не удалось", false);
        return false;
    }
    ReadMe();
    emit messages("Файл прочитан успешно", true);
    return true;
}

QList<QString> Settings::allValue()
{
    QList<QString> list;
    list.push_back(FullServer->Port);
    list.push_back(FullServer->sIP);
    list.push_back(QString::number(FullServer->quality));

    list.push_back(HalfServer->Port);
    list.push_back(HalfServer->sIP);
    list.push_back(QString::number(HalfServer->quality));

    list.push_back(Control->oIP);
    list.push_back(Control->Port);
    list.push_back(Control->sIP);
    return list;
}

QString Settings::findBroadcast(QString currentIP)
{
    if (!CheckIpAddress(currentIP)) return "Error";
    QRegExp rx ("[.]");
    QStringList list = currentIP.split(rx, QString::SkipEmptyParts);
    list.last()="255";
    QString temp=list.join('.');
    qDebug()<<temp;
    return temp;
}

bool Settings::CheckIpAddress(QString arr)
{
    QHostAddress addr(arr);
    if (QAbstractSocket::IPv4Protocol!=addr.protocol())
    {
        emit messages("Неверный IP адрес "+arr, false);
        return false;
    }
    return true;
}

void Settings::allUpdate(QList<QString> arr)
{
    if (arr.size()!=9) return;

    if (!CheckIpAddress(arr[1]) ||
            !CheckIpAddress(arr[4]) ||
            !CheckIpAddress(arr[6]) ||
            !CheckIpAddress(arr[8])) return;

    FullServer->oIP=arr[6];
    FullServer->Port=arr[0];
    FullServer->sIP=arr[1];
    FullServer->quality=arr[2].toInt();

    HalfServer->oIP=arr[6];
    HalfServer->Port=arr[3];
    HalfServer->sIP=arr[4];
    HalfServer->quality=arr[5].toInt();

    Control->oIP=arr[6];
    Control->Port=arr[7];
    Control->sIP=arr[8];

    WriteMe();
    ReadMe();
    emit messages("Настройки обновлены успешно",true);

}

void Settings::loadDefault()
{
    ReadMe();
    WriteMe();
    emit messages("Загружены настройки по умолчанию", true);
}

void Settings::ReadMe()
{
    NeedGUI=Set->value("NeedGUI").toBool();
    Set->beginGroup("Server FullHD");
    FullServer->oIP=Set->value("Own IP","192.168.3.12").toString();
    FullServer->Port=Set->value("Port",5000).toString();
    FullServer->sIP=Set->value("Server IP","224.1.1.5").toString();
    FullServer->quality=Set->value("Quality",65).toInt();
    Set->endGroup();
    Set->beginGroup("Server HalfHD");
    HalfServer->oIP=Set->value("Own IP","192.168.3.12").toString();
    HalfServer->Port=Set->value("Port",5002).toString();
    HalfServer->sIP=Set->value("Server IP","224.1.1.5").toString();
    HalfServer->quality=Set->value("Quality",65).toInt();
    Set->endGroup();
    Set->beginGroup("Control");
    Control->oIP=Set->value("Own IP","192.168.3.11").toString();
    Control->Port=Set->value("Port",1200).toString();
    Control->sIP=Set->value("Server IP","192.168.3.255").toString();
    Set->endGroup();
    emit readyChanged(true);
    emit needGUIChanged();
}

void Settings::WriteMe()
{
    Set->setValue("NeedGUI",NeedGUI);
    Set->beginGroup("Server FullHD");
    Set->setValue("Own IP",FullServer->oIP);
    Set->setValue("Port",FullServer->Port);
    Set->setValue("Server IP",FullServer->sIP);
    Set->setValue("Quality",FullServer->quality);
    Set->endGroup();
    Set->beginGroup("Server HalfHD");
    Set->setValue("Own IP",HalfServer->oIP);
    Set->setValue("Port",HalfServer->Port);
    Set->setValue("Server IP",HalfServer->sIP);
    Set->setValue("Quality",HalfServer->quality);
    Set->endGroup();
    Set->beginGroup("Control");
    Set->setValue("Own IP",Control->oIP);
    Set->setValue("Port",Control->Port);
    Set->setValue("Server IP",Control->sIP);
    Set->endGroup();
    Set->sync();
}

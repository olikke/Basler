#pragma once

#include <ostream>
#include <QString>
#include <QDebug>
#include <QDataStream>

const int VIDEO_WIDTH = 1920;
const int VIDEO_HEIGHT = 1080;

void getTime(std::chrono::high_resolution_clock::time_point &time);
int getMilliSecSince(std::chrono::high_resolution_clock::time_point time);

struct OneNode
{
    QString oIP;
    QString Port;
    QString sIP;
    QString Resolution;
    int quality;
    QString toString() ;
};
Q_DECLARE_METATYPE(OneNode)

void SetSingleRulesForDS(QDataStream &ds);

enum class DeviceState: quint8 {
    Green,
    Yellow,
    Red,
    Unknown
};

QDataStream & operator >>(QDataStream &ds, DeviceState &state);
QDataStream & operator <<(QDataStream &ds, DeviceState &state);
DeviceState operator + (const DeviceState& left, const DeviceState &right);

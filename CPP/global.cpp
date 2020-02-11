#include "global.h"

QString OneNode::toString()
{
    return QString("ownIP %1, IP %2, Port %3, Resolution %4, quality %5").arg(
                oIP,sIP,Port,Resolution,QString::number(quality));
}

void getTime(std::chrono::high_resolution_clock::time_point &time)
{
     time=std::chrono::high_resolution_clock::now();
}

int getMilliSecSince(std::chrono::system_clock::time_point timeStart)
{
    std::chrono::system_clock::time_point now;
    getTime(now);
    return std::chrono::duration_cast<std::chrono::milliseconds>(now-timeStart).count();
}

void SetSingleRulesForDS(QDataStream &ds)
{
    ds.setVersion(QDataStream::Qt_5_11);
    ds.setByteOrder(QDataStream::LittleEndian);
    ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
}

QDataStream &operator >>(QDataStream &ds, DeviceState &state)
{
        quint8 temp;
        ds>>temp;
        state=static_cast<DeviceState>(temp);
        return ds;
}

QDataStream &operator <<(QDataStream &ds, DeviceState &state)
{
    ds<<static_cast<quint8>(state);
    return ds;
}

DeviceState operator +(const DeviceState &left, const DeviceState &right)
{
    if (left>right) return left;
    return right;
}

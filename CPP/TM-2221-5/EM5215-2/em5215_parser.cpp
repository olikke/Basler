#include "em5215_parser.h"

EM5215parser::EM5215parser(QString _name,QObject *parent) :
    QObject(parent),
    name(_name)
{
    em5215Struct.DataExh=true;
    lastDataPacket.reserve(10);
}

void EM5215parser::answer(QByteArray &ba)
{
    QString commandName=toString(toEM5215Command(ba[0]));
    if (commandName==UNKNOWN)
    {
        emit Message(LoggerMsg::Warning,QString("%1: Неизвестный ответ %2.").arg(name,QString(ba.toHex(' ')).toUpper()));
        return;
    }
    int baSize=ba.size();
    switch (baSize)
    {
        case 3: answer3(ba,commandName); break;
        case 10: answer10(ba,commandName); break;
        case 12: answer12(ba,commandName); break;
        default:emit Message(LoggerMsg::Warning,QString("%1: Неизвестный ответ %2.").arg(name,QString(ba.toHex(' ')).toUpper()));
    }

}

void EM5215parser::answer3(QByteArray &ba, QString &commandName)
{
    char temp=char(ba[0] ^ ba[1]);
    if (temp != ba[2])
    {
        errorCheckSumm(ba,commandName);
        return;
    }
    char code=ba.at(1);
    if (code!=static_cast<char>(EM5215Code::OK))
    {
        emit Message(LoggerMsg::Warning,QString("%1: Команда '%2' выполнена c ошибкой: %3.'").arg(name,commandName,toString(static_cast<EM5215Code>(code))));
        return;
    }
    emit Message(LoggerMsg::Info,QString("%1: Команда '%2' выполнена успешно: %3").arg(name,commandName,ba.toHex(' ')));

}

void EM5215parser::answer10(QByteArray &ba, QString &commandName)
{    
    // Have the data changed? fastest check
    if (qstrcmp(lastDataPacket,ba)==0)
    {
        emit dataNoChanged();
        return;
    }
    quint8 crc=calcCRC8(reinterpret_cast<quint8*>(ba.data()),ba.size()-1);
    if (quint8(ba[9])!=crc)
    {
        errorCheckSumm(ba,commandName);
        return;
    }
    lastDataPacket=ba;
    // parse
    (static_cast<quint8>(ba[2])>=0x80) ? em5215Struct.Mode=EM5215Mode::Manual : em5215Struct.Mode = EM5215Mode::Auto;
    em5215Struct.TemperatureKu=ba[1];
    em5215Struct.Humidity=ba[2] & 0x7F;
    em5215Struct.TemperatureBody=ba[3];
    em5215Struct.TemperatureRadiator=ba[4];
    em5215Struct.FanSpeed=ba[5];
    em5215Struct.CurrentP1=ba[6]&0x0F;
    em5215Struct.CurrentP2=(ba[6]&0xF0)>>4;
    em5215Struct.KuOn=(ba[7] & 0x01)==0x01;
    em5215Struct.FanOn=(ba[7] & 0x02) == 0x02;
    em5215Struct.R1On=(ba[7] & 0x04)==0x04;
    em5215Struct.R2On=(ba[7] & 0x08) ==0x08;
    em5215Struct.P1On=(ba[7] &0x30)>>4;
    em5215Struct.P2On=(ba[7] & 0xC0)>>6;
    em5215Struct.FanU=ba[8];
    emit getData(em5215Struct);
}

void EM5215parser::answer12(QByteArray &ba, QString &commandName)
{
    if (toEM5215Command(ba[0])!=EM5215Command::Data)
    {
        emit Message(LoggerMsg::Warning,QString("%1: неверный пакет данных %2").arg(name,QString(ba.toHex(' '))));
        return;
    }
    quint8 crc=calcCRC8(reinterpret_cast<quint8*>(ba.data()),ba.size()-1);
    if (quint8(ba[11])!=crc)
    {
        errorCheckSumm(ba,commandName);
        return;
    }
    em5215Struct.SerialNumber=(static_cast<quint8>(ba[4])<<16)+(static_cast<quint8>(ba[5])<<8)+static_cast<quint8>(ba[6]);
    emit getData(em5215Struct);
    emit Message(LoggerMsg::Info,QString("%1: Команда '%2' выполнена успешно. Серийный номер ЭМ-5215_2: %3")
         .arg(name,commandName,QString::number(em5215Struct.SerialNumber)));

}

quint8 EM5215parser::calcCRC8(quint8 *buf, int len)
{
    quint8 crc=0xFF;
    while (len--)
        crc=CRC8Table[crc ^ *buf++] ;
    return crc;
}

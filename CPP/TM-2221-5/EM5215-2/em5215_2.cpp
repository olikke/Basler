#include "em5215_2.h"

EM5215::EM5215(QString _name,QObject *parent)
{
    this->setParent(parent);
    em5215Ring.push_back(EM5215Struct());
    em5215Ring.push_back(EM5215Struct());
    RingCounter=0;
    em5215Struct=&em5215Ring[RingCounter];
    getTime(timeStamp);
    waitingTime=1000;
    id=_name;
    com.reserve(3);
}

void EM5215::setAutoMode(const bool autoMode)
{
    if (autoMode) setAuto();
    else setManual();
}

bool EM5215::isCommandAvailable(EM5215Command com)
{
    if (em5215Struct->Mode==EM5215Mode::Manual) return true;
    if (isAutoAvailable(com)) return true;
    emit Message(LoggerMsg::Warning,QString("%1: Команда %2 недоступна").arg(id,toString(com)));
    return false;
}

bool EM5215::isAutoAvailable(EM5215Command command)
{
    for (const auto & available:AutoAvailable)
        if (command==available) return true;
    return false;
}

void EM5215::swapCounter()
{
    RingCounter=OppositeCounter();
    em5215Struct=&em5215Ring[RingCounter];
}

void EM5215::setDataFromUDP(QDataStream &ds)
{
    getTime(timeStamp);
    ds>>em5215Ring[OppositeCounter()];
    swapCounter();
    if (parse) checkDataAndEmit();
}

void EM5215::setDataFromController(EM5215Struct &newStruct)
{
    getTime(timeStamp);
    em5215Ring[OppositeCounter()]=newStruct;
    swapCounter();
    if (parse) checkDataAndEmit();
}

void EM5215::checkDataAndEmit()
{
    int old=OppositeCounter();

    if (em5215Ring[old].SerialNumber!=em5215Struct->SerialNumber)
    {
        emit Message(LoggerMsg::Warning,QString("%1: Новый серийный номер ЭМ-5215: %2").arg(id,QString::number(em5215Struct->SerialNumber)));
        emit serialNumberChanged();
    }

    if (em5215Ring[old].DataExh!=em5215Struct->DataExh)
        emit dataExhChanged();

    if (em5215Ring[old].Mode!=em5215Struct->Mode)
        emit AutoModeChanged();

    if (em5215Ring[old].TemperatureKu!=em5215Struct->TemperatureKu)
        emit temperatureKUChanged();

    if (em5215Ring[old].Humidity!=em5215Struct->Humidity)
        emit humidityChanged();

    if (em5215Ring[old].TemperatureBody!=em5215Struct->TemperatureBody)
        emit temperatureBodyChanged();

    if (em5215Ring[old].TemperatureRadiator!=em5215Struct->TemperatureRadiator)
        emit temperatureRadiatorChanged();

    if (em5215Ring[old].FanSpeed!=em5215Struct->FanSpeed)
        emit fanSpeedChanged();

    if (em5215Ring[old].CurrentP1!=em5215Struct->CurrentP1)
        emit currentP1Changed();

    if (em5215Ring[old].CurrentP2!=em5215Struct->CurrentP2)
        emit currentP2Changed();

    if (em5215Ring[old].KuOn!=em5215Struct->KuOn)
        emit kuOnChanged();

    if (em5215Ring[old].FanOn!=em5215Struct->FanOn)
        emit fanOnChanged();

    if (em5215Ring[old].R1On!=em5215Struct->R1On)
        emit r1OnChanged();

    if (em5215Ring[old].R2On!=em5215Struct->R2On)
        emit r2OnChanged();

    if (em5215Ring[old].P1On!=em5215Struct->P1On)
        emit p1OnChanged();

    if (em5215Ring[old].P2On!=em5215Struct->P2On)
        emit p2OnChanged();

    if (em5215Ring[old].FanU!=em5215Struct->FanU)
        emit fanUChanged();
}

void EM5215::switchKu(const bool switchOn)
{
    newCommand(EM5215Command::SwitchKU,static_cast<quint8>(switchOn)) ;
}

void EM5215::changeFanU( const quint8 rpm)
{
    newCommand(EM5215Command::SwitchFun,rpm);
}

void EM5215::switchR1(const bool switchOn)
{
    newCommand(EM5215Command::SwitchR1, static_cast<quint8>(switchOn));
}

void EM5215::switchR2(const bool switchOn)
{
    newCommand(EM5215Command::SwitchR2, static_cast<quint8>(switchOn));
}

void EM5215::switchR(const bool switchOn)
{
    newCommand(EM5215Command::SwitchR, static_cast<quint8>(switchOn));
}

void EM5215::checkPortsState()
{
    static bool lastDE=false;
    bool temp=getMilliSecSince(timeStamp) <waitingTime;
    if (temp!=lastDE)
    {
        lastDE=temp;
        em5215Struct->DataExh=temp;
        emit dataExhChanged();
    }
}

void EM5215::dataNoChanged()
{
    getTime(timeStamp);
}

void EM5215::switchP1(const int peltier,const int current)
{
    quint8 temp=quint8(current<<4)+((peltier & 0x03));
    newCommand(EM5215Command::SwitchP1,temp);
}

void EM5215::switchP2(const int peltier,const int current)
{
    quint8 temp=quint8(current<<4)+((peltier & 0x03));
    newCommand(EM5215Command::SwitchP2,temp);
}

QList<int> EM5215::getFanLimit()
{
    QList<int> List;
    List.push_back(FAN_LO_LIMIT);
    List.push_back(FAN_HI_LIMIT);
    return List;
}

QList<int> EM5215::getCurrentLimit()
{
    QList<int> List;
    List.push_back(CURRENT_LO_LIMIT);
    List.push_back(CURRENT_HI_LIMIT);
    return List;
}

DeviceState EM5215::getHumidityColor()
{
    if (em5215Struct->Humidity<HUMIDITY_YELLOW) return DeviceState::Green;
    if (em5215Struct->Humidity<HUMIDITY_RED) return DeviceState::Yellow;
    return DeviceState::Red;
}

QDataStream &operator >>(QDataStream &ds, EM5215 &v)
{
    v.setDataFromUDP(ds);
    return ds;
}

QDataStream &operator <<(QDataStream &ds, EM5215 &v)
{
    ds<<*v.em5215Struct;
    return ds;
}

void EM5215::setAuto()
{
    newCommand(EM5215Command::AutoMode,1);
}

void EM5215::setManual()
{
    newCommand(EM5215Command::ManualMode,1);
}

void EM5215::askData()
{
    newCommand(EM5215Command::Data,1);
}

void EM5215::poll()
{
    newCommand(EM5215Command::Poll,0);
}

void EM5215::command(QByteArray &ba)
{
    if (ba.size()!=3) return;
    if (!isCommandAvailable(toEM5215Command(ba[0]))) return;
    QString commandName=toString(toEM5215Command(ba[0]));
    emit Message(LoggerMsg::Info,QString("%1: выполнение внешней команды %2: %3").arg(id,commandName,QString(ba.toHex(' '))));
    emit executeCommand(ba);
}

void EM5215::newCommand(EM5215Command command, quint8 value)
{
   if (!isCommandAvailable(command)) return;

    quint8 _command=static_cast<quint8>(command);
    com[0]=_command;
    com[1]=value;
    com[2]=_command ^ value;
    if (command!=EM5215Command::Poll && command!=EM5215Command::Data)
        emit Message(LoggerMsg::Info,QString("%1: отправлена команда %2: %3").arg(id,toString(command),QString(com.toHex(' '))));
    emit executeCommand(com);
}

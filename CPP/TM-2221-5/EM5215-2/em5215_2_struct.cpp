#include "em5215_2_struct.h"

EM5215Command toEM5215Command(char byte)
{
    for (const auto & available:EM5215CommandList)
        if (static_cast<quint8>(byte)==static_cast<quint8>(available)) return available;
    return EM5215Command::Undefined;
}

QString toString(EM5215Command command)
{
    switch (command) {
    case EM5215Command::Undefined:
        return "Неизвестная команда";
    case EM5215Command::Poll:
        return "Опрос состояния";
    case EM5215Command::SwitchKU:
        return "Вкл/Выкл КУ";
    case EM5215Command::SwitchFun:
        return "Вкл/Выкл вентилятора";
    case EM5215Command::SwitchR1:
        return "Вкл/Выкл резистора 1";
    case EM5215Command::SwitchR2:
        return "Вкл/Выкл резистора 2";
    case EM5215Command::SwitchR:
        return "Вкл/Выкл резисторов";
    case EM5215Command::SwitchP1:
        return "Вкл/Выкл Пельтье 1";
    case EM5215Command::SwitchP2:
        return "Вкл/Выкл Пельтье 2";
    case EM5215Command::AutoMode:
        return "Переключение в автоматический режим";
    case EM5215Command::ManualMode:
        return "Переключение в ручной режим";
    case EM5215Command::Data:
        return "Запрос данных устройства";
    }
    return "Неизвестная команда";
}

QString toString(EM5215Code code)
{
    switch (code) {
    case EM5215Code::OK:
        return "Успешно";
    case EM5215Code::ErrCom:
        return "Ошибочная/недопустимая команда";
    case EM5215Code::ErrCS:
        return "Неверная контрольная сумма";
    }
    return UNKNOWN;
}

QDataStream &operator <<(QDataStream &ds, EM5215Struct &v)
{
    SetSingleRulesForDS(ds);
    ds  <<v.DataExh
        <<v.SerialNumber
        <<static_cast<quint8>(v.Mode)
        <<v.TemperatureKu
        <<v.Humidity
        <<v.TemperatureBody
        <<v.TemperatureRadiator
        <<v.FanSpeed
        <<v.CurrentP1
        <<v.CurrentP2
        <<v.KuOn
        <<v.FanOn
        <<v.R1On
        <<v.R2On
        <<v.P1On
        <<v.P2On
        <<v.FanU;
    return ds;
}

QDataStream &operator >>(QDataStream &ds, EM5215Struct &v)
{
    SetSingleRulesForDS(ds);
    ds  >>v.DataExh
        >>v.SerialNumber;
    quint8 temp;
    ds>>temp;
    v.Mode=static_cast<EM5215Mode>(temp);
    ds   >>v.TemperatureKu
         >>v.Humidity
         >>v.TemperatureBody
         >>v.TemperatureRadiator
         >>v.FanSpeed
         >>v.CurrentP1
         >>v.CurrentP2
         >>v.KuOn
         >>v.FanOn
         >>v.R1On
         >>v.R2On
         >>v.P1On
         >>v.P2On
         >>v.FanU;
    return ds;
}

QDebug operator<<(QDebug dbg, const EM5215Struct *v)
{
    dbg.nospace() <<"\n";
    QString mode;
    v->Mode==EM5215Mode::Auto? mode="Auto" : mode="Manual";
    QString n1,n2,n3,n4,n5;
    v->KuOn? n1="yes" : n1="no";
    v->FanOn? n2="yes" :n2="no";
    v->R1On? n3="yes" :n3="no";
    v->R2On? n4="yes" :n4="no";
    v->FanU!=0? n5=QString::number(7+(v->FanU*5.5/255),'f',2) : n5="0";
    dbg.nospace() <<"SN "<<v->SerialNumber<<"\t Mode "<<mode<<"\t KuOn "<<n1<<"\n";
    dbg.nospace() <<"FanOn "<<n2<<"\t FanSpd "<<v->FanSpeed*60<<"\t FanU "<<n5<<"\n";
    dbg.nospace() <<"R1On "<<n3<<"\t R2On "<<n4<<"\n";
    dbg.nospace() <<"P1On "<<v->P1On<<"\t CurrP1 "<<v->CurrentP1<<"\n";
    dbg.nospace() <<"P2On "<<v->P2On<<"\t CurrP2 "<<v->CurrentP2<<"\n";
    dbg.nospace() <<"TempKU "<<v->TemperatureKu<< "\t TempBody "<<v->TemperatureBody<<"\n";
    dbg.nospace() <<"Humidity "<<v->Humidity<<"\t TempRad "<<v->TemperatureRadiator<<"\n";
    return dbg.space();
}

DeviceState EM5215Struct::state()
{
    if (!DataExh) return DeviceState::Red;
    if (Humidity>=HUMIDITY_RED) return DeviceState::Red;
    if (Humidity>=HUMIDITY_YELLOW) return DeviceState::Yellow;
    return DeviceState::Green;
}

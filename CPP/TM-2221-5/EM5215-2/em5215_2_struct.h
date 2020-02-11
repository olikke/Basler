#pragma once
#include "../CPP/global.h"

const QString UNKNOWN="Неизвестная команда";

const quint32 UNDEFINED_NUMBER = 0xFFFFFFFF;

const int FAN_HI_LIMIT = 0xFF;
const int FAN_LO_LIMIT = 0x00;
const double FAN_VALUE_HI_LIMIT = 12.5;
const double FAN_VALUE_LO_LIMIT = 7.;


const double CURRENT_VALUE_HI_LIMIT = 1.;
const double CURRENT_VALUE_LO_LIMIT = 0.2;
const int CURRENT_HI_LIMIT = 0x0F;
const int CURRENT_LO_LIMIT = 0x00;

const quint8 HUMIDITY_YELLOW = 60;
const quint8 HUMIDITY_RED = 70;

enum class EM5215Command:quint8 {
    //
    Undefined     = 0x00,
    //опрос состояния устройства
    Poll                = 0x11,
    //вкл/выкл камеры
    SwitchKU     = 0x22,
    //вкл/выкл вентилятора
    SwitchFun   = 0x33,
    //вкл/выкл резистора обогрева 1
    SwitchR1      = 0x44,
    //вкл/выкл резистора обогрева 2
    SwitchR2      = 0x55,
    //вкл/выкл обоих резисторов
    SwitchR        = 0x66,
    //управление Пельтье 1
    SwitchP1      = 0x77,
    //управление Пельтье 2
    SwitchP2      = 0x88,
    //переключение в автоматический режим
    AutoMode    = 0xAA,
    //переключение в ручной режим
    ManualMode= 0xBB,
    //запрос данных устройства (номер, дата изготовления и т.д.
    Data              = 0x99
};

static const EM5215Command EM5215CommandList[] =
{
    EM5215Command::Undefined ,
    EM5215Command::Poll,
    EM5215Command::SwitchKU,
    EM5215Command::SwitchFun,
    EM5215Command::SwitchR1,
    EM5215Command::SwitchR2,
    EM5215Command::SwitchR,
    EM5215Command::SwitchP1,
    EM5215Command::SwitchP2,
    EM5215Command::AutoMode,
    EM5215Command::ManualMode,
    EM5215Command::Data
};

QString toString(EM5215Command command);

EM5215Command toEM5215Command(char byte);

enum class EM5215Code: char{
    //команда выполнена
    OK          = 0x55,
    //ошибка команды (возможно в ручном режиме недопустимая команда)
    ErrCom = 0x33,
    //ошибка контрольной суммы
    ErrCS     = 0x22
};

QString toString(EM5215Code code);


enum class EM5215Mode{
    Auto,
    Manual
};

//В автоматичекском режиме набор команд ограничен
static const EM5215Command AutoAvailable[] =
{
    EM5215Command::Poll,
    EM5215Command::SwitchKU,
    EM5215Command::SwitchFun,
    EM5215Command::ManualMode,
    EM5215Command::AutoMode,
    EM5215Command::Data
};

struct EM5215Struct {
    bool    DataExh = false;
    quint64 SerialNumber=UNDEFINED_NUMBER;
    EM5215Mode  Mode;
    qint8   TemperatureKu;
    quint8  Humidity;
    qint8   TemperatureBody;
    qint8   TemperatureRadiator;
    quint8  FanSpeed;
    quint8  CurrentP1;
    quint8  CurrentP2;
    quint8  FanU;
    bool    KuOn=false;
    bool    FanOn=false;
    bool    R1On=false;
    bool    R2On=false;
    quint8  P1On;
    quint8  P2On;
    friend QDataStream & operator << (QDataStream & ds, EM5215Struct &v);
    friend QDataStream & operator >> (QDataStream & ds, EM5215Struct &v);
    friend QDebug operator<<(QDebug dbg, const EM5215Struct *v);
    DeviceState state();
};

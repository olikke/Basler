#pragma once
#include <QObject>
#include <Base/GCString.h>
#include <QDebug>

class PylonFeaturesEnum
{
    Q_GADGET
public:
    enum DataType
    {
        DInt,
        DEnum,
        DFloat,
        DString,
        DBool
    };
    Q_ENUM (DataType)
    enum EnumInt
    {
        Width,
        Height,
        XOffset,
        YOffset,
        CenterX,
        CenterY,
        GainRaw,
        BlackLevelRaw,
        DigitalShift,
        GainLoLimit,
        GainHiLimit,
        ExposureTimeRaw
    };
    Q_ENUM (EnumInt)
    enum EnumOfEnum
    {
        GainAuto,
        GammaSelector,
        ExposureAuto,
        AutoFunctionProfile
    };
    Q_ENUM (EnumOfEnum)
    enum EnumFloat
    {
        TemperatureAbs,
        AcquisitionFrameRateAbs,
        ResultingFrameRateAbs,
        ResultingFramePeriodAbs,
        AutoExposureTimeAbsLowerLimit,
        AutoExposureTimeAbsUpperLimit
    };
    Q_ENUM (EnumFloat)
   PylonFeaturesEnum() = delete;
};

class NameForPylon:public QObject
{
    Q_OBJECT
private:    
    const QVector<QString> IntName
    {
         "Width","Высота","Высота кадра в пикселях",
         "Height","Ширина","Ширина кадра в пикселях",
         "OffsetX","Х сдвиг","Горизонтальный сдвиг кадра в пикселях",
         "OffsetY","Y сдвиг","Вертикальный сдвиг кадра в пикселях",
         "CenterX","Х центр","Центрирование изображения по горизонтали",
         "CenterY","У центр","Центрирование изображения по вертикали",
         "GainRaw","Усиление","Значение выбранного усиления (необработанное значение)",
         "BlackLevelRaw","Уровень черного","Значение уровня черного (необработанное значение)",
         "DigitalShift","Цифровой сдвиг", "Цифровой сдвиг позволяет умножать значения пикселей на изображениях. При этом увеличивается яркость изображения. Если для данного параметра задано нулевое значение, цифровой сдвиг отключен",
         "AutoGainRawLowerLimit","Мин. усиление","Минимальное значение усиления, если установлено непрерывная авторегулировка параметра  (необработанное значение)",
         "AutoGainRawUpperLimit","Макс. усиление","Максимальное значение усиления, если установлено непрерывная авторегулировка параметра  (необработанное значение)",
         "ExposureTimeRaw","Экспозиция", "Экспозиция камеры в микросекундах (необработанное значение) при выключенном режиме авторегулировки экспозиции"
    };
    const QVector<QString> EnumName
    {
        "GainAuto", "Режим усиления" , "Установка режима работы автофункции усиления. В непрерывном режиме, усиление автоматически настраивается в пределах заданного ограничения до достижения указанного значения яркости.",
        "GammaSelector", "", "",
        "ExposureAuto", "Режим экспозиции", "Установка режима работы автоматической установки экспозиции. В режиме 'Выкл' время экспозиции задается вручную параметром ХХХ. В непрерывном режиме экспозиция устанавливается в пределах заданного ограничения до достижения указанного значения яркости.",
        "AutoFunctionProfile", "", ""
    };
    const QVector<QString> FloatName=
    {
        "TemperatureAbs","","",
        "AcquisitionFrameRateAbs","Частота кадров","Значение кадровой частоты для захвата изображения камерой",
        "ResultingFrameRateAbs","","",
        "ResultingFramePeriodAbs","","",
        "AutoExposureTimeAbsLowerLimit","Мин. время, мкс","Минимальное время экспозиции, когда включен режим автоматической установки экспозиции",
        "AutoExposureTimeAbsUpperLimit","Макс. время, мкс","Максимальное время экспозиции, когда включен режим автоматической установки экспозиции"
    };
    QString getName(int &numb, int &TypeOfData, int Column);
public:
    Q_INVOKABLE GenICam::gcstring getName(int numb, int Type)      {return  getName(numb,Type,0).toUtf8().constData();}
    Q_INVOKABLE QString getNameRU(int numb, int Type) {return  getName(numb,Type,1);}
    Q_INVOKABLE QString getHint(int numb, int Type)         {return  getName(numb,Type,2);}
};



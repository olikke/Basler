#pragma once

#include <QObject>
#include <QDataStream>
#include "em5215_2_struct.h"
#include "../CPP/log.h"
#include "../CPP/unitsparent.h"
#include "../CPP/global.h"

//olikke
//ВНИМАНИЕ!!! ПРИ ОТПРАВКЕ ЛЮБОЙ КОМАНДЫ НА ИЗМЕНЕНИЕ, СТАВИТЬ СЧЕТЧИК НА ВЫДАЧУ ИЗВЕЩЕНИЙ ОБ ИЗМЕНЕНИИ ДАННОГО ПАРАМЕТРА НА ФОРМУ!!
//поскольку туды-сюды отправка, и изменения могут произойти с задержкой. Чтоб слайдер не дергался. Или свитч. Или че там

class EM5215 : public UnitsParent
{
    Q_OBJECT
public:
    //в Главном операторе прописать схемное обозначение чтобы отличать в логе записи от разных устройств
    EM5215(QString _name=" ", QObject *parent = nullptr);

    Q_PROPERTY(int serialNumber READ getSerialNumber NOTIFY serialNumberChanged)
    int getSerialNumber() {return em5215Struct->SerialNumber;}

    Q_PROPERTY(bool dataExh READ getDataExh NOTIFY dataExhChanged)
    bool getDataExh() {return  em5215Struct->DataExh;}

    Q_PROPERTY(bool autoMode READ getAutoMode NOTIFY AutoModeChanged)
    bool getAutoMode() {return em5215Struct->Mode==EM5215Mode::Auto;}

    Q_PROPERTY(int temperatureKU READ getTemperatureKU NOTIFY temperatureKUChanged)
    int getTemperatureKU() const {return em5215Struct->TemperatureKu;}

    Q_PROPERTY(int humidity READ getHumidity  NOTIFY humidityChanged)
    int getHumidity() const {return em5215Struct->Humidity;}

    Q_PROPERTY(int temperatureBody READ getTemperatureBody NOTIFY temperatureBodyChanged)
    int getTemperatureBody() const {return em5215Struct->TemperatureBody;}

    Q_PROPERTY(int temperatureRadiator READ getTemperatureRadiator NOTIFY temperatureRadiatorChanged)
    int getTemperatureRadiator() const {return em5215Struct->TemperatureRadiator;}

    Q_PROPERTY(int fanSpeed READ getFanSpeed NOTIFY fanSpeedChanged)
    int getFanSpeed() const {return em5215Struct->FanSpeed*60;}

    Q_PROPERTY(int fanU READ getFanU NOTIFY fanUChanged)
    int getFanU() const {return em5215Struct->FanU;}

    Q_PROPERTY(int currentP1 READ getCurrentP1 NOTIFY currentP1Changed)
    int getCurrentP1() const {return em5215Struct->CurrentP1;}

    Q_PROPERTY(int currentP2 READ getCurrentP2 NOTIFY currentP2Changed)
    int getCurrentP2() const {return em5215Struct->CurrentP2;}

    Q_PROPERTY(bool isKuOn READ isKuOn NOTIFY kuOnChanged)
    bool isKuOn() const {return em5215Struct->KuOn;}

    Q_PROPERTY(bool isFanOn READ getIsFanOn NOTIFY fanOnChanged)
    bool getIsFanOn() const {return em5215Struct->FanOn;}

    Q_PROPERTY(bool isR1On READ getIsR1On NOTIFY r1OnChanged)
    bool getIsR1On() const {return em5215Struct->R1On;}

    Q_PROPERTY(bool isR2On READ getIsR2On NOTIFY r2OnChanged)
    bool getIsR2On() const {return em5215Struct->R2On;}

    Q_PROPERTY(int isP1On READ getIsP1On NOTIFY p1OnChanged)
    int getIsP1On() const {return em5215Struct->P1On;}

    Q_PROPERTY(int isP2On READ getIsP2On NOTIFY p2OnChanged)
    int getIsP2On() const {return em5215Struct->P2On;}

    Q_INVOKABLE void setAutoMode(const bool autoMode);
    Q_INVOKABLE void switchKu(const bool switchOn);
    Q_INVOKABLE void changeFanU(const quint8 u);
    Q_INVOKABLE void switchR1(const bool switchOn);
    Q_INVOKABLE void switchR2(const bool switchOn);
    Q_INVOKABLE void switchR(const bool switchOn);
    Q_INVOKABLE void switchP1(const int peltier, const int current);
    Q_INVOKABLE void switchP2(const int peltier, const int current);

    Q_INVOKABLE void startParse(const bool p) {parse=p;}

    //for calculate value in qml
    Q_INVOKABLE QList<int> getFanLimit();
    Q_INVOKABLE QList<int> getCurrentLimit();
    Q_INVOKABLE double getFanLoValue() {return FAN_VALUE_LO_LIMIT;}
    Q_INVOKABLE double getFanConst() {return (FAN_VALUE_HI_LIMIT-FAN_VALUE_LO_LIMIT)/(FAN_HI_LIMIT-FAN_LO_LIMIT);}
    Q_INVOKABLE double getCurrentLoValue() {return CURRENT_VALUE_LO_LIMIT;}
    Q_INVOKABLE double getCurrentConst() {return (CURRENT_VALUE_HI_LIMIT-CURRENT_VALUE_LO_LIMIT)/(CURRENT_HI_LIMIT-CURRENT_LO_LIMIT);}

    Q_INVOKABLE DeviceState getHumidityColor();

    Q_INVOKABLE DeviceState state() {return em5215Struct->state();}

    friend QDataStream & operator << (QDataStream & ds, EM5215 &v);
    friend QDataStream & operator >> (QDataStream & ds, EM5215 &v);
signals:
    void Message(LoggerMsg lm,  QString message);
    void executeCommand(QByteArray &ba);

    void dataExhChanged();
    void AutoModeChanged();

    void serialNumberChanged();

    void temperatureKUChanged();
    void humidityChanged();
    void temperatureBodyChanged();
    void temperatureRadiatorChanged();
    void fanSpeedChanged();
    void fanUChanged();
    void currentP1Changed();
    void currentP2Changed();
    void kuOnChanged();
    void fanOnChanged();
    void r1OnChanged();
    void r2OnChanged();
    void p1OnChanged();
    void p2OnChanged();
public slots:

    virtual void checkPortsState() override;

    //signals from em5215parser (using only in TM-2221, never in ARMC)
    void setDataFromController(EM5215Struct &newStruct);
    void dataNoChanged();

    void setAuto();
    void setManual();
    void askData();
    void poll();
    //execute command from ARMC
    void command(QByteArray &ba);
private:
    std::chrono::high_resolution_clock::time_point timeStamp;
    QByteArray com;
    bool dataExh=false;

    void newCommand(EM5215Command command, quint8 value);

    bool isCommandAvailable(EM5215Command com);
    bool isAutoAvailable(EM5215Command command);

    QVector<EM5215Struct> em5215Ring;
    EM5215Struct* em5215Struct;
    int RingCounter;
    int OppositeCounter() {return em5215Ring.size()-RingCounter-1;}
    void swapCounter();
    void setDataFromUDP(QDataStream &ds);
    void checkDataAndEmit();
};


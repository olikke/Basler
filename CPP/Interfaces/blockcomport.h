#pragma once

#include <QObject>
#include <QSerialPort>
#include "../CPP/log.h"

enum class ComPortErrors
{
    CantOpenCom,
    Timeout,
    ErrorWrite
};
Q_DECLARE_METATYPE(ComPortErrors)

class BlockComPort : public QObject
{
    Q_OBJECT
public:
    BlockComPort(QString comPort, QObject *parent = nullptr);
signals:
    void Message(LoggerMsg lm,  QString message);
    void answer(QByteArray &ba);
    void comPortError(ComPortErrors err);
public slots:
    void executeCommand(QByteArray ba);


private:
    QSerialPort* serialPort;
    quint8 timeout;
    void openCom();
    QString comPortName;
};

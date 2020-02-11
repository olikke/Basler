#include "blockcomport.h"

BlockComPort::BlockComPort(QString comPort,QObject *parent) :
    QObject(parent),
    serialPort(new QSerialPort(this)),
    comPortName(comPort)
{
    openCom();
}

void BlockComPort::executeCommand(QByteArray ba)
{
    if (serialPort->isOpen())
    {
       serialPort->write(ba);
       serialPort->readAll(); //предварительно почистили входной буфер
       if (serialPort->waitForBytesWritten(timeout))
       {
           if  (serialPort->waitForReadyRead(timeout))
           {
               QByteArray answ=serialPort->readAll();
               while (serialPort->waitForReadyRead(5))  //на мелком компе больше 8 байт  не проходит за 1 раз
               {
                   answ+=serialPort->readAll();
               }
               emit answer(answ);
           }
           else
           {
               emit Message(LoggerMsg::Warning,QString("COM %1: Тайм-аут ответа").arg(serialPort->portName()));
               emit comPortError(ComPortErrors::Timeout);
           }
       }
       else
       {
           emit Message(LoggerMsg::Warning,QString("COM %1: Ошибка записи в порт %2").arg(
                            serialPort->portName(),
                            serialPort->errorString()));
           emit comPortError(ComPortErrors::ErrorWrite);
      }
    }
    else openCom();
}

void BlockComPort::openCom()
{
    static quint8 comPortStillNotOpen=0;
    serialPort->setPortName(comPortName);
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    timeout=50;
    if (!serialPort->open(QIODevice::ReadWrite))
    {
        comPortStillNotOpen++;
        if (comPortStillNotOpen>=100)
        {
            emit Message(LoggerMsg::Info,QString("COM %1: Не удалось открыть СОМ-порт ").arg(serialPort->portName())+serialPort->errorString());
            comPortStillNotOpen=0;
        }
    }   else
    {
        emit Message(LoggerMsg::Info,QString("COM %1: СОМ-порт открыт.").arg(serialPort->portName()));
        emit comPortError(ComPortErrors::CantOpenCom);
        comPortStillNotOpen=0;
    }


}

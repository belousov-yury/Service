#ifndef MODBUS_H
#define MODBUS_H
#include "uart.h"
#include <QObject>

class ModBus : public QObject
{
    Q_OBJECT
public:
    explicit ModBus(QObject *parent = 0);
    UART    *uartObj;
    struct  modBusDataStruct
    {
      quint32 sender;
      quint32 receiver;
      int typeFrame;
      QByteArray data;
    };
signals:

    void    sendUART(QByteArray data);
    void    sendToDC(modBusDataStruct data);
public slots:
    void    readToUART(QByteArray data);
    void    readToDC(modBusDataStruct data);

private:
    void        parsingToData(QByteArray data);
    void        parsingToPackege(modBusDataStruct str);
    QByteArray  crc8(QByteArray seed, QByteRef inData);
    QByteArray  ControlSum(QByteArray str);

    modBusDataStruct modBusDataObj;
};

#endif // MODBUS_H

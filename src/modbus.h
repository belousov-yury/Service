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
    struct  modBusDataStruct//структура данных
    {
      quint32 sender;//id оправителя
      quint32 receiver;//id получателя
      quint8 typeFrame;//тип кадра (0х00 -> Кадр данных, 0x01 -> Кадр подтверждения, 0x02 -> Кадр команды)
      QByteArray data;//данные
    };
signals:

    void    sendUART(QByteArray data);//отправка данны в UART
    void    sendToDC(modBusDataStruct data);//отправка данны в DCunit
public slots:
    void    readToUART(QByteArray data);//прием данных из UART
    void    readToDC(modBusDataStruct data);//прием данных из DCunit

private:
    QByteArray  parsingIdSenderToId4byte(quint32 id);//парсинг id из int в 4 байта
    void        parsingToData(QByteArray data);//парсиг из пакакета, который пришел из СОМ-порта, в понятные данные
    void        parsingToPackege(modBusDataStruct str);//парсинг данных в пакет для передачи в СОМ-порт
    QByteArray  crc8(QByteArray seed, QByteRef inData);
    QByteArray  ControlSum(QByteArray str);//составление контрольной суммы
    QByteArray  fillingFieldData(int size);//заполнение данных нулями(поле 22 байта, но оно не всегда  заполнено данными)
    QByteArray  crc;
};

#endif // MODBUS_H

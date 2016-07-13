#ifndef DCUNIT_H
#define DCUNIT_H

#include <QObject>
#include "device.h"
#include "modbus.h"
#include "networkmanager.h"
#include <QJsonObject>
#include <QJsonDocument>

class DCunit : public QObject
{
    Q_OBJECT
public:
    explicit DCunit(QObject *parent = 0);
    QList <Device*>  *deviceList;
    QList <GPIO*>  *gpioList;
    NetworkManager *nmObj;
    ModBus   *modBus;
    void    addDevice(int idDevice, int interfaceNumber);//Добавить Slave-модем
    void    addChild(int idDevice, int idGPIO, int pinSignal,int pinGround, bool mode, QString deviceName);//Добавить датчик на Slave-модем
    void    deleteDevice(int idDevice);
    void    deleteChild(int idDevice,int idGPIO);
    void    updateDevice(int idDevice, int interfaceNumber);
    void    updateChild(int idDevice, int idGPIO, int pinSignal, int pinGround, bool mode, QString deviceName);

    void    getDateTimePeriod(); //считать текущие дату, время, период опроса
    void    getSerialNumber();   //считать серийный номер CC1310 (номер модема)
    void    getCNTRS();          //прочитать значения всех счётчиков, состояний всех входов
    void    getPins();           //считать конфигурацию входов модема.
    void    getTempVolt();       //прочитать значение температуры корпуса микросхемы и напряжение питания

    void    setPins();           //задать конфигурацию входов модема - задаются сразу все 8 входов.
    void    clearCNTR();         //обмен закончен, можно сразу ложиться спать

    void    dataType(ModBus::modBusDataStruct data);
    void    responseType(ModBus::modBusDataStruct data);
    void    commandType(ModBus::modBusDataStruct data);
signals:

    void    sendToModBus(ModBus::modBusDataStruct data);
    void    sendToNM(QByteArray data);

public slots:

    void    readToModBus(ModBus::modBusDataStruct data);

    void    readToNM(QByteArray data);

private:
    QByteArray receiverIdGeneration();
    QByteArray parsingIdSenderToId4byte(int id);
    QByteArray fillingFieldData(int size);
    struct setAlarmStruct
    {
        QByteArray dayCurrent;
        QByteArray monthCurrent;
        QByteArray yearCurrent;
        QByteArray hourCurrent;
        QByteArray minuteCurrent;
        QByteArray secondCurrent;
        QByteArray dayAlarm;
        QByteArray monthAlarm;
        QByteArray yearAlarm;
        QByteArray hourAlarm;
        QByteArray minuteAlarm;
        QByteArray secondAlarm;
    };



};

#endif // DCUNIT_H

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

    NetworkManager *nmObj;
    ModBus   *modBus;
    Device   *device;


signals:

    void    sendToModBus(ModBus::modBusDataStruct data);
    void    sendToNM(QByteArray data);

public slots:

    void    readToModBus(ModBus::modBusDataStruct data);
    void    readToNM(QByteArray data);

private:

    bool    status;//true -> wait , false ->busy статус на разрешение получения данных от NetworkManager

    void    dataType(ModBus::modBusDataStruct data);
    void    responseType(ModBus::modBusDataStruct data);
    void    commandType(ModBus::modBusDataStruct data);

//    struct setAlarmStruct
//    {
//        Byte dayCurrent;
//        Byte monthCurrent;
//        Byte yearCurrent;
//        Byte hourCurrent;
//        Byte minuteCurrent;
//        Byte secondCurrent;
//        Byte dayAlarm;
//        Byte monthAlarm;
//        Byte yearAlarm;
//        Byte hourAlarm;
//        Byte minuteAlarm;
//        Byte secondAlarm;
//    };



};

#endif // DCUNIT_H

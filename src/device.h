#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "gpio.h"
#include <QJsonObject>
#include <QJsonDocument>
class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = 0);

    struct  modBusDataStruct//структура данных
    {
      quint32 sender;//id оправителя
      quint32 receiver;//id получателя
      quint8 typeFrame;//тип кадра (0х00 -> Кадр данных, 0x01 -> Кадр подтверждения, 0x02 -> Кадр команды)
      QByteArray data;//данные
    };

    QList <Device*>  *deviceList;
    QList <GPIO*>    *gpioList;


    int getInterfaceNumber() const;
    void setInterfaceNumber(int value);

    int getIdDevice() const;
    void setIdDevice(int value);

    int getAdress() const;
    void setAdress(int value);

    bool getStatus() const;
    void setStatus(bool value);

    void    addDevice(int idDevice, int interfaceNumber);//Добавить Slave-модем
    void    addChild(int idDevice, int idGPIO, int pinSignal, bool mode, QString deviceName);//Добавить датчик на Slave-модем
    void    deleteDevice(int idDevice);
    void    deleteChild(int idDevice,int idGPIO);
    void    updateDevice(int idDevice, int interfaceNumber);
    void    updateChild(int idDevice, int idGPIO, int pinSignal, int pinGround, bool mode, QString deviceName);

    void    initConfirmation();//подтверждение инициализации(сигнал маодема "wake up")
signals:

public slots:
    void        readToDC(QByteArray data);
    void        readToModBus(modBusDataStruct data);//парсер ответов модема, т.е. что ответил модем c UART
private:
    //-------параметры модема-Slave--------------
    int         idDevice;//id модема-Slave
    int         adress;//адрес модема-Slave
    int         interfaceNumber;//количество возможных подключаемых датчиков и счетчиков
    bool        status;//true -> ok, false -> wait(ждет ответа модема-slave)
    //------------------------------------------
    quint32 receiverIdGeneration();// функция для генерации номера id сервера, в дальнейшем доработать




};

#endif // DEVICE_H

#include "device.h"
#include "gpio.h"
#include <QDebug>
Device::Device(QObject *parent) :
    QObject(parent)
{

}
void Device::readToModBus(modBusDataStruct data)//парсер ответов модема, т.е. что ответил модем
{
    switch (data.typeFrame) {
    case 0x00://pData
        dataType(data);
        break;
    case 0x01://pResponse
        responseType(data);
       break;
    case 0x02://pCommand
        commandType(data);
        break;
    default:
        break;
    }
}



quint32 Device::receiverIdGeneration()//функция для генерации номера id сервера, в дальнейшем доработать
{
    return 13;
}
//----------------------------------------------------------------------------------------------------------
void Device::addDevice(int idDevice, int interfaceNumber)//добавить объект девайса(Slave-модема)
{
    Device *device = new Device(this);
    device->setIdDevice(idDevice);
    device->setAdress(0);
    device->setInterfaceNumber(interfaceNumber);
    device->setAdress(0);
    device->setStatus(false);
    deviceList->append(device);
    qDebug()<< "addDevice "<< idDevice;
}

void Device::addChild(int idDevice, int idGPIO, int pinSignal,
                      bool mode, QString deviceName)//добавить объект датчика или счетчика
{
    GPIO *gpioObj = new GPIO(this);
    gpioObj->setIdDevice(idDevice);
    gpioObj->setIdGPIO(idGPIO);
    gpioObj->setPinSignal(pinSignal);
    gpioObj->setMode(mode);
    gpioObj->setDeviceName(deviceName);
    gpioList->append(gpioObj);
    qDebug()<< "addChild" << idGPIO;

}
void Device::deleteDevice(int idDevice)//удалить
{
    int i = 0;
    while( deviceList->at(i)->getIdDevice() !=  idDevice)
    {
        i++;
    }
    deviceList->at(i)->deleteLater();
    deviceList->removeAt(i);
}

void Device::deleteChild(int idDevice, int idGPIO)//удалить
{
    int i = 0;
    while( gpioList->at(i)->getIdDevice() !=  idDevice && gpioList->at(i)->getIdGPIO() !=  idGPIO )
    {
        i++;
    }
    gpioList->at(i)->deleteLater();
    gpioList->removeAt(i);
}
void Device::updateDevice(int idDevice, int interfaceNumber)
{
    int i = 0;
    while( deviceList->at(i)->getIdDevice() !=  idDevice)
    {
        i++;
    }
     deviceList->at(i)->setIdDevice(idDevice);
     deviceList->at(i)->setInterfaceNumber(interfaceNumber);
     qDebug()<<"updateDevice id = " << idDevice << "is done";
}

void Device::updateChild(int idDevice, int idGPIO, int pinSignal, int pinGround, bool mode, QString deviceName)
{
    int i = 0;
    while( gpioList->at(i)->getIdDevice() !=  idDevice && gpioList->at(i)->getIdGPIO() !=  idGPIO )
    {
        i++;
    }
    gpioList->at(i)->setIdDevice(idDevice);
    gpioList->at(i)->setIdGPIO(idGPIO);
    gpioList->at(i)->setPinSignal(pinSignal);
    gpioList->at(i)->setMode(mode);
    gpioList->at(i)->setDeviceName(deviceName);
    qDebug()<<"updateChild id = " << idGPIO << "is done";
}

void Device::initConfirmation()
{
    int i = 0;
    while (deviceList->at(i)->getStatus() != false)
    {
        i++;
    }
    deviceList->at(i)->setAdress( deviceList->at(i)->getIdDevice());
    deviceList->at(i)->setStatus(true);
    QByteArray data,adress;
    //data.append(команда на установку id модема-Slave)
    QDataStream stream(&adress, QIODevice::WriteOnly);
    stream << id;
    data.append(adress);
    //спросить на какой id отправлять установку нового id
}

void Device::readToDC(QByteArray data)
{

}
//----------------------сетеры гетеры---------------------------------------------------------------

int Device::getInterfaceNumber() const
{
    return interfaceNumber;
}

void Device::setInterfaceNumber(int value)
{
    interfaceNumber = value;
}
int Device::getIdDevice() const
{
    return idDevice;
}

void Device::setIdDevice(int value)
{
    idDevice = value;
}
int Device::getAdress() const
{
    return adress;
}

void Device::setAdress(int value)
{
    adress = value;
}
bool Device::getStatus() const
{
    return status;
}

void Device::setStatus(bool value)
{
    status = value;
}
//--------------------------------------------------------------------------------------------------

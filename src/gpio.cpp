#include "gpio.h"
#include <QDebug>
GPIO::GPIO(QObject *parent) :
    QObject(parent)
{

}
int GPIO::getPinSignal() const
{
    return pinSignal;
}

void GPIO::setPinSignal(int value)
{
    pinSignal = value;
}
bool GPIO::getMode() const
{
    return mode;
}

void GPIO::setMode(bool value)
{
    mode = value;
}
QString GPIO::getDeviceName() const
{
    return deviceName;
}

void GPIO::setDeviceName(const QString &value)
{
    deviceName = value;
}
int GPIO::getIdGPIO() const
{
    return idGPIO;
}

void GPIO::setIdGPIO(int value)
{
    idGPIO = value;
}
int GPIO::getIdDevice() const
{
    return idDevice;
}

void GPIO::setIdDevice(int value)
{
    idDevice = value;
}








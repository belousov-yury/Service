#include "device.h"
#include <QDebug>
Device::Device(QObject *parent) :
    QObject(parent)
{

}


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



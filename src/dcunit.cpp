#include "dcunit.h"
#include <QDebug>
DCunit::DCunit(QObject *parent) :
    QObject(parent)
{
    nmObj = new NetworkManager (9090,"localhost");
    modBus = new ModBus;

    connect(nmObj,SIGNAL(sendToDC(QByteArray)),this,SLOT(readToNM(QByteArray)));
    connect(this,SIGNAL(sendToNM(QByteArray)),nmObj,SLOT(readToDC(QByteArray)));
    //------

    connect(this,SIGNAL(sendToModBus(ModBus::modBusDataStruct)),modBus,SLOT(readToDC(ModBus::modBusDataStruct)));
    connect(modBus,SIGNAL(sendToDC(ModBus::modBusDataStruct)),this,SLOT(readToModBus(ModBus::modBusDataStruct)));

}
void DCunit::readToModBus(ModBus::modBusDataStruct data)//парсер ответов модема, т.е. что ответил модем
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
void DCunit::dataType(ModBus::modBusDataStruct data)//(Кадр данных)парсер ответов модема, т.е. что ответил модем. код кадра 0х00
{

}

void DCunit::responseType(ModBus::modBusDataStruct data)//(Кадр подтверждение)парсер ответов модема, т.е. что ответил модем. код кадра 0х01
{
    switch (data.data[0]) {
    case 0x00://команда выполнена
        qDebug() << "command is done";
        break;
    case 0x01://модем проснулся
         qDebug() << "modem woke up";
        break;
    case 0xCC://подтверждение ухода в режим сна //этот ответ не нужен, он повторяет код 0х00 (примечание Антона)
        qDebug() << "modem asleep";
        break;
    case 0xB0://тип таймера "минутный" (задаём интервал в минутах 1..1440)
        qDebug() << "type of timer - minute";
        break;
    case 0xB1://тип таймера "календарный" (задано число месяца и время в этот день)
        qDebug() << "type of timer - calendar";
        break;
    case 0xFF: //ошибка выполнения
        qDebug() << "error";
        break;
    default:
        break;
    }
}

void DCunit::commandType(ModBus::modBusDataStruct data)//(Кадр команды)парсер ответов модема, т.е. что ответил модем. код кадра 0х01
{
    switch (data.typeFrame) {
    case 0x01://задать конфигурацию входов модема - задаются сразу все 8 входов.
        {
        }
        break;

    case 0x02://задать таймер "календарный"
        {
        }
        break;

    case 0x03://задать таймер "минутный"(max 1440)
        {
        }
        break;

    case 0x04://считать текущие дату, время, период опроса
        {

        }
        break;

    case 0x05://считать серийный номер CC1310 (номер модема)
        {
        }
        break;

    case 0x06://прочитать значения всех счётчиков, состояний всех входов
        {
        }
        break;

    case 0x07://считать конфигурацию входов модема.
        {
        }
        break;

    case 0x08://прочитать значение температуры корпуса микросхемы и напряжение питания
        {
        }
        break;

    case 0x09://обнулить значение счётчика номер N
        {
        }
        break;

    case 0xFF://Команда "уснуть"
        {
        }
        break;

    default:
        break;
    }
}

void DCunit::readToNM(QByteArray data)//прием данных от калсса NetworkManager
{
    ModBus::modBusDataStruct dataStruct;
    QJsonDocument doc  = QJsonDocument::fromJson(data) ;
    QJsonObject obj = doc.object();

    dataStruct.sender.append(parsingIdSenderToId4byte(obj.value("idDevice").toInt()));
    dataStruct.receiver.append(receiverIdGeneration());
    dataStruct.typeFrame[0] = 0x02;

    int command = obj.value("Command").toInt();
    qDebug() << QString::number(command,16);

    switch (command) {
    case 1://0x01://задать конфигурацию входов модема - задаются сразу все 8 входов.
        {
        dataStruct.data.append(0x01);
        }
        break;

    case 2://0x02://задать таймер "календарный"
        {
        dataStruct.data.append(0x02);
        }
        break;

    case 3://0x03://задать таймер "минутный"(max 1440)
        {
        dataStruct.data.append(0x03);
        dataStruct.data.append(parsingIdSenderToId4byte(obj.value("Minute").toInt()).mid(2,2));
        }
        break;

    case 4://0x04://считать текущие дату, время, период опроса
        {
        dataStruct.data.append(0x04);
        }
        break;

    case 5://0x05://считать серийный номер CC1310 (номер модема)
        {
        dataStruct.data.append(0x05);
        }
        break;

    case 6://0x06://прочитать значения всех счётчиков, состояний всех входов
        {
         dataStruct.data.append(0x06);
        }
        break;

    case 7://0x07://считать конфигурацию входов модема.
        {
        dataStruct.data.append(0x07);
        }
        break;

    case 8://0x08://прочитать значение температуры корпуса микросхемы и напряжение питания
        {
        dataStruct.data.append(0x08);
        }
        break;

    case 9://0x09://обнулить значение счётчика номер N
        {
        dataStruct.data.append(0x09);
        }
        break;

    case 255://0xFF://Команда "уснуть"
        {
        dataStruct.data[0xFF];
        }
        break;

    default:
        break;
    }
    dataStruct.data.append(fillingFieldData(dataStruct.data.size()));
    emit sendToModBus(dataStruct);
}

QByteArray DCunit::receiverIdGeneration()// функция для генерации номера id сервера, в дальнейшем доработать
{
    QByteArray a;
    a[0] = 0x00;
    a[1] = 0x00;
    a[2] = 0x00;
    a[3] = 0x01;
    return a;
}

QByteArray DCunit::parsingIdSenderToId4byte(int id)//парсинг id из int в 4 байта
{
    QByteArray out;
    QDataStream stream(&out, QIODevice::WriteOnly);
    stream << id;
    return out;
}

QByteArray DCunit::fillingFieldData(int size)//заполнение данных нулями(23 байта поле, но не всегда оно заполнено данными)
{
    QByteArray out;
 for(int i = 0; i < 23 - size; i ++ )
 {
     out[i] = 0x00;
 }
 return out;
}
//-----------------------------------------------------
void DCunit::addDevice(int idDevice, int interfaceNumber)//добавить девайс(Slave-модем)
{
    Device *device = new Device(this);
    device->setIdDevice(idDevice);
    device->setInterfaceNumber(interfaceNumber);
    deviceList->append(device);
    qDebug()<< "addDevice "<< idDevice;
}

void DCunit::addChild(int idDevice, int idGPIO, int pinSignal,int pinGround, bool mode, QString deviceName)//добавить датчик или счетчик
{
    GPIO *gpioObj = new GPIO(this);
    gpioObj->setIdDevice(idDevice);
    gpioObj->setIdGPIO(idGPIO);
    gpioObj->setPinSignal(pinSignal);
    gpioObj->setPinGround(pinGround);
    gpioObj->setMode(mode);
    gpioObj->setDeviceName(deviceName);
    gpioList->append(gpioObj);
    qDebug()<< "addChild" << idGPIO;

}
void DCunit::deleteDevice(int idDevice)//удалить
{
    int i = 0;
    while( deviceList->at(i)->getIdDevice() !=  idDevice)
    {
        i++;
    }
    deviceList->at(i)->deleteLater();
    deviceList->removeAt(i);
}

void DCunit::deleteChild(int idDevice, int idGPIO)//удалить
{
    int i = 0;
    while( gpioList->at(i)->getIdDevice() !=  idDevice && gpioList->at(i)->getIdGPIO() !=  idGPIO )
    {
        i++;
    }
    gpioList->at(i)->deleteLater();
    gpioList->removeAt(i);
}
void DCunit::updateDevice(int idDevice, int interfaceNumber)
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

void DCunit::updateChild(int idDevice, int idGPIO, int pinSignal, int pinGround, bool mode, QString deviceName)
{
    int i = 0;
    while( gpioList->at(i)->getIdDevice() !=  idDevice && gpioList->at(i)->getIdGPIO() !=  idGPIO )
    {
        i++;
    }
    gpioList->at(i)->setIdDevice(idDevice);
    gpioList->at(i)->setIdGPIO(idGPIO);
    gpioList->at(i)->setPinSignal(pinSignal);
    gpioList->at(i)->setPinGround(pinGround);
    gpioList->at(i)->setMode(mode);
    gpioList->at(i)->setDeviceName(deviceName);
    qDebug()<<"updateChild id = " << idGPIO << "is done";
}
//-----------------------------------------------------------

#include "dcunit.h"
#include <QDebug>
DCunit::DCunit(QObject *parent) :
    QObject(parent)
{
    status = true;
    device = new Device(this);
    nmObj = new NetworkManager (9090,"localhost");
    modBus = new ModBus;

    connect(nmObj,SIGNAL(sendToDC(QByteArray)),device,SLOT(readToNM(QByteArray)));
    connect(this,SIGNAL(sendToNM(QByteArray)),nmObj,SLOT(readToDC(QByteArray)));
    //------

    connect(this,SIGNAL(sendToModBus(ModBus::modBusDataStruct)),modBus,SLOT(readToDC(ModBus::modBusDataStruct)));
    connect(modBus,SIGNAL(sendToDC(ModBus::modBusDataStruct)),this,SLOT(readToModBus(ModBus::modBusDataStruct)));

}

void DCunit::readToNM(QByteArray data)//прием данных от калсса NetworkManager
{
    QJsonDocument doc  = QJsonDocument::fromJson(data) ;
    QJsonObject obj = doc.object();

    int command = obj.value("Command").toInt();
    switch (command) {
    case 100://добавить устройство(Slave-modem)
        status = false;
        device->addDevice(obj.value("idDevice").toInt(),obj.value("interfaceNumber").toInt());
        break;
    case 101://добавить датчик или счетчик к устройству(Slave-modem)

        break;
    case 102://удалить устройство(Slave-modem)

        break;
    case 103://удалить датчик или счетчик с устройства(Slave-modem)

        break;
    case 104://обновить данные устройства(Slave-modem)

        break;
    case 105://обновить данные датчика или счетчика на устройстве(Slave-modem)

        break;
    default:
        break;
    }
}
void DCunit::readToModBus(ModBus::modBusDataStruct data)
{
    switch (data.typeFrame) {//проверка на тип кадра
    case 0://pData

        break;
    case 1://pResponse

       break;
    case 2://pCommand

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
         if (data.receiver == 0)
         {
            device->initConfirmation();
            status = true;
         }
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
    switch (data.data[0]) {
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
//    switch (command) {
//    case 1://0x01://задать конфигурацию входов модема - задаются сразу все 8 входов.
//        {

//        }
//        break;

//    case 2://0x02://задать таймер "календарный"
//        {

//        }
//        break;

//    case 3://0x03://задать таймер "минутный"(max 1440)
//        {


//        }
//        break;

//    case 4://0x04://считать текущие дату, время, период опроса
//        {

//        }
//        break;

//    case 5://0x05://считать серийный номер CC1310 (номер модема)
//        {

//        }
//        break;

//    case 6://0x06://прочитать значения всех счётчиков, состояний всех входов
//        {

//        }
//        break;

//    case 7://0x07://считать конфигурацию входов модема.
//        {

//        }
//        break;

//    case 8://0x08://прочитать значение температуры корпуса микросхемы и напряжение питания
//        {

//        }
//        break;

//    case 9://0x09://обнулить значение счётчика номер N
//        {

//        }
//        break;

//    case 255://0xFF://Команда "уснуть"
//        {

//        }
//        break;

//    default:
//        break;
//    }

//    emit sendToModBus();



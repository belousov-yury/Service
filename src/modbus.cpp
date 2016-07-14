#include "modbus.h"
#include <QDebug>

ModBus::ModBus(QObject *parent) :
    QObject(parent)
{
    uartObj = new UART;
    connect(this,SIGNAL(sendUART(QByteArray)),uartObj,SLOT(readToModBus(QByteArray)));
    connect(uartObj,SIGNAL(sendModBus(QByteArray)),this,SLOT(readToUART(QByteArray)));
}

void ModBus::readToUART(QByteArray data)
{
    qDebug()<<data;
    parsingToData(data);
}

void ModBus::readToDC(ModBus::modBusDataStruct data)
{
    parsingToPackege(data);
}

void ModBus::parsingToData(QByteArray data)
{//проверка crc
//  crc.append(data.right(1));
//  if(crc == ControlSum(data.left(31)))
//    {
    modBusDataStruct modBusDataObj;

    QDataStream sender(data.left(4));
    sender >> modBusDataObj.sender;

    QDataStream receiver(data.mid(4,4));
    receiver >> modBusDataObj.receiver;

    QDataStream typeFrame(data.mid(8,1));
    typeFrame >> modBusDataObj.typeFrame;

    modBusDataObj.data.append(data.mid(9,22));

    emit sendToDC(modBusDataObj);
//    }
//    else
//    {
//        qDebug() << "Error control sum!";
//    }
}

void ModBus::parsingToPackege(modBusDataStruct str)
{
    QByteArray data;
    data.append(parsingIdSenderToId4byte(str.sender));
    data.append(str.receiver);
    data.append(str.typeFrame);
    data.append(str.data);
   // data[31] = ControlSum(data);
    emit sendUART(data);
}
QByteArray ModBus::parsingIdSenderToId4byte(quint32 id)//парсинг id из int в 4 байта
{
    QByteArray out;
    QDataStream stream(&out, QIODevice::WriteOnly);
    stream << id;
    return out;
}

QByteArray ModBus::fillingFieldData(int size)//заполнение данных нулями(поле 22 байта, но оно не всегда  заполнено данными)
{
    QByteArray out;
    for(int i = 0; i < 23 - size; i ++ )
    {
        out[i] = 0x00;
    }
    return out;
}
//QByteArray ModBus::ControlSum(QByteArray str)
//{
//    QByteArray crc;
//    crc = 0;
//    for ( int i = 0; i < str.size()-2 ; i++ )
//        {
//            crc = crc8(crc, str[i]);
//        }
//    return crc;
//}
//QByteArray crc8(QByteArray seed, QByteArray inData)
//{
//    int bitsLeft = 0;
//    int tempp = 0;

//    for (bitsLeft = 8; bitsLeft > 0; bitsLeft--)
//    {
//       tempp = ((seed ^ inData) & 0x01);
//       if (tempp == 0) seed.toInt() >> 1;
//       else
//       {
//           seed ^= 0x18;
//           seed >>= 1;
//           seed |= 0x80;
//       }
//       inData >> 1;
//    }
//    return seed;
//}

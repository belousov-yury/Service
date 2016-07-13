#include "uart.h"
#include <QDebug>
#include <QEvent>
UART::UART(QObject *parent) :
    QObject(parent)
{
    isConnected = false;
    serialPort = new QSerialPort;
    settings = new QSettings("parametrsPort",QSettings::IniFormat);
    loadSettingsPort();

    //writeSettingsPort("COM20",9600,8,0,1,0,3);
    testt();
    connectPort();
    //connect(serialPort,SIGNAL(readyRead()),this,SLOT(readInPort()));
}

void UART :: textDisplay(QString str)
{
    qDebug()<<str;
}

void UART :: connectPort(void)
{
    serialPort->setPortName(settingsPort.portName);
    if (serialPort->open(QIODevice::ReadWrite))
        {
            if (serialPort->setBaudRate(settingsPort.baudRate)
                    && serialPort->setDataBits(settingsPort.dataBits)
                    && serialPort->setParity(settingsPort.parity)
                    && serialPort->setStopBits(settingsPort.stopBits)
                    && serialPort->setFlowControl(settingsPort.flowControl))
                {
                    if (serialPort->isOpen())
                    {
                        isConnected = true;
                        textDisplay(settingsPort.portName + " is Open!\r");
                    }
                }
            else
                {
                serialPort->close();
                textDisplay("Error");
                }
        }
    else
        {
        serialPort->close();
        textDisplay("Error connected");
        }
}

void UART :: writeSettingsPort(QString name, int baudrate, int dataBits, int parity, int stopBits, int flowControl, int numberError)
{
        settingsPort.portName       = name;
        settingsPort.baudRate       = (QSerialPort::BaudRate) baudrate;
        settingsPort.dataBits       = (QSerialPort::DataBits) dataBits;
        settingsPort.parity         = (QSerialPort::Parity) parity;
        settingsPort.stopBits       = (QSerialPort::StopBits) stopBits;
        settingsPort.flowControl    = (QSerialPort::FlowControl) flowControl;
        settingsPort.numberError    = numberError;
        saveSettingsPort();
}

void UART :: loadSettingsPort()
{
    settings->beginGroup("parametrsPort");
    settingsPort.portName       = (settings->value("PortName","COM1")).toString();
    settingsPort.baudRate       = (QSerialPort::BaudRate)(settings->value("BaudRate",9600)).toInt();
    settingsPort.dataBits       = (QSerialPort::DataBits)(settings->value("DataBits",8)).toInt();
    settingsPort.parity         = (QSerialPort::Parity)(settings->value("Parity",0)).toInt();
    settingsPort.stopBits       = (QSerialPort::StopBits)(settings->value("StopBits",1)).toInt();
    settingsPort.flowControl    = (QSerialPort::FlowControl)(settings->value("FlowControl",0)).toInt();
    settingsPort.numberError    = (settings->value("NumberError",5)).toInt();
    settings->endGroup();
}
void UART :: saveSettingsPort()
{
    settings->beginGroup("parametrsPort");
    settings->setValue("PortName", settingsPort.portName);
    settings->setValue("BaudRate", settingsPort.baudRate);
    settings->setValue("DataBits", settingsPort.dataBits);
    settings->setValue("Parity",   settingsPort.parity);
    settings->setValue("StopBits", settingsPort.stopBits);
    settings->setValue("FlowControl", settingsPort.flowControl);
    settings->setValue("NumberError", settingsPort.numberError);
    settings->endGroup();
    settings->sync();
}

void UART :: testt()
{
    qDebug()<< settingsPort.portName
             <<settingsPort.baudRate
             <<settingsPort.dataBits
             <<settingsPort.parity
             <<settingsPort.stopBits
             <<settingsPort.flowControl
             <<settingsPort.numberError;
}
void UART :: sendToPort(QByteArray data)
{

     serialPort->write(data);
     serialPort->waitForBytesWritten(500);
     this->readInPort();

 // if(serialPort->isOpen())
 //     {
 //         serialPort->write(data);
 //         this->data.clear();
     //     }
}

void UART::readToModBus(QByteArray data)
{
    this->sendToPort(data);
}

void UART :: readInPort()
{
    QByteArray data;

    if(serialPort->waitForReadyRead(10000))
    {
       data =  serialPort->readAll();

               while(serialPort->waitForReadyRead(500))
               {
                   data+=serialPort->readAll();
               }
    }

   // qDebug()<<data;
    sendModBus(data);
}

void UART :: disconnectPort()
{
    if(serialPort->isOpen())
    {
            serialPort->close();
            isConnected = false;
            textDisplay(settingsPort.portName + " is CLose!\r");
    }
}


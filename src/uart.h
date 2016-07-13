#ifndef UART_H
#define UART_H
#include <QtSerialPort/QtSerialPort>
#include <QObject>
#include <QSettings>

class UART : public QObject
{
    Q_OBJECT
public:
    explicit UART(QObject *parent = 0);


    QSerialPort     *serialPort;
    QSettings       *settings;
    void            textDisplay(QString str);
signals:
    void        dataRead();
    void        test();
    void        sendModBus(QByteArray str);
public slots:
    void        disconnectPort();
    void        connectPort(void);
    void        writeSettingsPort(QString name, int baudrate, int DataBits, int Parity, int stopBits, int flowControl, int numberError);
    void        sendToPort(QByteArray data);
    void        readToModBus(QByteArray data);

private:
    struct Settings
    {
        QString                     portName;
        quint32                     baudRate;
        QSerialPort::DataBits       dataBits;
        QSerialPort::Parity         parity;
        QSerialPort::StopBits       stopBits;
        QSerialPort::FlowControl    flowControl;
        int                         numberError;
    };
    Settings    settingsPort;
    bool        isConnected;
    void        loadSettingsPort();
    void        saveSettingsPort();
    void        testt();
    //QByteArray  data;

private slots:
    void        readInPort();
};

#endif // UART_H

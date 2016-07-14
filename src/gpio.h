#ifndef GPIO_H
#define GPIO_H

#include <QObject>

class GPIO : public QObject
{
    Q_OBJECT
public:
    explicit GPIO(QObject *parent = 0);



    int getPinSignal() const;
    void setPinSignal(int value);

    bool getMode() const;
    void setMode(bool value);

    QString getDeviceName() const;
    void setDeviceName(const QString &value);

    int getIdGPIO() const;
    void setIdGPIO(int value);

    int getIdDevice() const;
    void setIdDevice(int value);

signals:

public slots:
private:
    int         idGPIO;
    int         idDevice;
    int         pinSignal;
    int         mode;// = >
// = >00 - вывод незадействован
//    01 - датчик дыма с нормально-замкнутыми контактами
//    02 - датчик дыма с нормально-разомкнутыми контактами
//    02 - датчик протечки воды
//    03 - импульсный счётчик воды
//    03 - импульсный счётчик газа
//    03 - импульсный счётчик электричества
//    04 - термометр
    QString     deviceName;

};

#endif // GPIO_H

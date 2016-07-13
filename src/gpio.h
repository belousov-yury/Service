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

    int getPinGround() const;
    void setPinGround(int value);

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
    int         pinGround;
    bool        mode;//true -> in, false -> out
    QString     deviceName;

};

#endif // GPIO_H

#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "gpio.h"

#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = 0);

    int getInterfaceNumber() const;
    void setInterfaceNumber(int value);

    int getIdDevice() const;
    void setIdDevice(int value);

signals:

public slots:
private:
    int         idDevice;
    int         interfaceNumber;
};

#endif // DEVICE_H

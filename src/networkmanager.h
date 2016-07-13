#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QJsonArray>

class NetworkManager : public QObject
{
    Q_OBJECT

public:

    NetworkManager(int nPort, QString nameConnect);
    void            loadSettings();
    void            saveSettings();
    void            showSet();
    ~NetworkManager();

signals:
    void    sendToDC(QByteArray data);

public slots:

    virtual void    slotNewConnection();
            void    slotRead();
            void    status();
            void    slotError(QAbstractSocket::SocketError err);
            void    slotConnected();
            void    connectToClient();
            void    readToDC(QByteArray data);

private:

    QSettings       *settings;
    struct          settingNet
    {
        int         Port;
        QString     address;
    };
    settingNet      setting;
    QTcpServer      *tcpServer;
    QTcpSocket      *socket;
    quint16         blockSize;
    void            SendData(QByteArray data);
    QHash<QTcpSocket*, QByteArray>  buffers;
    QHash<QTcpSocket*, qint32*>     sizes;
    quint16     m_nNextBlockSize;

};

#endif // NETWORKMANAGER_H

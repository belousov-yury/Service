#include "networkmanager.h"

NetworkManager::NetworkManager(int nPort, QString nameConnect)
{
    settings = new QSettings("NetworkSettings", QSettings::IniFormat);

    loadSettings();
    setting.Port = nPort;
    setting.address = nameConnect;


    tcpServer = new QTcpServer(this);
    socket = new QTcpSocket(this);
    if(!tcpServer->listen(QHostAddress::Any, setting.Port)){
        qDebug() << "Serer error\n" << "Unable to start server: " << tcpServer->errorString();
        tcpServer->close();
    }
    if(tcpServer->isListening())

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    connect(socket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(socket, SIGNAL(readyRead()), SLOT(slotRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotError(QAbstractSocket::SocketError)));

}

void NetworkManager::loadSettings()
{
    settings->beginGroup("Parametrs");
    setting.Port = settings->value("Port", 9090).toInt();
    setting.address = (settings->value("Address", "127.0.0.1")).toString();
    settings->endGroup();
}

void NetworkManager::saveSettings()
{
    settings->beginGroup("Parametrs");
    settings->setValue("Port", setting.Port);
    settings->setValue("Address", setting.address);
    settings->endGroup();
    settings->sync();
}

void NetworkManager::showSet()
{
    qDebug() << setting.Port
             << setting.address;
}

NetworkManager::~NetworkManager()
{
    saveSettings();
    delete settings;
}

void NetworkManager::slotNewConnection()
{
    qDebug()<< "New connected!";
    socket = tcpServer->nextPendingConnection();
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(status()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotRead()));
    QByteArray buffer;
            qint32 *s = new qint32(0);
            buffers.insert(socket, buffer);
            sizes.insert(socket, s);
}

void NetworkManager::slotRead()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
        QByteArray buffer = buffers.value(socket);
        qint32 *s = sizes.value(socket);
        qint32 size = *s;
        while (socket->bytesAvailable() > 0)
        {
            QByteArray tmp = socket->readAll();
            buffer.append(tmp);
            qDebug() << "buffer received ( size" << tmp.size() << "): " << buffer.toHex();
        }
        size = buffer.size();
        *s = size;
        QString var = "RX: ";
        var.append(buffer);
        qDebug()<< var;
        emit sendToDC(buffer);

}

void NetworkManager::SendData(QByteArray data)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);
    out << quint16(0) << data;
    out.device()->seek(0);
    out << (quint16)(arrBlock.size() - sizeof(quint16));

    socket->write(arrBlock);
}


void NetworkManager::status()
{
    socket = (QTcpSocket *)sender();
    if(socket == NULL){
        return;
    }
    QString str;
    str = (socket->peerAddress()).toString();
    qDebug() << "Client " + str + " disconnected.";
}

void NetworkManager::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                           "The Host was not found." :
                         err == QAbstractSocket::RemoteHostClosedError ?
                            "The remote Host is closed." :
                         err == QAbstractSocket::ConnectionRefusedError ?
                             "The connection was refused." :
                         QString(socket->errorString()));
    qDebug() << strError;
}

void NetworkManager::slotConnected()
{
    qDebug() << "Recieved the connected() signal.";
}

void NetworkManager::connectToClient()
{
        socket->connectToHost(setting.address, setting.Port);

}

void NetworkManager::readToDC(QByteArray data)
{
    SendData(data);
}

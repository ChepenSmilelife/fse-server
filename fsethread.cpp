#include "fsethread.h"

#include <QSqlDatabase>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QMutexLocker>
#include <QDebug>

FSEThread::FSEThread(qintptr SocketDescriptor, QSqlDatabase *database,
                     QObject *parent )
    : QThread(parent), socketDescriptor(SocketDescriptor), db(database)
{
}

void FSEThread::run()
{
    //QMutexLocker locker(&mutex);

    QTcpSocket tcpSocket;

    if(!tcpSocket.setSocketDescriptor(socketDescriptor))
    {
        emit error(tcpSocket.error());
        emit errorString(tcpSocket.errorString());
    }
    tcpSocket.open(QIODevice::ReadWrite);

    emit debugString(tr("A new connection coming: ")
                     + QString("%1").arg(tcpSocket.peerAddress().toString()));


    const int timeout = 5 * 1000; // 5 sec

    // message: length + content
    // get length of message content from tcpSocket,
    // the length has been stored in type 'quint16'(2 bytes)
    while(tcpSocket.bytesAvailable() < (int)sizeof(quint16))
        if(!tcpSocket.waitForReadyRead(timeout)) {
            emit error(tcpSocket.error());
            emit errorString(tcpSocket.errorString());
            return;
        }
    quint16 blkSize; // size of message content
    QDataStream in(&tcpSocket);
    in.setVersion(QDataStream::Qt_4_8);
    in >> blkSize;

    // get message content, its type is QStringList
    while(tcpSocket.bytesAvailable() < (int)blkSize)
        if(!tcpSocket.waitForReadyRead(timeout)) {
            emit error(tcpSocket.error());
            emit errorString(tcpSocket.errorString());
            return;
        }
    QStringList message;
    in >> message;

    qDebug() << "FSE tcpSocket thread say: message -> "
             << blkSize << message;

    qDebug() << "FSE tcpSocket thread say: connection state -> " << tcpSocket.state();
    qDebug()<<"client say: is open -> " << tcpSocket.isOpen();
    qDebug()<<"client say: is writable -> " << tcpSocket.isWritable();
    qDebug()<<"client say: is valid -> " << tcpSocket.isValid();

    // send message
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);

    QStringList msg;
    msg << "success";
    msg << "login successfully";

    out << (quint16)0 << msg;
    out.device()->seek(0);
    out << (quint16) (bytes.size() - sizeof(quint16));

    tcpSocket.write(bytes);
    //tcpSocket.flush();
    if(!tcpSocket.waitForBytesWritten(timeout)) {
        qDebug() << "FSE tcpSocket thread say: send timeout";
        emit error(tcpSocket.error());
        emit errorString(tcpSocket.errorString());
        return;
    }
    tcpSocket.disconnectFromHost();
}

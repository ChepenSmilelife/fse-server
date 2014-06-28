#include "fsethread.h"

#include <QSqlDatabase>
#include <QTcpSocket>
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
}

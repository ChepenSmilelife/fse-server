#include "fsethread.h"

#include <QSqlDatabase>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QMutexLocker>
#include <QVariant>
#include <QList>
#include <QDebug>

#include "sqlhelper.h"
#include "fseenum.h"

FSEThread::FSEThread(qintptr SocketDescriptor, QSqlDatabase *database,
                     QObject *parent )
    : QThread(parent), socketDescriptor(SocketDescriptor), db(database)
{
}

void FSEThread::run()
{
    QTcpSocket tcpSocket;

    if(!tcpSocket.setSocketDescriptor(socketDescriptor))
    {
        emit error(tcpSocket.error());
        emit errorString(tcpSocket.errorString());
    }

    emit debugString(tr("A new connection coming: ")
                     + QString("%1").arg(tcpSocket.peerAddress().toString()));


    const int timeout = 5 * 1000; // 5 sec


    /*
     * get length of message content from tcpSocket,
     * the length has been stored in type 'quint16'(2 bytes)
    */
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

    // get message content, its type is QList<QVariant>
    while(tcpSocket.bytesAvailable() < (int)blkSize)
        if(!tcpSocket.waitForReadyRead(timeout)) {
            emit error(tcpSocket.error());
            emit errorString(tcpSocket.errorString());
            return;
        }
    QList<QVariant> message;
    in >> message;

    if(message.size() < 3) {
        emit errorString(tr("Invalid connection from ")
                         + tcpSocket.peerAddress().toString());
        tcpSocket.disconnectFromHost();
        return;
    }

    QString username = message.takeFirst().toString();
    QString password = message.takeFirst().toString();
    int uid = checkUser(username, password, db);

    if( uid >= 0) {
        int command = message.takeFirst().toInt();
        switch (command) {
        case LoginCMD:
            emit debugString("[" + username + "]["
                             + tcpSocket.peerAddress().toString()
                             + "]" + tr(" login successfully"));
            processLogin(&tcpSocket, LoginSuccessState);
            break;
        case PushFilePasswordCMD:
            processPushFilePWD(&tcpSocket, uid, message.takeFirst().toStringList());
            break;
        case PullFilePasswordCMD:
            processPullFilePWD(&tcpSocket, uid, message.takeFirst().toStringList());
            break;
        default:
            break;
        }
    }
    else {
        processLogin(&tcpSocket, LoginFailedState);
        emit debugString("[" + username + "]["
                         + tcpSocket.peerAddress().toString()
                         + "]" + tr(" try to login, but failed"));
    }

    tcpSocket.disconnectFromHost();
}

void FSEThread::sendMessage(QTcpSocket *socket, const QList<QVariant> &block)
{
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << (quint16)0 << block;
    out.device()->seek(0);
    out << (quint16) (bytes.size() - sizeof(quint16));
    socket->write(bytes);

    int timeout = 5 * 1000;
    if(!socket->waitForBytesWritten(timeout)) {
        emit error(socket->error());
        emit errorString(socket->errorString());
    }
}

void FSEThread::processLogin(QTcpSocket *socket, FSEState state)
{
    QList<QVariant> block;
    block << QVariant(state);
    block << QVariant( state == LoginSuccessState ?
                           tr("Login Successfully") : tr("Wrong, please try again"));

    sendMessage(socket, block);
}

void FSEThread::processPushFilePWD(QTcpSocket *socket, int uid, QStringList argument)
{
    bool error = false;
    QString description;
    if(argument.size() < 3) {
        emit debugString(tr("argument too few for adding file to database from ")
                         + socket->peerAddress().toString());
        error = true;
    }
    else {
        QString filename = argument.takeFirst();
        QString filemd5 = argument.takeFirst();
        QString filepwd = argument.takeFirst();

        int fid = addFile(filename, filemd5, filepwd, uid, db);
        description = (fid == -1) ? tr("the file is already pushed") : tr("push successfully");
        emit debugString(tr("add file to database: ") + filename);
    }

    if(error)
        description = tr("argument too few");

    QList<QVariant> block;
    block << QVariant( error ? PushFilePWDFailedState : PushFilePWDSuccessState)
             << QVariant(description);

    sendMessage(socket, block);
}

void FSEThread::processPullFilePWD(QTcpSocket *socket, int uid, QStringList argument)
{
    bool error = false;
    QString pwd;
    if(argument.size() < 1)
        error = true;
    else {
        QString filemd5 = argument.takeFirst();
        pwd = getFilePWD(filemd5, uid, db);
        if(pwd.isEmpty())
            error = true;
    }
    QList<QVariant> block;
    block << QVariant( error ? PullFilePWDFailedState : PullFilePWDSuccessState )
             << QVariant( error ? tr("File not exist") : pwd );

    sendMessage(socket, block);
}

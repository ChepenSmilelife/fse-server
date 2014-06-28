#ifndef FSETHREAD_H
#define FSETHREAD_H

#include <QThread>
#include <QMutex>
#include <QTcpSocket>
/*
QT_BEGIN_NAMESPACE
class QSqlDatabase;
QT_END_NAMESPACE
*/
#include <QSqlDatabase>
#include "fseenum.h"

class FSEThread : public QThread
{
    Q_OBJECT
public:
    explicit FSEThread(qintptr SocketDescriptor, QSqlDatabase *database,
                       QObject *parent = 0);

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);
    void errorString(QString errorStr);
    void debugString(QString debug);

protected:
    void sendMessage(QTcpSocket *socket,const QList<QVariant> &block);
    void processLogin(QTcpSocket *socket, FSEState state);
    void processPushFilePWD(QTcpSocket *socket, int uid , QStringList argument);
    void processPullFilePWD(QTcpSocket *socket, int uid , QStringList argument);

private:
    //QMutex mutex;
    qintptr socketDescriptor;
    QSqlDatabase *db;
};

#endif // FSETHREAD_H

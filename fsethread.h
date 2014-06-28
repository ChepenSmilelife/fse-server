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

public slots:

private:
    //QMutex mutex;
    qintptr socketDescriptor;
    QSqlDatabase *db;
};

#endif // FSETHREAD_H

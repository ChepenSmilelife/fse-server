#ifndef FSESERVER_H
#define FSESERVER_H

#include <QTcpServer>

QT_BEGIN_NAMESPACE
class QSqlDatabase;
QT_END_NAMESPACE

class FSEServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit FSEServer(QObject *parent = 0);

    void setDatabase(QSqlDatabase *database);

signals:
    void errorString(QString errorStr);
    void debugString(QString debug);

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QSqlDatabase *db;

};

#endif // FSESERVER_H

#include "fseserver.h"

#include <QSqlDatabase>

#include "fsethread.h"

FSEServer::FSEServer(QObject *parent) :
    QTcpServer(parent)
{
}

void FSEServer::setDatabase(QSqlDatabase *database)
{
    db = database;
}

void FSEServer::incomingConnection(qintptr socketDescriptor)
{
    FSEThread *tcpThread = new FSEThread(socketDescriptor, db, this);
    connect(tcpThread, SIGNAL(errorString(QString)), this, SIGNAL(errorString(QString)));
    connect(tcpThread, SIGNAL(finished()), tcpThread, SLOT(deleteLater()));
    tcpThread->start();
}


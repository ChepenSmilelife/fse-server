#ifndef SOCKETERRORSTRING_H
#define SOCKETERRORSTRING_H

#include <QString>
#include <QAbstractSocket>

QString socketErrorToString(QAbstractSocket::SocketError error);

#endif // SOCKETERRORSTRING_H

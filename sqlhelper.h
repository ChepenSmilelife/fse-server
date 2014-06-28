#ifndef SQLHELPER_H
#define SQLHELPER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QSqlDatabase;
QT_END_NAMESPACE

struct GroupInfo {
    int gid;
    QString groupName;
    QString passwordMD5;
    QString description;
    QString createDate;
};

struct UserInfo {
    int uid;
    int gid;
    QString username;
    QString realname;
    QString md5password;
    int age;
    bool valid;
    QString address;
    QString description;
    QString createdate;
};

bool addGroup(QSqlDatabase *db, GroupInfo group);

bool addUser(QSqlDatabase *db, const UserInfo &user, QString &errorText);

bool deleteUser(QSqlDatabase *db, const QString username);

#endif // SQLHELPER_H

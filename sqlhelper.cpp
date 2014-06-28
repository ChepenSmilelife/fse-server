#include "sqlhelper.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QDebug>

#include "qmd5.h"
static QString dateformat = "yyyy-MM-dd";

bool addGroup(QSqlDatabase *db, GroupInfo group)
{
    QSqlQuery query(*db);
    query.prepare("insert into GroupInfo "
                  "(groupname, password, description, createdate) "
                  "values(:groupname, :password, :description, :createdate");
    query.bindValue(":groupname", group.groupName);
    query.bindValue(":password", group.passwordMD5);
    query.bindValue(":description", group.description);
    query.bindValue(":createdate", group.createDate);
    if(query.exec())
        return true;
    else
        return false;
}

bool addUser(QSqlDatabase *db, const UserInfo &user, QString &errorText)
{
    QSqlQuery query(*db);
    query.prepare("insert into UserInfo "
                  "(gid, username, realname, password, age, valid,"
                  "address, description, createdate)"
                  "values(:gid, :username, :realname, :password,"
                  ":age, :valid, :address, :description, :createdate)");
    query.bindValue(":gid", user.gid);
    query.bindValue(":username", user.username);
    query.bindValue(":realname", user.realname);
    query.bindValue(":password", user.md5password);
    query.bindValue(":age", user.age);
    query.bindValue(":valid", user.valid);
    query.bindValue(":address", user.address);
    query.bindValue(":description", user.description);
    query.bindValue(":createdate",
                    QDateTime::currentDateTime().toLocalTime().toString(dateformat));
    if(!query.exec()) {
        errorText = query.lastError().text();
        return false;
    }
    else
        return true;
}

bool deleteUser(QSqlDatabase *db, const QString username)
{
    QSqlQuery query(*db);
    QString sql = QString("select uid from UserInfo"
                          " where username = %1").arg(username);
    if(query.exec(sql) && query.next()) {
        int uid = query.value("uid").toInt();
        sql = QString("delete from File_User "
                      " where uid = %1").arg(QString::number(uid));
        if(query.exec(sql))
            return true;
    }
    return false;
}

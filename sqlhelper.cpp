#include "sqlhelper.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QDebug>

int checkUser(QString username, QString password, QSqlDatabase *db)
{
    int uid = -1;
    bool valid = true;
    QString sql = QString("select uid, valid from UserInfo "
                          " where username = '%1' "
                          " and "
                          " password = '%2' ").arg(username).arg(password);
    QSqlQuery query(sql, *db);

    if(query.next()) {
        uid = query.value(0).toInt();
        valid = query.value(1).toBool();
    }
    if(!valid)
        uid = -1;
    return uid;
}

int getfid(QString fileMD5, QSqlDatabase *db)
{
    int fid = -1;
    QString sql = QString("select fid from FileInfo "
                          " where md5 = '%1'").arg(fileMD5);
    QSqlQuery query(sql, *db);
    if(query.next()) {
        fid = query.value(0).toInt();
    }
    return fid;
}

int addFileMD5(QString fileMD5, QString pwdMD5, QSqlDatabase *db)
{
    int fid = -1;
    QSqlQuery query(*db);
    query.prepare("insert into FileInfo (md5, password) "
                  "values (:md5, :password)");
    query.bindValue(":md5",fileMD5);
    query.bindValue(":password",pwdMD5);
    if(query.exec())
        fid = getfid(fileMD5, db);
    return fid;
}

int file2userExist(int fid, int uid, QSqlDatabase *db)
{
    int fuid = -1;
    QString sql = QString("select fuid from File_User "
                          " where fid = %1 "
                          " and uid = %2").arg(fid).arg(uid);
    QSqlQuery query(sql, *db);
    if(query.next())
        fuid = query.value(0).toInt();
    return fuid;
}

int addFile2User(int fid, int uid, QString filename, QSqlDatabase *db)
{
    int fuid = file2userExist(fid, uid, db);
    if(fuid != -1)
        return -1;
    QSqlQuery query(*db);
    query.prepare("insert into File_User (fid, uid, filename, pushdate) "
                  "values (:fid, :uid, :filename, :pushdate)");
    query.bindValue(":fid", fid);
    query.bindValue(":uid", uid);
    query.bindValue(":filename", filename);
    query.bindValue(":pushdate",
                    QDateTime::currentDateTime().toLocalTime().toString("yyyy-MM-dd"));
    if(query.exec())
        fuid = file2userExist(fid, uid, db);
    return fuid;
}

int addFile(QString fileName, QString fileMD5, QString pwdMD5, int uid, QSqlDatabase *db)
{
    int fid = getfid(fileMD5, db);
    if(fid == -1) {
        fid = addFileMD5(fileMD5, pwdMD5, db);
    }
    qDebug() << "fid: "<< fid << "uid: " << uid;

    int fuid = addFile2User(fid, uid, fileName, db);
    return (fuid == -1) ? fuid : fid;
}

QString getFilePWD(QString fileMD5, int uid, QSqlDatabase *db)
{
    QString filePWD = "";
    QString sql = QString("select FileInfo.password from FileInfo, File_User "
                          " where FileInfo.fid = File_User.fid "
                          " and File_User.uid = %1 "
                          " and FileInfo.md5 = '%2' ").arg(uid).arg(fileMD5);
    QSqlQuery query(sql, *db);
    if(query.next())
        filePWD = query.value(0).toString();
    return filePWD;
}

QStringList getUserInfo(int uid, QSqlDatabase *db)
{
    QStringList info;
    QString sql = QString("select username, realname, age, address, description "
                          " from UserInfo "
                          " where uid = %1 ").arg(uid);
    QSqlQuery query(sql, *db);
    if(query.next()) {
        info << query.value(0).toString()
             << query.value(1).toString()
             << QString::number(query.value(2).toInt())
             << query.value(3).toString()
             << query.value(4).toString();
    }
    return info;
}

bool modifyUserInfo(int uid, QStringList args, QSqlDatabase *db)
{
    QSqlQuery query(*db);
    if(args[4].isEmpty()) {
        query.prepare("update UserInfo "
                      " set realname = :realname, age = :age, "
                      "     address = :address, description = :desc "
                      " where uid = :uid ");
        query.bindValue(":realname", args[0]);
        query.bindValue(":age", args[1].toInt());
        query.bindValue(":address", args[2]);
        query.bindValue(":desc", args[3]);
        query.bindValue(":uid", uid);
    }
    else {
        query.prepare("update UserInfo "
                      " set realname = :realname, age = :age, "
                      "     address = :address, description = :desc, "
                      "     password = :pwd "
                      " where uid = :uid ");
        query.bindValue(":realname", args[0]);
        query.bindValue(":age", args[1].toInt());
        query.bindValue(":address", args[2]);
        query.bindValue(":desc", args[3]);
        query.bindValue(":pwd", args[4]);
        query.bindValue(":uid", uid);
    }
    if(query.exec())
        return true;
    return false;
}

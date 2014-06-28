#ifndef SQLHELPER_H
#define SQLHELPER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QSqlDatabase;
QT_END_NAMESPACE

// if username and password(MD5) is valid, return uid.
// if don't found it, return -1;
int checkUser(QString username, QString password, QSqlDatabase *db);

// if not exist, return -1, from table FileInfo
int getfid(QString fileMD5, QSqlDatabase *db);

// if success, return file's fid, else return -1
// add file to table FileInfo
int addFileMD5(QString fileMD5, QString pwdMD5, QSqlDatabase *db);

// return fuid
int file2userExist(int fid, int uid, QSqlDatabase *db);

// add "file to user" to table File_User, return fuid
int addFile2User(int fid, int uid, QString filename, QSqlDatabase *db);

// if success, return file's fid, else return -1
int addFile(QString fileName, QString fileMD5, QString pwdMD5, int uid, QSqlDatabase *db);

QString getFilePWD(QString fileMD5, int uid, QSqlDatabase *db);

#endif // SQLHELPER_H

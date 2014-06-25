#ifndef DATABASECONFIGUREDIALOG_H
#define DATABASECONFIGUREDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QSqlDatabase;
QT_END_NAMESPACE

namespace Ui {
class DatabaseConfigureDialog;
}

class DatabaseConfigureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseConfigureDialog(QWidget *parent = 0);
    ~DatabaseConfigureDialog();

    void setSqlDatabase(QSqlDatabase **database, QString defaultConnectionName = "serversmall");

    bool isConnectToLocalDBFile() const;

public slots:
    // when confirm current settings, it will connect to database.
    // if failed, it will given error information
    void confirmDatabase();

    void setHostName(QString hostname);

    void setHostPort(int port);

    void setDatabaseName(QString databaseName);

    void setUserName(QString userName);

    void setPassword(QString password);

    void setConnectToLocalDBFile(bool d);

private slots:
    void on_checkBox_stateChanged(int arg1);
    void on_pushButtonDBPath_clicked();

private:
    Ui::DatabaseConfigureDialog *ui;
    QSqlDatabase **db;
    QString defaultConName;
};

#endif // DATABASECONFIGUREDIALOG_H

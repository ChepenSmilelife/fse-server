#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fseserver.h"

QT_BEGIN_NAMESPACE
class QCloseEvent;
class QTimer;
class QSqlDatabase;
QT_END_NAMESPACE


enum ServerState { ServerStop, ServerWorking };

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void startServer();
    void stopServer();
    void runningTimeChange();
    void stateChange(ServerState st);
    void logCollect(QString log);
    void socketError(QAbstractSocket::SocketError error);

    void on_actionStartOrStop_triggered();

    void on_actionAboutQt_triggered();

    void on_actionConfigure_triggered();

    void on_actionSQL_triggered();

    void on_spinBoxPort_valueChanged(int arg1);

protected:
    void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;
    // running time
    unsigned int runningSec;
    unsigned int runningMin;
    unsigned int runningHour;
    unsigned long runningDay;
    QTimer *timer;
    // database
    QSqlDatabase *db;
    // network
    QTcpServer *tcpServer;
    int serverPort;
    QString IPv4;
    ServerState serverState;
    // multithread server
    FSEServer *server;
};

#endif // MAINWINDOW_H

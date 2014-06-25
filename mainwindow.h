#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void portChange(int p);

    void on_actionStartOrStop_triggered();

    void on_actionAboutQt_triggered();

    void on_actionConfigure_triggered();

    void on_actionSQL_triggered();

protected:
    void closeEvent(QCloseEvent *e);
    // check ipv4 address of current host
    QString checkIPv4() const;
    // check hostname of current host
    QString checkHostname() const;

private:
    Ui::MainWindow *ui;

    void initData();
    int serverPort;
    QString IPv4;
    QString hostname;
    ServerState serverState;
    // running time
    unsigned int runningSec;
    unsigned int runningMin;
    unsigned int runningHour;
    unsigned long runningDay;
    QTimer *timer;
    QSqlDatabase *db;

};

#endif // MAINWINDOW_H

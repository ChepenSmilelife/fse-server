#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QTimer>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDateTime>
#include <QDir>

#include <QDebug>

#include "databaseconfiguredialog.h"
#include "sqldialog.h"
#include "socketerrorstring.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initData();

    ui->lineEditIPv4->setText(IPv4);
    ui->lineEditHostname->setText(hostname);

    connect(timer, SIGNAL(timeout()),
            this, SLOT(runningTimeChange()));
    connect(ui->spinBoxPort, SIGNAL(valueChanged(int)),
            this, SLOT(portChange(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QString log = ui->textEditLog->toPlainText();
    if(!log.isEmpty()) {
        QString logDir = "log";
        QDir dir(QDir::current());
        dir.mkdir(logDir);
        if(dir.cd(logDir)) {
            QString logFormat = "yyyy-MM-dd-hh.mm.ss.log";
            QString path = dir.filePath(QDateTime::currentDateTime().toLocalTime().toString(logFormat));
            QFile logFile(path);
            if(logFile.open(QIODevice::WriteOnly)) {
                QTextStream out(&logFile);
                out << log;
            }
        }
    }
    e->accept();
}

void MainWindow::initData()
{
    IPv4 = checkIPv4();
    hostname = checkHostname();
    serverPort = ui->spinBoxPort->value();
    serverState = ServerStop;

    runningDay = runningSec = runningMin = runningHour = 0;
    timer = new QTimer(this);
    timer->start(1000);

    db = NULL;

    server = NULL;
}

QString MainWindow::checkIPv4() const
{
    QString ipv4;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use first non-localhost IPv4 address
    for(int i = 0; i < ipAddressesList.size(); ++i)
    {
        if( ipAddressesList[i] != QHostAddress::LocalHost
                &&  ipAddressesList[i].toIPv4Address())
        {
            ipv4 = ipAddressesList[i].toString();
            break;
        }
    }
    if(ipv4.isEmpty())
        ipv4 = QHostAddress(QHostAddress::LocalHost).toString();
    return ipv4;
}

QString MainWindow::checkHostname() const
{
    return QHostInfo::localHostName();
}

void MainWindow::runningTimeChange()
{
    if(++runningSec >= 60) {
        runningSec = 0;
        ++runningMin;
    }
    if(runningMin >= 60) {
        runningMin = 0;
        ++runningHour;
    }
    if(runningHour >= 24) {
        runningHour = 0;
        ++runningDay;
    }
    QString runningTime = QString::number(runningDay)
            + ":" + QString::number(runningHour)
            + ":" + QString::number(runningMin)
            + ":" + QString::number(runningSec);
    ui->lineEditRunningTime->setText(runningTime);
}

void MainWindow::stateChange(ServerState st)
{
    QString state;
    switch (st) {
    case ServerWorking:
        state = tr("working");
        ui->actionStartOrStop->setText(tr("Stop"));
        ui->spinBoxPort->setEnabled(false);
        break;
    default:
        state = tr("stop");
        ui->actionStartOrStop->setText(tr("Start"));
        ui->spinBoxPort->setEnabled(true);
        break;
    }
    ui->lineEditState->setText(state);
    serverState = st;
}

void MainWindow::portChange(int p)
{
    serverPort = p;
}

void MainWindow::logCollect(QString log)
{
    ui->textEditLog->append(QDateTime::currentDateTime().toLocalTime().toString("yyyy-MM-dd>HH:mm:ss")
                            + "    " + log);
}

void MainWindow::socketError(QAbstractSocket::SocketError error)
{
    logCollect(socketErrorToString(error));
}

void MainWindow::startServer()
{
    if(!db || !db->isOpen()) {
        QMessageBox::information(this, tr("No database"),
                                 tr("you need to configure database for server at first..."));
        return;
    }
    if(server) {
        server->close();
        delete server;
    }

    server = new FSEServer(this);
    server->setDatabase(db);

    connect(server, SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(server, SIGNAL(errorString(QString)),
            this, SLOT(logCollect(QString)));
    connect(server, SIGNAL(debugString(QString)),
            this, SLOT(logCollect(QString)));

    if(server->listen(QHostAddress(IPv4), serverPort)) {
        logCollect(tr("start to listen ") + IPv4 + ":" +QString::number(serverPort));
        stateChange(ServerWorking);
    }
    else {
        qDebug() << "start failed" << server->serverError();
        stopServer();
    }
}

void MainWindow::stopServer()
{
    if(server) {
        server->close();
        delete server;
    }

    server = NULL;

    logCollect(tr("stop to listen ") + IPv4 + ":" +QString::number(serverPort));
    stateChange(ServerStop);
}

void MainWindow::on_actionStartOrStop_triggered()
{
    if(serverState == ServerStop)
        startServer();
    else
        stopServer();
}

void MainWindow::on_actionAboutQt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_actionConfigure_triggered()
{
    if(serverState == ServerWorking) {
        int w = QMessageBox::warning(this, tr("Reconfigure Database?"),
                                     tr("Server is working now...\n"
                                        "you must stop server, if you want to reconfigure server.\n"
                                        "Are you sure to stop server?"),
                                     tr("Yes, I sure"), tr("No, thanks"));
        if(w == 0)
            stopServer();
        else
            return;
    }
    DatabaseConfigureDialog dbConfDialog;
    dbConfDialog.setSqlDatabase(&db);
    // init configure
    if(db) {
        dbConfDialog.setHostName(db->hostName());
        dbConfDialog.setHostPort(db->port());
        dbConfDialog.setUserName(db->userName());
        dbConfDialog.setPassword(db->password());
        dbConfDialog.setDatabaseName(db->databaseName());
    }
    dbConfDialog.exec();
}

void MainWindow::on_actionSQL_triggered()
{
    if(!db || !db->isOpen())
        return;
    else{
        SQLDialog sqlDialog;
        sqlDialog.initAll(db);
        sqlDialog.exec();
    }
}

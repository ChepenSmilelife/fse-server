#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QTimer>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>

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

void MainWindow::startServer()
{
    stateChange(ServerWorking);
}

void MainWindow::stopServer()
{
    stateChange(ServerStop);
}

void MainWindow::on_actionStartOrStop_triggered()
{
    if(serverState == ServerStop)
        startServer();
    else
        stopServer();
}

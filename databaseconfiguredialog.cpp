#include "databaseconfiguredialog.h"
#include "ui_databaseconfiguredialog.h"

#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QFileDialog>
#include <QDebug>

DatabaseConfigureDialog::DatabaseConfigureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseConfigureDialog)
{
    ui->setupUi(this);

    db = NULL;

    connect(ui->buttonBox, SIGNAL(accepted()),
            this, SLOT(confirmDatabase()));
}

DatabaseConfigureDialog::~DatabaseConfigureDialog()
{
    delete ui;
}

void DatabaseConfigureDialog::setSqlDatabase(QSqlDatabase **database,
                                             QString defaultConnectionName)
{
    db = database;
    defaultConName = defaultConnectionName;
}

void DatabaseConfigureDialog::confirmDatabase()
{
    if(db == NULL)
        return;

    if(ui->comboBoxDBType->currentText() == "QSQLITE"
            && !QFile::exists(ui->lineEditDBName->text())) {
        if(ui->lineEditDBName->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, tr("Empty database name?"),
                                 tr("Pease enter database name"));
            return;
        }

        int w = QMessageBox::question(this, tr("Database does not exist"),
                                         tr("Database file does not exit\n"
                                            "Do you want to create a new .db file"),
                                         tr("Yes, I sure"), tr("No, thanks"));
        if(w == 1)
            return;

    }

    if(*db != NULL){
        // delete old connection to old database
        if((*db)->isOpen()) {
            qDebug()<<">> remove old connections"<<defaultConName;
            (*db)->close();

        }
        {
            QSqlDatabase tmpdb = QSqlDatabase::database(defaultConName);
            delete *db;
            *db = NULL;
        }
        QSqlDatabase::removeDatabase(defaultConName);
    }

    // start a new connection to database
    *db = new QSqlDatabase(QSqlDatabase::addDatabase(ui->comboBoxDBType->currentText(),
                                    defaultConName));

    if(!ui->checkBox->isChecked()) {
        (*db)->setHostName(ui->lineEditDBHost->text());
        (*db)->setUserName(ui->lineEditDBUser->text());
        (*db)->setPassword(ui->lineEditDBPassword->text());
        (*db)->setPort(ui->spinBoxPort->value());
    }

    (*db)->setDatabaseName(ui->lineEditDBName->text());

    qDebug()<<"host:"<<ui->lineEditDBHost->text()<<"\n"
           <<"port:"<<ui->spinBoxPort->value()<<"\n"
           <<"user:"<<ui->lineEditDBUser->text()<<"\n"
           <<"password:"<<ui->lineEditDBPassword->text()<<"\n"
           <<"db:"<<ui->lineEditDBName->text()<<"\n"
           <<"type:"<<ui->comboBoxDBType->currentText()<<"\n"
           <<"isLocalDBFile"<<ui->checkBox->isChecked()<<"\n";

    if(!(*db)->open()) {
        QMessageBox::warning(this, tr("Database connect error"),
                             (*db)->lastError().text());
    }
    else{
        qDebug()<<">> Open successfully";
        this->accept();
    }
}

void DatabaseConfigureDialog::setHostName(QString hostname)
{
    ui->lineEditDBHost->setText(hostname);
}

void DatabaseConfigureDialog::setUserName(QString userName)
{
    ui->lineEditDBUser->setText(userName);
}

void DatabaseConfigureDialog::setPassword(QString password)
{
    ui->lineEditDBPassword->setText(password);
}

void DatabaseConfigureDialog::setHostPort(int port)
{
    ui->spinBoxPort->setValue(port);
}

void DatabaseConfigureDialog::setDatabaseName(QString databaseName)
{
    ui->lineEditDBName->setText(databaseName);
}

void DatabaseConfigureDialog::setConnectToLocalDBFile(bool d)
{
    ui->checkBox->setChecked(d);
}

bool DatabaseConfigureDialog::isConnectToLocalDBFile() const
{
    return ui->checkBox->isChecked();
}

void DatabaseConfigureDialog::on_checkBox_stateChanged(int arg1)
{
    bool toEnable = true;
    QString backgroundColor;
    if(arg1 == Qt::Checked) { // if set connect to local db file
        toEnable = false;     // disable user, host, password
        backgroundColor = "background-color: rgb(150, 150, 150);";
    }else {
        toEnable = true;
        backgroundColor = "background-color: rgb(255, 255, 255);";
    }

    ui->pushButtonDBPath->setEnabled(!toEnable);

    ui->lineEditDBHost->setEnabled(toEnable);
    ui->lineEditDBUser->setEnabled(toEnable);
    ui->lineEditDBPassword->setEnabled(toEnable);
    ui->spinBoxPort->setEnabled(toEnable);

    ui->lineEditDBHost->setStyleSheet(backgroundColor);
    ui->lineEditDBPassword->setStyleSheet(backgroundColor);
    ui->lineEditDBUser->setStyleSheet(backgroundColor);
    ui->spinBoxPort->setStyleSheet(backgroundColor);
}

void DatabaseConfigureDialog::on_pushButtonDBPath_clicked()
{
    QString dbPath = QFileDialog::getOpenFileName(this, tr("Database file"),
                                                  ".",
                                                  "database file (*.db)");
    if(!dbPath.isEmpty())
        ui->lineEditDBName->setText(dbPath);
}

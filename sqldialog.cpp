#include "sqldialog.h"
#include "ui_sqldialog.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>

SQLDialog::SQLDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SQLDialog)
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    db = NULL;
    model = NULL;
}

SQLDialog::~SQLDialog()
{
    delete ui;
}

void SQLDialog::initAll(QSqlDatabase *database)
{
    db = database;
    model = new QSqlQueryModel;
    QStringList tbList = db->tables();
    for(int i = 0; i < tbList.size(); ++i)
    {
        ui->comboBoxTableList->addItem(tbList[i]);
    }
    ui->tableView->setModel(model);

    connect(ui->pushButtonRefresh, SIGNAL(clicked()),
            this, SLOT(refresh()));
    connect(ui->lineEditSQL, SIGNAL(returnPressed()),
            this, SLOT(execSQL()));

    refresh();
}

void SQLDialog::refresh()
{
    model->setQuery(QString("select * from %1").arg(ui->comboBoxTableList->currentText()),
                    *db);
}

void SQLDialog::execSQL()
{
    QSqlQuery sql(*db);
    if(!sql.exec(ui->lineEditSQL->text()))
        QMessageBox::warning(this, tr("input error"),
                             sql.lastError().text());
    else
        ui->lineEditSQL->clear();
}

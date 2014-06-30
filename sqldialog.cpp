#include "sqldialog.h"
#include "ui_sqldialog.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlTableModel>
#include <QHeaderView>
#include <QFileDialog>
#include "qmd5.h"

#include <QDebug>


SQLDialog::SQLDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SQLDialog)
{
    ui->setupUi(this);
/*    this->setAutoFillBackground(true);//设置背景填充
    QPalette palette;
    palette.setBrush(QPalette::Window,QBrush(QPixmap(":/T5.png")));//背景用图片填充
    this->setPalette(palette);*/
    db = NULL;
    tableMode = NULL;

    QHeaderView *header = ui->tableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setHorizontalHeader(header);
}

SQLDialog::~SQLDialog()
{
    delete ui;
}

void SQLDialog::initAll(QSqlDatabase *db)
{
    tableMode = new QSqlTableModel(this, *db);
    tableMode->setEditStrategy(QSqlTableModel::OnManualSubmit);//设置编辑策略
    ui->tableView->setModel(tableMode);
    connect(ui->lineEditPWD, SIGNAL(returnPressed()),
            this, SLOT(changePWD()));
    connect(ui->pushButtonInsertdata,SIGNAL(clicked()),
            this,SLOT(insertdata()));
    connect(ui->pushButtonSavedata,SIGNAL(clicked()),
            this,SLOT(savedata()));
    connect(ui->pushButtonDeletedata,SIGNAL(clicked()),
            this,SLOT(deletedata()));

    refresh();
}

void SQLDialog::refresh()
{
    tableMode->setTable("UserInfo");
    tableMode->select();
}

//插入数据
void SQLDialog::insertdata()
{
    int rownum = tableMode->rowCount();//获取当前行数
    int UserID = rownum+1;
    if(tableMode) {
        qDebug()<<"Insert Say: Sart...";
        tableMode->insertRow(rownum);
        qDebug()<<"Insert Say: 1 row...";
        tableMode->setData(tableMode->index(rownum,0),UserID);//在当前行的下一行插入数据
        qDebug()<<"Insert Say: end...";
    }

}

//删除数据
void SQLDialog::deletedata()
{
    if(tableMode){
        int curRow = ui->tableView->currentIndex().row(); //获取选中的行
        tableMode->removeRow(curRow);//删除该行
        int suer = QMessageBox::warning(this,tr("Delete this row?"),
                                        tr("Are you sure to delete this row?"),
                                          QMessageBox::Yes,QMessageBox::No);
            if(suer == QMessageBox::No)
            {
               tableMode->revertAll(); //如果不删除，则撤销
            }
            else tableMode->submitAll(); //否则提交，在数据库中删除该行
    }
}
//保存数据
void SQLDialog::savedata()
{
    int sure = QMessageBox::warning(this,tr("Save data"),
                                    tr("Are you sure to save them?"),
                                    QMessageBox::Yes,QMessageBox::No);
        if(sure == QMessageBox::No)
        {
           tableMode->revertAll(); //如果不保存，则撤销
        }
        else tableMode->submitAll(); //否则提交
        refresh();

}
//关键字查询
void SQLDialog::keywordselect()
{
    tableMode->setTable("UserInfo");//设置查询当前表内容
    QString keyword = ui->lineEditKeyword->text().trimmed();//从输入框获取关键字
    int colm=tableMode->columnCount();//获取表的列数，然后一列一列的循环查询
    for(int i=1;i<=colm;i++)
    {
        switch (i) {
        case 1:
            tableMode->setFilter(QObject::tr(" uid = %1").arg(keyword));
            tableMode->select();
            break;
        case 2:
            tableMode->setFilter(QObject::tr(" realname like \'%%1%\'").arg(keyword));
            tableMode->select();
            break;
        case 3:
            tableMode->setFilter(QObject::tr(" age = %1").arg(keyword));
            tableMode->select();
            break;
        case 4:
            tableMode->setFilter(QObject::tr(" username like \'%%1%\'").arg(keyword));
            tableMode->select();
            break;
        case 5:
            tableMode->setFilter(QObject::tr(" createdate = '%1'").arg(keyword));
            tableMode->select();
            break;


        }
        int crurow=tableMode->rowCount();//如果查询到结果，则停止查询。
        if(crurow>0)
        {
            break;
        }
    }
}


void SQLDialog::changePWD()
{
    QString tmpPwd = ui->lineEditPWD->text();
    QString newPwd = md5Str(tmpPwd);
    ui->lineEditPWD->setText(newPwd);

}

void SQLDialog::on_lineEditKeyword_textChanged(const QString &arg1)
{
    if(arg1.trimmed().isEmpty()) {
        refresh();
    }
    else {
        keywordselect();
    }
}

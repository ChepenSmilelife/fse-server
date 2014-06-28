#ifndef SQLDIALOG_H
#define SQLDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
class QSqlDatabase;
class QSqlTableModel;
QT_END_NAMESPACE

namespace Ui {
class SQLDialog;
}

class SQLDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SQLDialog(QWidget *parent = 0);
    ~SQLDialog();

    void initAll(QSqlDatabase *database);

public slots:
    void refresh();
    void insertdata();
    void savedata();
    void deletedata();
    void keywordselect();

private slots:
    void changePWD();

private:
    Ui::SQLDialog *ui;
    QSqlDatabase *db;
    QSqlTableModel*tableMode;
};

#endif // SQLDIALOG_H

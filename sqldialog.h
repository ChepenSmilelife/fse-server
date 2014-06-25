#ifndef SQLDIALOG_H
#define SQLDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QSqlDatabase;
class QSqlQueryModel;
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
    void execSQL();

private:
    Ui::SQLDialog *ui;
    QSqlDatabase *db;
    QSqlQueryModel *model;
};

#endif // SQLDIALOG_H

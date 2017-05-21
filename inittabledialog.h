#ifndef INITTABLEDIALOG_H
#define INITTABLEDIALOG_H

#include <QDialog>
#include "editdialog.h"

class editDialog;
namespace Ui {
class InitTableDialog;
}

class InitTableDialog : public QDialog
{
    Q_OBJECT

public:
    //explicit InitTableDialog(QWidget *parent = 0);
    explicit InitTableDialog(editDialog *edit=0,QWidget *parent = 0);
    ~InitTableDialog();

private slots:
    void on_pushButton_OK_clicked();

    void on_lineEdit_colmun_textChanged(const QString &arg1);

    void on_lineEdit_row_textChanged(const QString &arg1);

    void on_lineEdit_rowHeight_textChanged(const QString &arg1);

    void on_lineEdit_columnWidth_textChanged(const QString &arg1);

private:
    Ui::InitTableDialog *ui;
    editDialog *editRowAndColmun;
};

#endif // INITTABLEDIALOG_H

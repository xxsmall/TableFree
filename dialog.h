#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "editdialog.h"


class editDialog;
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    //explicit Dialog(QWidget *parent = 0);
    explicit Dialog(editDialog *edit=0,QWidget *parent = 0);
    ~Dialog();
public:
    int selectRowOrColmun;//选择的是列还是行  1，行  2，列
private slots:
    void on_pushButton_OK_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
    editDialog *edit3;
};

#endif // DIALOG_H

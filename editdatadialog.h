#ifndef EDITDATADIALOG_H
#define EDITDATADIALOG_H

#include <QDialog>
#include "editdialog.h"
#include <QDebug>


class editDialog;

namespace Ui {
class EditDataDialog;
}

class EditDataDialog : public QDialog
{
    Q_OBJECT

public:
   // explicit EditDataDialog(QWidget *parent = 0);
    explicit EditDataDialog(editDialog *edit=0,QWidget *parent = 0);
    QStandardItem *tempItem;
    ~EditDataDialog();

private slots:
    void on_pushButton_OK_clicked();

    void on_pushButton_BackGroundPhoto_clicked();

    void on_pushButton_BackGroundColor_clicked();

    void on_pushButton_Font_clicked();

    void on_pushButton_FontColor_clicked();

    void on_pushButton_RegPara_clicked();

    void on_pushButton_Cancel_clicked();

private:
    Ui::EditDataDialog *ui;
    editDialog *edit3;
};

#endif // EDITDATADIALOG_H

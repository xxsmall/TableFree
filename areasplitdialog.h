#ifndef AREASPLITDIALOG_H
#define AREASPLITDIALOG_H

#include <QDialog>
#include "editdialog.h"
#include <QDebug>


class editDialog;
namespace Ui {
class AreaSplitDialog;
}

class AreaSplitDialog : public QDialog
{
    Q_OBJECT

public:
    //explicit AreaSplitDialog(QWidget *parent = 0);
    explicit AreaSplitDialog(editDialog *edit=0,QWidget *parent = 0);
    ~AreaSplitDialog();

    QList<int> rowList;//row列表
    QList<int> columnList;//column列表
    int selectRowOrColmun;//选择的是列还是行  1，行  2，列
    void createRowList();//产生row显示列表
    void createColumnList();//产生column显示列表
private slots:
    void on_pushButton_mode_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_OK_clicked();

private:
    Ui::AreaSplitDialog *ui;
    editDialog *edit3;
};

#endif // AREASPLITDIALOG_H

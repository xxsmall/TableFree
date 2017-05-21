#include "inittabledialog.h"
#include "ui_inittabledialog.h"

InitTableDialog::InitTableDialog( editDialog *edit, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InitTableDialog)
{
    ui->setupUi(this);
    editRowAndColmun = edit;
    QString str;
    str=QString("%1").arg(editRowAndColmun->initColumnWidth,0,10);
    ui->lineEdit_columnWidth->setText(str);
    str=QString("%1").arg(editRowAndColmun->initRowHeight,0,10);
    ui->lineEdit_rowHeight->setText(str);

}

InitTableDialog::~InitTableDialog()
{
    delete ui;
}

void InitTableDialog::on_pushButton_OK_clicked()
{
   QString str;
   str=ui->lineEdit_row->text();
   quint32 row=str.toInt();
   str=ui->lineEdit_colmun->text();
   quint32 colmun=str.toInt();
   str=ui->lineEdit_rowHeight->text();
   quint32 setRowHeight=str.toInt();
   str=ui->lineEdit_columnWidth->text();
   quint32 setColumnWidth=str.toInt();
   if(colmun >0 && row >0 && setRowHeight>0 && setColumnWidth>0)
   {
       editRowAndColmun->tableRow=row;
       editRowAndColmun->tableColumn=colmun;
       editRowAndColmun->initColumnWidth=setColumnWidth;
       editRowAndColmun->initRowHeight=setRowHeight;
       editRowAndColmun->initTable(row,colmun);
       this->close();
   }else
   {
       QMessageBox::warning(NULL,"警告","表格参数输入不正确！");
   }

}

void InitTableDialog::on_lineEdit_colmun_textChanged(const QString &arg1)
{
    QString str=ui->lineEdit_colmun->text();
    int size=str.length();
    QList<QString> strZeroToNine;
    strZeroToNine.append("0");
    strZeroToNine.append("1");
    strZeroToNine.append("2");
    strZeroToNine.append("3");
    strZeroToNine.append("4");
    strZeroToNine.append("5");
    strZeroToNine.append("6");
    strZeroToNine.append("7");
    strZeroToNine.append("8");
    strZeroToNine.append("9");
    bool ok=false;
    for(int i=0;i<size;i++)
    {
         QChar tempStr= str[i];
         QString str2;
         str2.append(tempStr);

         if(strZeroToNine.contains(tempStr))
         {
             ok=true;
             continue;
         }else
         {
             ok=false;
             break;
         }
    }

    if(ok)
    {
        ;
    }else
    {
        ui->lineEdit_colmun->setText("");
    }
}

void InitTableDialog::on_lineEdit_row_textChanged(const QString &arg1)
{
    QString str=ui->lineEdit_row->text();
    int size=str.length();
    QList<QString> strZeroToNine;
    strZeroToNine.append("0");
    strZeroToNine.append("1");
    strZeroToNine.append("2");
    strZeroToNine.append("3");
    strZeroToNine.append("4");
    strZeroToNine.append("5");
    strZeroToNine.append("6");
    strZeroToNine.append("7");
    strZeroToNine.append("8");
    strZeroToNine.append("9");
    bool ok=false;
    for(int i=0;i<size;i++)
    {
         QChar tempStr= str[i];
         QString str2;
         str2.append(tempStr);

         if(strZeroToNine.contains(tempStr))
         {
             ok=true;
             continue;
         }else
         {
             ok=false;
             break;
         }
    }

    if(ok)
    {
        ;
    }else
    {
        ui->lineEdit_row->setText("");
    }
}

void InitTableDialog::on_lineEdit_rowHeight_textChanged(const QString &arg1)
{
    QString str=ui->lineEdit_rowHeight->text();
    int size=str.length();
    QList<QString> strZeroToNine;
    strZeroToNine.append("0");
    strZeroToNine.append("1");
    strZeroToNine.append("2");
    strZeroToNine.append("3");
    strZeroToNine.append("4");
    strZeroToNine.append("5");
    strZeroToNine.append("6");
    strZeroToNine.append("7");
    strZeroToNine.append("8");
    strZeroToNine.append("9");
    bool ok=false;
    for(int i=0;i<size;i++)
    {
         QChar tempStr= str[i];
         QString str2;
         str2.append(tempStr);

         if(strZeroToNine.contains(tempStr))
         {
             ok=true;
             continue;
         }else
         {
             ok=false;
             break;
         }
    }

    if(ok)
    {
        ;
    }else
    {
        ui->lineEdit_rowHeight->setText("");
    }
}

void InitTableDialog::on_lineEdit_columnWidth_textChanged(const QString &arg1)
{
    QString str=ui->lineEdit_columnWidth->text();
    int size=str.length();
    QList<QString> strZeroToNine;
    strZeroToNine.append("0");
    strZeroToNine.append("1");
    strZeroToNine.append("2");
    strZeroToNine.append("3");
    strZeroToNine.append("4");
    strZeroToNine.append("5");
    strZeroToNine.append("6");
    strZeroToNine.append("7");
    strZeroToNine.append("8");
    strZeroToNine.append("9");
    bool ok=false;
    for(int i=0;i<size;i++)
    {
         QChar tempStr= str[i];
         QString str2;
         str2.append(tempStr);

         if(strZeroToNine.contains(tempStr))
         {
             ok=true;
             continue;
         }else
         {
             ok=false;
             break;
         }
    }

    if(ok)
    {
        ;
    }else
    {
        ui->lineEdit_columnWidth->setText("");
    }
}

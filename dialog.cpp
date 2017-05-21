#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(editDialog *edit,QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    edit3= edit;
    selectRowOrColmun=1;
    ui->pushButton->setText("拆分行");
    this->setWindowTitle("基本单元格分割");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_OK_clicked()
{
    QString str;
    bool ok;
    str=ui->lineEdit->text();
    quint32 needSplit=str.toInt(&ok,10);
    if(ok && needSplit>1 && needSplit<1000)
    {
        quint16 number=edit3->getTableSelectNumber();
        if(number==1)
        {
            if(selectRowOrColmun==1)
            {
                edit3->rowSplit(needSplit);

                edit3->initTable(edit3->tableRow,edit3->tableColumn);

                edit3->updateTableSize();

                edit3->useNewCellListSetSpan();

            }else if(selectRowOrColmun==2)
            {
                edit3->columnSplit(needSplit);

                edit3->initTable(edit3->tableRow,edit3->tableColumn);

                edit3->updateTableSize();

                edit3->useNewCellListSetSpan();
            }else
            {
                ;
            }

        }else
        {
             QMessageBox::warning(NULL,"警告信息",QString("请选择1个基本的表格单元拆分，已选择%1个基本表格单元！").arg(number,0,10));
        }
    }else
    {
        QMessageBox::warning(NULL,"警告信息","请输入正确的范围");
    }

    this->close();
}

void Dialog::on_lineEdit_textChanged(const QString &arg1)
{
    QString str=ui->lineEdit->text();
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
        ui->lineEdit->setText("");
    }
}

void Dialog::on_pushButton_clicked()
{
    if(selectRowOrColmun==1)
    {
        selectRowOrColmun=2;
        ui->pushButton->setText("拆分列");
    }else if(selectRowOrColmun==2)
    {
        selectRowOrColmun=1;
        ui->pushButton->setText("拆分行");
    }else
    {
        selectRowOrColmun=1;
        ui->pushButton->setText("拆分行");
    }

}

#include "areasplitdialog.h"
#include "ui_areasplitdialog.h"

AreaSplitDialog::AreaSplitDialog(editDialog *edit,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AreaSplitDialog)
{
    ui->setupUi(this);
    edit3=edit;
    selectRowOrColmun=1;
    ui->pushButton_mode->setText("拆分行");
    this->setWindowTitle("区域分割");
    createRowList();
    createColumnList();
    ui->comboBox->clear();
    for(int i=0;i<rowList.count();i++)
    {
        QString str;
        str=QString("%1").arg(rowList[i],0,10);
        ui->comboBox->addItem(str);
    }
}

AreaSplitDialog::~AreaSplitDialog()
{
    delete ui;
}

void AreaSplitDialog::on_pushButton_mode_clicked()
{
    if(selectRowOrColmun==1)
    {
        selectRowOrColmun=2;
        ui->pushButton_mode->setText("拆分列");
        ui->comboBox->clear();
        for(int i=0;i<columnList.count();i++)
        {
            QString str;
            str=QString("%1").arg(columnList[i],0,10);
            ui->comboBox->addItem(str);
        }

    }else if(selectRowOrColmun==2)
    {
        selectRowOrColmun=1;
        ui->pushButton_mode->setText("拆分行");
        ui->comboBox->clear();
        for(int i=0;i<rowList.count();i++)
        {
            QString str;
            str=QString("%1").arg(rowList[i],0,10);
            ui->comboBox->addItem(str);
        }
    }else
    {
        selectRowOrColmun=1;
        ui->pushButton_mode->setText("拆分行");
        ui->comboBox->clear();
        for(int i=0;i<rowList.count();i++)
        {
            QString str;
            str=QString("%1").arg(rowList[i],0,10);
            ui->comboBox->addItem(str);
        }
    }
}

/**
 * @brief AreaSplitDialog::createRowList
 * 产生可分割的行列表
 * creat a row list that to split selected area.
 */
void AreaSplitDialog::createRowList()
{
    rowList.clear();
    int rowMini=edit3->areaMiniX;
    int rowMax=edit3->areaMaxX;
    qDebug()<<rowMini<<"  "<<rowMax;
    int rowInclude=rowMax-rowMini+1;

    if(rowInclude<=1)
    {
        return ;
    }else if(rowInclude >1)
    {
        for(int i=2;i<=rowInclude;i++)
        {
            int canSplit=rowInclude/i;
            if(canSplit*i == rowInclude)
            {
                rowList.append(i);
            }
        }

    }

}

/**
 * @brief AreaSplitDialog::createColumnList
 * 产生可分割的列 列表
 * creat a column list that to split selected area.
 */
void AreaSplitDialog::createColumnList()
{
    columnList.clear();
    int columnMini=edit3->areaMiniY;
    int columnMax=edit3->areaMaxY;
    int columnInclude=columnMax-columnMini+1;
    if(columnInclude<=1)
    {
        return ;
    }else if(columnInclude > 1)
    {
        for(int i=2;i<=columnInclude;i++)
        {
            int canSplit = columnInclude/i;
            if(canSplit*i == columnInclude)
            {
                columnList.append(i);
            }
        }
    }
}

void AreaSplitDialog::on_comboBox_currentTextChanged(const QString &arg1)
{


}

/**
 * @brief AreaSplitDialog::on_pushButton_OK_clicked
 * 分割函数
 * the split function.
 */
void AreaSplitDialog::on_pushButton_OK_clicked()
{
    QString str=ui->comboBox->currentText();
    int number=str.toInt();
    qDebug()<<"number "<<number;
    if(number >= 2 ) //小于0不正确，等于0为没选择，1，为原样分，均不合理，大于等于2才正常
    {
        if(selectRowOrColmun==1)//行拆分
        {


            if(edit3->selectBaseCellNumber == number)//单列的满行拆分，还原表格即可
            {
                edit3->initAreaCell(edit3->areaMiniX,edit3->areaMiniY,edit3->areaMaxX,edit3->areaMaxY);//表格队列还原
                edit3->initTable(edit3->tableRow,edit3->tableColumn);//表格重建
                edit3->updateTableSize();
                edit3->useNewCellListSetSpan();//表格span重现 ;

            }else if(edit3->selectBaseCellNumber > number)
            {
                edit3->initAreaCell(edit3->areaMiniX,edit3->areaMiniY,edit3->areaMaxX,edit3->areaMaxY);//表格队列还原
                edit3->initTable(edit3->tableRow,edit3->tableColumn);//表格重建
                edit3->updateTableSize();
                edit3->useNewCellListSetSpan();//表格span重现 ;

                for(int i=0 ; i<number ; i++)//分割的行数，例如，6列可以分为 6/2=3，6/3=2，6/6=1
                {
                   qDebug()<<"area maxX"<<edit3->areaMaxX<<"  area miniX"<<edit3->areaMiniX;
                   int singleRowNumber=(edit3->areaMaxX - edit3->areaMiniX+1)/number;//分割的行数，例如，6列可以分为 6/2=3，6/3=2，6/6=1
                   //区域合并
                   edit3->areaCombination(edit3->areaMiniX+i*singleRowNumber,edit3->areaMiniY,edit3->areaMiniX+(i+1)*singleRowNumber-1,edit3->areaMaxY);
                   qDebug()<<"row num:  "<<singleRowNumber;
                }

                edit3->initTable(edit3->tableRow,edit3->tableColumn);

                edit3->updateTableSize();

                edit3->useNewCellListSetSpan();

            }

        }

        if(selectRowOrColmun==2)//列拆分
        {
            if(edit3->selectBaseCellNumber == number)//单行的满列拆分，还原表格即可
            {
                edit3->initAreaCell(edit3->areaMiniX,edit3->areaMiniY,edit3->areaMaxX,edit3->areaMaxY);//表格队列还原
                edit3->initTable(edit3->tableRow,edit3->tableColumn);//表格重建
                edit3->updateTableSize();
                edit3->useNewCellListSetSpan();//表格span重现 ;

            } if(edit3->selectBaseCellNumber > number)
            {
                edit3->initAreaCell(edit3->areaMiniX,edit3->areaMiniY,edit3->areaMaxX,edit3->areaMaxY);//表格队列还原
                edit3->initTable(edit3->tableRow,edit3->tableColumn);//表格重建
                edit3->updateTableSize();
                edit3->useNewCellListSetSpan();//表格span重现 ;

                for(int i=0 ; i<number ; i++)//分割的列数，例如，6列可以分为 6/2=3，6/3=2，6/6=1
                {
                   qDebug()<<"area maxY"<<edit3->areaMaxY<<"  area miniY"<<edit3->areaMiniY;
                   int singleRowNumber=(edit3->areaMaxY - edit3->areaMiniY+1)/number;
                   edit3->areaCombination(edit3->areaMiniX,edit3->areaMiniY+i*singleRowNumber,edit3->areaMaxX,edit3->areaMiniY+(i+1)*singleRowNumber-1);
                   qDebug()<<"column num:  "<<singleRowNumber;
                }

                edit3->initTable(edit3->tableRow,edit3->tableColumn);

                edit3->updateTableSize();

                edit3->useNewCellListSetSpan();

            };
        }
    }

    this->close();
}

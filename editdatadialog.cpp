#include "editdatadialog.h"
#include "ui_editdatadialog.h"

EditDataDialog::EditDataDialog(editDialog *edit,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDataDialog)
{
    ui->setupUi(this);
    edit3=edit;
    this->setWindowTitle(tr("单元格属性编辑"));
    ui->comboBox_DataType->addItem(tr("自定义字符串"));
    ui->comboBox_DataType->addItem(tr("注册参数"));
    ui->textEdit_BackGroundColor->setEnabled(false);
    if(edit3->cellListObj[edit3->editPosition].data.editFlag=="1")//if the cell is selected,take data to the ui
    {
        int red=edit3->cellListObj[edit3->editPosition].data.backGroundColorRed.toInt();
        int green=edit3->cellListObj[edit3->editPosition].data.backGroundColorGreen.toInt();
        int blue=edit3->cellListObj[edit3->editPosition].data.backGroundColorBlue.toInt();
        QColor tempColor;
        tempColor.setRed(red);
        tempColor.setBlue(blue);
        tempColor.setGreen(green);

        ui->textEdit_BackGroundColor->setPalette(QPalette(tempColor));

        red=edit3->cellListObj[edit3->editPosition].data.fontColorRed.toInt();
        green=edit3->cellListObj[edit3->editPosition].data.fontColorGreen.toInt();
        blue=edit3->cellListObj[edit3->editPosition].data.fontColorBlue.toInt();

        tempColor.setRed(red);
        tempColor.setBlue(blue);
        tempColor.setGreen(green);
        ui->textEdit_FontColor->setPalette(QPalette(tempColor));;

        QString str=edit3->cellListObj[edit3->editPosition].data.backGroundPhoto;
        ui->lineEdit_BackGroundPhoto->setText(str);

        str=edit3->cellListObj[edit3->editPosition].data.userString;
        ui->lineEdit_UserString->setText(str);

        str=edit3->cellListObj[edit3->editPosition].data.registerXPath;
        ui->lineEdit_RegisterPara->setText(str);

        QFont font;
        font.setFamily(edit3->cellListObj[edit3->editPosition].data.fontFamily);
        font.setBold(edit3->cellListObj[edit3->editPosition].data.fontBold.toInt());
        font.setItalic(edit3->cellListObj[edit3->editPosition].data.fontItalic.toInt());
        font.setPointSize(edit3->cellListObj[edit3->editPosition].data.fontSize.toInt());
        ui->label_Font->setFont(font);
    }
}

EditDataDialog::~EditDataDialog()
{
    delete ui;
}

void EditDataDialog::on_pushButton_OK_clicked()
{  //save the cell data that edit
   int i=edit3->cellListObj[edit3->editPosition].row;
   int j=edit3->cellListObj[edit3->editPosition].colmun;

   QList<quint16> rowHeight;//行高
   QList<quint16> columnWidth;//列宽

   edit3->getWidthAndHeight();

   for(int m=0;m<edit3->tableRow;m++)//取出行高对列
   {
      rowHeight.append(edit3->rowHeight[m]);

   }

   for(int n=0;n<edit3->tableColumn;n++)//取出列宽队列
   {
       columnWidth.append(edit3->columnWidth[n]);

   }

   int width = 0 ;
   int height = 0 ;
   int startX = edit3->cellListObj[edit3->editPosition].startRow;
   int endX = edit3->cellListObj[edit3->editPosition].endRow;

   for(int m = startX ; m <= endX ; m++)//calculate the cell's  size
   {
       height = height + rowHeight[m] ;
   }

   int startY = edit3->cellListObj[edit3->editPosition].startColmun;
   int endY= edit3->cellListObj[edit3->editPosition].endColmun;

   for(int m = startY ; m <= endY ; m++)//calculate the cell's  size
   {
       width = width + columnWidth[m] ;
   }


   QSize sizeItem;
   sizeItem.setWidth(width);
   sizeItem.setHeight(height);

   QColor BGColor = ui->textEdit_BackGroundColor->palette().background().color();
   QString userStr = ui->lineEdit_UserString->text();
   QFont  font=ui->label_Font->font();
   QColor fontColor=ui->textEdit_FontColor->palette().background().color();
   QString regStr=ui->lineEdit_RegisterPara->text();
   int dataType=ui->comboBox_DataType->currentIndex();

   if(dataType==0)
   {
      tempItem=new QStandardItem(userStr);
      QPixmap pix(ui->lineEdit_BackGroundPhoto->text());
      tempItem->setForeground(QBrush(fontColor));
      pix=pix.scaled(sizeItem,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
      tempItem->setBackground(pix);

         // tempItem->setIcon(pix);
      edit3->modelEdit->setItem(i,j,tempItem); //首先创建Item
   }else
   {
       tempItem=new QStandardItem(regStr);
      // QPixmap pix(QLatin1String(":/images/mouse.png"));
       QPixmap pix(ui->lineEdit_BackGroundPhoto->text());
       tempItem->setForeground(QBrush(fontColor));

       pix=pix.scaled(sizeItem,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
       tempItem->setBackground(pix);

       edit3->modelEdit->setItem(i,j,tempItem); //首先创建Item create the item
    }

   if(ui->lineEdit_BackGroundPhoto->text().isEmpty())//if the back ground string is empty,use back color as back ground
   {
        edit3->modelEdit->item(i,j)->setForeground(QBrush(fontColor));//才能设置字体颜色
        edit3->modelEdit->item(i,j)->setBackground(QBrush(BGColor));//最后设置背景色
   }


   edit3->modelEdit->item(i,j)->setFont(font);//再设字体大小等信息

  //save data to cell object
   edit3->cellListObj[edit3->editPosition].data.dataType=QString("%1").arg(dataType,0,10);
   edit3->cellListObj[edit3->editPosition].data.backGroundPhoto=ui->lineEdit_BackGroundPhoto->text();
   edit3->cellListObj[edit3->editPosition].data.backGroundColorRed=QString("%1").arg(BGColor.red(),0,10);
   edit3->cellListObj[edit3->editPosition].data.backGroundColorGreen=QString("%1").arg(BGColor.green(),0,10);
   edit3->cellListObj[edit3->editPosition].data.backGroundColorBlue=QString("%1").arg(BGColor.blue(),0,10);
   edit3->cellListObj[edit3->editPosition].data.fontSize=QString("%1").arg(font.pointSize(),0,10);
   edit3->cellListObj[edit3->editPosition].data.fontFamily=font.family();
   edit3->cellListObj[edit3->editPosition].data.fontBold=QString("%1").arg(font.bold(),0,10);
   edit3->cellListObj[edit3->editPosition].data.fontItalic=QString("%1").arg(font.italic(),0,10);
   edit3->cellListObj[edit3->editPosition].data.fontColorRed=QString("%1").arg(fontColor.red(),0,10);
   edit3->cellListObj[edit3->editPosition].data.fontColorGreen=QString("%1").arg(fontColor.green(),0,10);
   edit3->cellListObj[edit3->editPosition].data.fontColorBlue=QString("%1").arg(fontColor.blue(),0,10);

   edit3->cellListObj[edit3->editPosition].data.registerXPath=regStr;
   edit3->cellListObj[edit3->editPosition].data.userString=userStr;
   edit3->cellListObj[edit3->editPosition].data.editFlag="1";

   this->close();


}

/**
 * @brief EditDataDialog::on_pushButton_BackGroundPhoto_clicked
 * set cell back ground photo
 */
void EditDataDialog::on_pushButton_BackGroundPhoto_clicked()
{
     QString fileName;
     fileName = QFileDialog::getOpenFileName(this,"open file dialog","/home","png  files(*.png)");
     ui->lineEdit_BackGroundPhoto->setText(fileName);

}

/**
 * @brief EditDataDialog::on_pushButton_BackGroundColor_clicked
 * set cell back ground color
 */
void EditDataDialog::on_pushButton_BackGroundColor_clicked()
{
    QColor color =QColorDialog::getColor(Qt::red);
    if(color.isValid())
    {

        ui->textEdit_BackGroundColor->setPalette(QPalette(color));
    }
}

/**
 * @brief EditDataDialog::on_pushButton_Font_clicked
 * set cell font
 */
void EditDataDialog::on_pushButton_Font_clicked()
{
    bool ok;
    QFont font =QFontDialog::getFont(&ok);

    if(ok)
    {
        ui->label_Font->setFont(font);
       //QMessageBox::about(NULL,"font size",QString::number(font.pointSize()));

   }
}

/**
 * @brief EditDataDialog::on_pushButton_FontColor_clicked
 * set font color
 */
void EditDataDialog::on_pushButton_FontColor_clicked()
{
    QColor color =QColorDialog::getColor(Qt::black);
    if(color.isValid())
    {

        ui->textEdit_FontColor->setPalette(QPalette(color));
    }
}

void EditDataDialog::on_pushButton_RegPara_clicked()
{

}

/**
 * @brief EditDataDialog::on_pushButton_Cancel_clicked
 * cancel button
 */
void EditDataDialog::on_pushButton_Cancel_clicked()
{
    this->close();
}

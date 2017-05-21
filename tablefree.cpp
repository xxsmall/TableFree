#include "tablefree.h"

TableFree::TableFree(QWidget *parent) :
    QWidget(parent)
{
    modelLeft=new QStandardItemModel();
    tableViewLeft = new QTableView();
    tableViewLeft->setModel(modelLeft);
    firstOpenFlag = true ;
    layout= new QGridLayout(this);//进行控件布局
    layout->addWidget(tableViewLeft,0,0);//设置布局layout= new QGridLayout(this);//进行控件布局
}

void TableFree::initTable(quint16 row,quint16 column)
{

    cellListObj.clear();
    cellListDescribe singleObj;
    this->tableViewLeft->clearSpans();//clear上一次的span信息
    modelLeft->clear();

    QStringList list ;
    for(int i=0;i<column;i++)
    {
        QString str;
        str=QString("%1 ").arg(i,0,10);
        list.append(str);

    }

    modelLeft->setHorizontalHeaderLabels(list);

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            QString str2;
            str2=QString("%1,%2").arg(i,0,10).arg(j,0,10);
            modelLeft->setItem(i,j,new QStandardItem(str2));
            singleObj.row=i;
            singleObj.colmun=j;
            singleObj.startRow=i;
            singleObj.startColmun=j;
            singleObj.endRow=i;
            singleObj.endColmun=j;
            singleObj.baseCell=1;
            cellListObj.append(singleObj);
            this->tableViewLeft->setColumnWidth(j,initColumnWidth);
        }
        this->tableViewLeft->setRowHeight(i,initRowHeight);

    }

}


void TableFree::updateTableSize()
{
       for(int i=0;i<rowHeight.count();i++)
        {
          //  QMessageBox::about(NULL,"还原区域",QString("%1").arg(rowHeight[i],0,10));
            this->tableViewLeft->setRowHeight(i,rowHeight[i]);
        }

        for(int j=0;j<columnWidth.count();j++)
        {
            this->tableViewLeft->setColumnWidth(j,columnWidth[j]);
        }
}

void TableFree::removeCell(int startRow,int startColumn,int endRow,int endColumn)
{
    int size = cellListObj.count();
    if(size > 0)
    {
        bool ok1=false;
        bool ok2=false;

        for(int i=0;i<size;)
        {
           if(cellListObj[i].startRow >= startRow && cellListObj[i].startColmun >= startColumn)
           {
               ok1=true;
           }else
           {
               ok1=false;
           }

           if(cellListObj[i].endRow <= endRow && cellListObj[i].endColmun <= endColumn)
           {
               ok2=true;
           }else
           {
               ok2=false;
           }

           if(ok1 && ok2)
           {
                cellListObj.removeAt(i);
                //i 不需要++
                size=cellListObj.count();
           }else
           {
               i=i+1;

           }
        }

        cellListDescribe tempObj;  //追加合并后的对象
        tempObj.row=startRow;
        tempObj.colmun=startColumn;
        tempObj.startRow=startRow;
        tempObj.startColmun=startColumn;
        tempObj.endRow=endRow;
        tempObj.endColmun=endColumn;
        tempObj.baseCell=0;
        cellListObj.append(tempObj);


    }
}


void TableFree::useNewCellListSetSpan()
{

    int size =newCellListObj.count();
    QMessageBox::about(NULL,"newCellListObj",QString(" 新表的大小newCellListObj.count=%1 ").arg(size,0,10));
    int miniX=0;
    int miniY=0;
    int maxX=0;
    int maxY=0;

    bool ok1;
    bool ok2;


    for(int i=0;i<size;i++)
    {
         miniX=newCellListObj[i].startRow;
         miniY=newCellListObj[i].startColmun;
         maxX=newCellListObj[i].endRow;
         maxY=newCellListObj[i].endColmun;

         if(newCellListObj[i].startRow == newCellListObj[i].endRow )
         {
             ok1 = true;
         }else
         {
             ok1=false;
         }
         if(newCellListObj[i].startColmun == newCellListObj[i].endColmun)
         {
             ok2=true;
         }else
         {
             ok2=false;
         }

         if(ok1 && ok2)
         {
             //基本单元格，不用setspan
                ;
         }else
         {   //非基本单元格，需要setspan
             this->tableViewLeft->setSpan(miniX,miniY,maxX-miniX+1,maxY-miniY+1);
             removeCell(miniX,miniY,maxX,maxY);
         }

    }

}


void TableFree::setCellString()
{
      int size=cellListObj.count();
      QColor BGColor ;
      QString userStr;
      QFont  font;
      QColor fontColor;
      QString regStr;
      for(int m=0;m<size;m++)
      {
          int i=cellListObj[m].row;
          int j=cellListObj[m].colmun;
          int width = 0 ;
          int height = 0 ;
          int dataType=cellListObj[m].data.dataType.toInt();
          QString bgPhoto=cellListObj[m].data.backGroundPhoto;
          int red = cellListObj[m].data.backGroundColorRed.toInt();
          int green = cellListObj[m].data.backGroundColorGreen.toInt();
          int blue = cellListObj[m].data.backGroundColorBlue.toInt();

          BGColor.setRed(red);
          BGColor.setGreen(green);
          BGColor.setBlue(blue);

          red = cellListObj[m].data.fontColorRed.toInt();
          green = cellListObj[m].data.fontColorGreen.toInt();
          blue = cellListObj[m].data.fontColorBlue.toInt();

          fontColor.setRed(red);
          fontColor.setGreen(green);
          fontColor.setBlue(blue);

          userStr=cellListObj[m].data.userString;
          regStr=cellListObj[m].data.registerXPath;

         // QMessageBox::about(NULL,"background color rgb",QString("%1  %2  %3").arg(red,0,10).arg(green,0,10).arg(blue,0,10));
          int startX = cellListObj[m].startRow;
          int endX = cellListObj[m].endRow;

          for(int m = startX ; m <= endX ; m++)
          {
              height = height + rowHeight[m] ;
          }

          int startY = cellListObj[m].startColmun;
          int endY= cellListObj[m].endColmun;

          for(int m = startY ; m <= endY ; m++)
          {
              width = width + columnWidth[m] ;
          }

          QSize sizeItem;
          sizeItem.setWidth(width);
          sizeItem.setHeight(height);
          QStandardItem *tempItem;
          //
          if(dataType==0)
          {
              tempItem=new QStandardItem(userStr);
              QPixmap pix(bgPhoto);
              tempItem->setForeground(QBrush(fontColor));
              pix=pix.scaled(sizeItem,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
              tempItem->setBackground(pix);

             // tempItem->setIcon(pix);
              this->modelLeft->setItem(i,j,tempItem); //首先创建Item

             // this->modelLeft->setItem(i,j,new QStandardItem(userStr));
          }else if(dataType==1)
          {
              tempItem=new QStandardItem(regStr);
              QPixmap pix(bgPhoto);
              tempItem->setForeground(QBrush(fontColor));
              pix=pix.scaled(sizeItem,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
              tempItem->setBackground(pix);

             // tempItem->setIcon(pix);
              this->modelLeft->setItem(i,j,tempItem); //首先创建Item

             // this->modelLeft->setItem(i,j,new QStandardItem(regStr));
          }

          if(bgPhoto.isEmpty())
          {
                this->modelLeft->item(i,j)->setForeground(QBrush(fontColor));//才能设置字体颜色
                this->modelLeft->item(i,j)->setBackground(QBrush(BGColor));//最后设置背景色
          }



        //  this->modelLeft->item(i,j)->setBackground(QBrush(BGColor));//最后设置背景色


         // QMessageBox::about(NULL," font color rgb",QString("%1  %2  %3").arg(red,0,10).arg(green,0,10).arg(blue,0,10));
       //   this->modelLeft->item(i,j)->setForeground(QBrush(fontColor));//能设置字体颜色

          QString strFamily=cellListObj[m].data.fontFamily;
          int     fontSize=cellListObj[m].data.fontSize.toInt();
          font.setFamily(strFamily);
          font.setPointSize(fontSize);
          font.setBold(cellListObj[m].data.fontBold.toInt());
          font.setItalic(cellListObj[m].data.fontItalic.toInt());
          this->modelLeft->item(i,j)->setFont(font);


      }

}


void TableFree::saveHeightInfo()
{
    int sizeRow = rowHeight.count();
    int sizeColumn = columnWidth.count();
    if((sizeRow > 0) && (sizeColumn > 0))
    {
        QString saveStr="";
        QString strTemp="";
        for(int i=0;i<sizeRow;i++)
        {
            strTemp="";
            strTemp=QString("%1").arg(rowHeight[i],0,10);

            if(i==(sizeRow-1))
            {
                saveStr= saveStr + strTemp ;
            }else
            {
              saveStr= saveStr + strTemp + ",";
            }

        }
        setRowHeightList(saveStr);

    }
}

void TableFree::saveWidthInfo()
{
    int sizeRow = rowHeight.count();
    int sizeColumn = columnWidth.count();
    if((sizeRow > 0) && (sizeColumn > 0))
    {
        QString saveStr="";
        QString strTemp="";
        for(int i=0;i<sizeColumn;i++)
        {
            strTemp="";
            strTemp=QString("%1").arg(columnWidth[i],0,10);
            if(i==(sizeColumn-1))
            {
                saveStr= saveStr + strTemp ;
            }else
            {
              saveStr= saveStr + strTemp + ",";
            }

        }
        setColumnWidthList(saveStr);
    }
}


void TableFree::saveCellListInfo()
{
    int size = cellListObj.count();
    if(size > 0)
    {
        QString saveStr="";
        QString strTemp="";
        for(int i=0;i<size;i++)
        {
            if(i==(size-1))
            {
                QMessageBox::about(NULL,"i== size-1",QString("%1").arg(size,0,10));
                strTemp="";
                QString str;
                str=QString("%1").arg(cellListObj[i].row,0,10);
                strTemp=strTemp+str+"," ;
                str=QString("%1").arg(cellListObj[i].colmun,0,10);
                strTemp=strTemp+str+"," ;
                str=QString("%1").arg(cellListObj[i].startRow,0,10);
                strTemp=strTemp+str+"," ;
                str=QString("%1").arg(cellListObj[i].startColmun,0,10);
                strTemp=strTemp+str+"," ;
                str=QString("%1").arg(cellListObj[i].endRow,0,10);
                strTemp=strTemp+str+"," ;
                str=QString("%1").arg(cellListObj[i].endColmun,0,10);
                strTemp=strTemp+str+"," ;
                str=QString("%1").arg(cellListObj[i].baseCell,0,10);
                strTemp=strTemp+str+"," ; setCellList(saveStr);

                str=cellListObj[i].data.dataType;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.backGroundPhoto;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.backGroundColorRed;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.backGroundColorBlue;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.backGroundColorGreen;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.fontSize;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.fontFamily;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.fontBold;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.fontItalic;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.fontColorRed;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.fontColorBlue;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.fontColorGreen;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.registerXPath;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.userString;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.editFlag;

                strTemp=strTemp+str ;  //最后一个不需要 ","
            }else
            {
                QMessageBox::about(NULL,"i != size-1",QString("%1").arg(size,0,10));
                strTemp="";
                QString str;
                str=QString("%1").arg(cellListObj[i].row,0,10);
                strTemp=strTemp+str+"," ;
                str=QString("%1").arg(cellListObj[i].colmun,0,10);
                strTemp=strTemp+str+"," ;
                str=QString("%1").arg(cellListObj[i].startRow,0,10);
                strTemp=strTemp+str+"," ;
                str=QString("%1").arg(cellListObj[i].startColmun,0,10);
                strTemp=strTemp+str+"," ;
                str=QString("%1").arg(cellListObj[i].endRow,0,10);
                strTemp=strTemp+str+"," ;
                str=QString("%1").arg(cellListObj[i].endColmun,0,10);
                strTemp=strTemp+str+"," ;
                str=QString("%1").arg(cellListObj[i].baseCell,0,10);
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.dataType;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.backGroundPhoto;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.backGroundColorRed;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.backGroundColorBlue;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.backGroundColorGreen;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.fontSize;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.fontFamily;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.fontBold;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.fontItalic;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.fontColorRed;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.fontColorBlue;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.fontColorGreen;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.registerXPath;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.userString;
                strTemp=strTemp+str+"," ;

                str=cellListObj[i].data.editFlag;
                strTemp=strTemp+str+"," ;
            }

                saveStr = saveStr +strTemp ;
        }

        setCellList(saveStr);

        int sizeNew=newCellListObj.count();
        if(sizeNew == 0)
        {
            setNewCellList(saveStr);//防止表格没合并或分割操作时，newCellListObj为空的情况
        }
    }
}

void TableFree::saveNewCellListInfo()
{

    int size = newCellListObj.count();
       if(size > 0)
       {
           QString saveStr="";
           QString strTemp="";
           for(int i=0;i<size;i++)
           {
               if(i==(size-1))
               {
                   strTemp="";
                   QString str;
                   str=QString("%1").arg(newCellListObj[i].row,0,10);
                   strTemp=strTemp+str+"," ;
                   str=QString("%1").arg(newCellListObj[i].colmun,0,10);
                   strTemp=strTemp+str+"," ;
                   str=QString("%1").arg(newCellListObj[i].startRow,0,10);
                   strTemp=strTemp+str+"," ;
                   str=QString("%1").arg(newCellListObj[i].startColmun,0,10);
                   strTemp=strTemp+str+"," ;
                   str=QString("%1").arg(newCellListObj[i].endRow,0,10);
                   strTemp=strTemp+str+"," ;
                   str=QString("%1").arg(newCellListObj[i].endColmun,0,10);
                   strTemp=strTemp+str+"," ;
                   str=QString("%1").arg(newCellListObj[i].baseCell,0,10);
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.dataType;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.backGroundPhoto;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.backGroundColorRed;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.backGroundColorBlue;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.backGroundColorGreen;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.fontSize;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.fontFamily;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.fontBold;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.fontItalic;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.fontColorRed;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.fontColorBlue;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.fontColorGreen;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.registerXPath;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.userString;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.editFlag;

                   strTemp=strTemp+str ;  //最后一个不需要 ","
               }else
               {
                   strTemp="";
                   QString str;
                   str=QString("%1").arg(newCellListObj[i].row,0,10);
                   strTemp=strTemp+str+"," ;
                   str=QString("%1").arg(newCellListObj[i].colmun,0,10);
                   strTemp=strTemp+str+"," ;
                   str=QString("%1").arg(newCellListObj[i].startRow,0,10);
                   strTemp=strTemp+str+"," ;
                   str=QString("%1").arg(newCellListObj[i].startColmun,0,10);
                   strTemp=strTemp+str+"," ;
                   str=QString("%1").arg(newCellListObj[i].endRow,0,10);
                   strTemp=strTemp+str+"," ;
                   str=QString("%1").arg(newCellListObj[i].endColmun,0,10);
                   strTemp=strTemp+str+"," ;
                   str=QString("%1").arg(newCellListObj[i].baseCell,0,10);
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.dataType;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.backGroundPhoto;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.backGroundColorRed;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.backGroundColorBlue;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.backGroundColorGreen;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.fontSize;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.fontFamily;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.fontBold;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.fontItalic;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.fontColorRed;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.fontColorBlue;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.fontColorGreen;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.registerXPath;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.userString;
                   strTemp=strTemp+str+"," ;

                   str=newCellListObj[i].data.editFlag;
                   strTemp=strTemp+str+"," ;
               }

               saveStr = saveStr +strTemp ;

           }

           setNewCellList(saveStr);

       }
}
void TableFree::setCellList(QString needSaveStr)
{
    cellListStr = needSaveStr;
}

void TableFree::setNewCellList(QString needSaveStr)
{
    newCellListStr = needSaveStr;
}

void TableFree::setRowHeightList(QString needSaveStr)
{
    rowHeightListStr = needSaveStr;
}

void TableFree::setColumnWidthList(QString needSaveStr)
{
    columnWidthListStr = needSaveStr ;
    if(firstOpenFlag)
    {

        reviewUITableFirstOpen();
        firstOpenFlag = false ;
    }
}

void TableFree::reviewUITableFirstOpen()
{
     QList<QString> splitRowHeight;//从存储属性分离出的行高队列
     QList<QString> splitColumnWidth;//从存储属性分离出的列宽队列

     int sizeRow = 0;  //表格的行数
     splitRowHeight = getRowHeightList().split(",");
     sizeRow =splitRowHeight.count();

     int sizeColumn = 0;
     splitColumnWidth =getColumnWidthList().split(",");
     sizeColumn=splitColumnWidth.count();

     if( (sizeRow > 0) && (!getRowHeightList().isEmpty()) )
     {
         tableRow = sizeRow;
         tableColumn =  sizeColumn ;
         rowHeight.clear();
         columnWidth.clear();
         for(int i=0;i<sizeRow;i++)
         {
             int height = splitRowHeight[i].toInt();
             rowHeight.append(height);
         }

         for(int i=0;i<sizeColumn;i++)
         {
             int width =splitColumnWidth[i].toInt();
             columnWidth.append(width);
         }

         initTable(tableRow,tableColumn);//初始化表格
         updateTableSize();//更新行高，列宽
         reviewNewCellListObj();//重现newCellListObj
         useNewCellListSetSpan();//重现表格span
         reviewCellListObj();//将cellListObj清空，然后还原为UI中存储的信息，
         setCellString();//重现表格的各类属性
         needTranslateData=true;
     }else
     {
         needTranslateData=false;
     }

}


void TableFree::reviewCellListObj()
{//22个对象
    QList<QString> strList;
    int size=0;
    strList=getCellList().split(",");
    size = strList.count();

    if( (size>0) && (!getCellList().isEmpty()) )
    {
        int number = size  / 22;
        cellListObj.clear();
        cellListDescribe single;
        for(int i=0;i<number;i++)
        {
            int j=0;
            single.row= strList[i*22+j].toInt();
            j=j+1;
            single.colmun = strList[i*22+j].toInt();
            j=j+1;
            single.startRow = strList[i*22+j].toInt();
            j=j+1;
            single.startColmun = strList[i*22+j].toInt();
            j=j+1;
            single.endRow = strList[i*22+j].toInt();
            j=j+1;
            single.endColmun = strList[i*22+j].toInt();
            j=j+1;
            single.baseCell = strList[i*22+j].toInt();
            j=j+1;
            single.data.dataType = strList[i*22+j];
            j=j+1;
            single.data.backGroundPhoto = strList[i*22+j];
            j=j+1;
            single.data.backGroundColorRed = strList[i*22+j];
            j=j+1;
            single.data.backGroundColorBlue = strList[i*22+j];
            j=j+1;
            single.data.backGroundColorGreen = strList[i*22+j];
            j=j+1;
            single.data.fontSize = strList[i*22+j];
            j=j+1;
            single.data.fontFamily = strList[i*22+j];
            j=j+1;
            single.data.fontBold = strList[i*22+j];
            j=j+1;
            single.data.fontItalic = strList[i*22+j];
            j=j+1;
            single.data.fontColorRed = strList[i*22+j];
            j=j+1;
            single.data.fontColorBlue = strList[i*22+j];
            j=j+1;
            single.data.fontColorGreen = strList[i*22+j];
            j=j+1;
            single.data.registerXPath = strList[i*22+j];
            j=j+1;
            single.data.userString = strList[i*22+j];
            j=j+1;
            single.data.editFlag = strList[i*22+j];
            j=j+1;




            cellListObj.append(single);


        }
       // QMessageBox::about(NULL,"cell review ! ",QString("number = %1").arg(number,0,10));
    }
}

void TableFree::reviewNewCellListObj()
{//22个对象
    QList<QString> strList;
    int size=0;
    strList=getNewCellList().split(",");
    size = strList.count();

    if( (size>0) && (!getNewCellList().isEmpty()) )
    {
        int number = size  / 22;
        newCellListObj.clear();
        cellListDescribe single;
        for(int i=0;i<number;i++)
        {
            int j=0;
            single.row= strList[i*22+j].toInt();
            j=j+1;
            single.colmun = strList[i*22+j].toInt();
            j=j+1;
            single.startRow = strList[i*22+j].toInt();
            j=j+1;
            single.startColmun = strList[i*22+j].toInt();
            j=j+1;
            single.endRow = strList[i*22+j].toInt();
            j=j+1;
            single.endColmun = strList[i*22+j].toInt();
            j=j+1;
            single.baseCell = strList[i*22+j].toInt();
            j=j+1;

            newCellListObj.append(single);


        }
       // QMessageBox::about(NULL,"New cell review ! ",QString("number = %1").arg(number,0,10));
    }

}

#include "editdialog.h"
#include "ui_editdialog.h"

editDialog::editDialog(TableFree *table, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editDialog)
{
    ui->setupUi(this);
    freeTableObject = table;
    modelEdit=new QStandardItemModel();
    ui->tableView_Edit->setModel(modelEdit);
    initRowHeight=40;
    initColumnWidth=100;
    if(freeTableObject->needTranslateData)
    {
        loadData();
    }

}

editDialog::~editDialog()
{
    delete ui;
}

void editDialog::on_pushButton_Init_clicked()
{

    InitTableDialog newDialog(this);
    newDialog.exec();

}

void editDialog::initTable(quint16 row,quint16 column)
{

    cellListObj.clear();
    cellListDescribe singleObj;
    ui->tableView_Edit->clearSpans();//clear上一次的span信息
    modelEdit->clear();

    QStringList list ;
    for(int i=0;i<column;i++)
    {
        QString str;
        str=QString("%1 ").arg(i,0,10);
        list.append(str);

    }

    modelEdit->setHorizontalHeaderLabels(list);

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            QString str2;
            str2=QString("%1,%2").arg(i,0,10).arg(j,0,10);
            modelEdit->setItem(i,j,new QStandardItem(str2));
            singleObj.row=i;
            singleObj.colmun=j;
            singleObj.startRow=i;
            singleObj.startColmun=j;
            singleObj.endRow=i;
            singleObj.endColmun=j;
            singleObj.baseCell=1;
            singleObj.data.fontColorBlue="0";
            singleObj.data.fontColorGreen="0";
            singleObj.data.fontColorRed="0";
            cellListObj.append(singleObj);
            ui->tableView_Edit->setColumnWidth(j,initColumnWidth);
        }
        ui->tableView_Edit->setRowHeight(i,initRowHeight);

    }




}

void editDialog::combinationTable()
{
    QItemSelectionModel *selectModelMain=ui->tableView_Edit->selectionModel();
    QModelIndexList selectedListMain=selectModelMain->selectedIndexes();

    int sizeMain=selectedListMain.size();



    QList<int> mainIndexList;
    mainIndexList.clear();

    int miniX=0;
    int miniY=0;
    int maxX=0;
    int maxY=0;
    QString str;
    QString strTemp;
    if(sizeMain >0)//获取应该增加的行的列表
    {

        for(int i=0;i<sizeMain;i++)
        {
            strTemp=QString("r%1 c%2,").arg(selectedListMain[i].row(),0,10).arg(selectedListMain[i].column(),0,10);
            str=str+strTemp;
            if(i==0)
            {
                miniX = selectedListMain[i].row();
                miniY = selectedListMain[i].column();
                maxX = selectedListMain[i].row();
                maxY = selectedListMain[i].column();
            }else
            {
                int posX = selectedListMain[i].row();
                int posY = selectedListMain[i].column();
                if((posX<=miniX) && (posY<=miniY))
                {
                    miniX = posX;
                    miniY = posY;
                }
                if((posX>= maxX) && (posY>= maxY))
                {
                    maxX=posX;
                    maxY=posY;
                }
            }



        }

    }//区域坐标检测

    ui->tableView_Edit->setSpan(miniX,miniY,maxX-miniX+1,maxY-miniY+1);
    removeCell(miniX,miniY,maxX,maxY);

    //QMessageBox::about(NULL,"合并单元格大小 和 选择区域的坐标",QString::number(sizeMain)+"=="+str);

    newCellListObj.clear();
    cellListDescribe singleObj;
    for(int k=0;k<cellListObj.count();k++)
    {
        singleObj.row=cellListObj[k].row;
        singleObj.colmun=cellListObj[k].colmun;
        singleObj.startRow=cellListObj[k].startRow;
        singleObj.startColmun=cellListObj[k].startColmun;
        singleObj.endRow=cellListObj[k].endRow;
        singleObj.endColmun=cellListObj[k].endColmun;
        singleObj.baseCell=cellListObj[k].baseCell;
        newCellListObj.append(singleObj);
    }

    rowHeight.clear();
    columnWidth.clear();

    for(int m=0;m<tableRow;m++)//取出行高对列
    {
        rowHeight.append(ui->tableView_Edit->rowHeight(m));
    }

    for(int n=0;n<tableColumn;n++)//取出列宽队列
    {
        columnWidth.append(ui->tableView_Edit->columnWidth(n));
    }
//    tableRow=tableRow;//更新表格的row和column，准备表格重置
//    tableColumn=tableColumn;
}

void editDialog::on_pushButton_SetSpan_clicked()
{
    combinationTable();
    initTable(tableRow,tableColumn);
    updateTableSize();
    useNewCellListSetSpan();
}


void editDialog::removeCell(int startRow,int startColumn,int endRow,int endColumn)
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

//        size=cellListObj.count();
//        for(int i=0;i<size;i++)//调试时使用，显示包含的单元格的信息
//        {
//            QString str1;
//            QString str2;
//            QString str3;
//            str1=QString("当前 r%1 c%2,").arg(cellListObj[i].row,0,10).arg(cellListObj[i].colmun,0,10);
//            str2=QString("\n startRow%1 c%2,").arg(cellListObj[i].startRow,0,10).arg(cellListObj[i].startColmun, 0,10);
//            str3=QString("\n endRow%1 c%2,").arg(cellListObj[i].endRow,0,10).arg(cellListObj[i].endColmun, 0,10);
//            str1=str1+str2+str3;
//            QMessageBox::about(NULL,"组合后的队列里有",str1);
//        }

    }
}

void editDialog::on_pushButton_split_clicked()
{
    quint16 selectNumber=getTableSelectNumber();
    if(selectNumber==1)
    {
        Dialog newDialog(this);
        newDialog.exec();
    }else if(selectNumber > 1)
    {
            QItemSelectionModel *selectModelMain=ui->tableView_Edit->selectionModel();
            QModelIndexList selectedListMain=selectModelMain->selectedIndexes();

            int sizeMain=selectedListMain.size();
            int miniX=0;
            int miniY=0;
            int maxX=0;
            int maxY=0;
            QString str;
            QString strTemp;

            if(sizeMain >0)//区域坐标检测;
            {

                for(int i=0;i<sizeMain;i++)
                {
                    strTemp=QString("r%1 c%2,").arg(selectedListMain[i].row(),0,10).arg(selectedListMain[i].column(),0,10);
                    str=str+strTemp;
                    if(i==0)
                    {
                        miniX = selectedListMain[i].row();
                        miniY = selectedListMain[i].column();
                        maxX = selectedListMain[i].row();
                        maxY = selectedListMain[i].column();
                    }else
                    {
                        int posX = selectedListMain[i].row();
                        int posY = selectedListMain[i].column();
                        if((posX<=miniX) && (posY<=miniY))
                        {
                            miniX = posX;
                            miniY = posY;
                        }
                        if((posX>= maxX) && (posY>= maxY))
                        {
                            maxX=posX;
                            maxY=posY;
                        }
                    }



                }

            }//区域坐标检测;
            int startRow=miniX;
            int startColumn=miniY;
            int endRow=maxX;
            int endColumn=maxY;

            int size = cellListObj.count();
            bool notBaseCell=true;
            if((startRow==endRow) && (startColumn==endColumn))
            {
                notBaseCell=false;
            }else
            {
                notBaseCell=true;
            }

            bool findThisArea=false;
            int  findPosition=0;
            bool ok1;
            bool ok2;
            bool ok3;
            bool ok4;
            if(size > 0  && notBaseCell)
            {
                for(int i=0;i<size;i++)
                {
                    if(cellListObj[i].startRow == startRow)
                    {
                        ok1=true;
                    }else
                    {
                        ok1=false;
                    }

                    if(cellListObj[i].startColmun == startColumn)
                    {
                        ok2=true;
                    }else
                    {
                        ok2=false;
                    }

                    if(cellListObj[i].endRow == endRow)
                    {
                        ok3=true;
                    }else
                    {
                        ok3=false;
                    }

                    if(cellListObj[i].endColmun == endColumn)
                    {
                        ok4=true;
                    }else
                    {
                        ok4=false;
                    }

                    if(ok1 && ok2 && ok3 && ok4)
                    {
                        findThisArea=true;
                        findPosition=i;
                        break;
                    }else
                    {
                        ;
                    }

                }
            }

            if(findThisArea)
            {
                areaMiniX=startRow;
                areaMiniY=startColumn;
                areaMaxX=endRow;
                areaMaxY=endColumn;
                selectBaseCellNumber=sizeMain;
                AreaSplitDialog newDialog2(this);
                newDialog2.exec(); ;//调用新的split对话框
            }else
            {
                QMessageBox::about(NULL,"警告","您选择的区域不在有效队列中");

            }
    }else
    {
        ;
    }

}


void editDialog::rowSplit(qint16 needSplit)
{
    QItemSelectionModel *selectModelMain=ui->tableView_Edit->selectionModel();
    QModelIndexList selectedListMain=selectModelMain->selectedIndexes();

    int sizeMain=selectedListMain.size();
    int miniX=0;
    int miniY=0;
    int maxX=0;
    int maxY=0;
    QString str;
    QString strTemp;

    if(sizeMain >0)//获取应该增加的行的列表
    {

        for(int i=0;i<sizeMain;i++)
        {
            strTemp=QString("r%1 c%2,").arg(selectedListMain[i].row(),0,10).arg(selectedListMain[i].column(),0,10);
            str=str+strTemp;
            if(i==0)
            {
                miniX = selectedListMain[i].row();
                miniY = selectedListMain[i].column();
                maxX = selectedListMain[i].row();
                maxY = selectedListMain[i].column();
            }else
            {
                int posX = selectedListMain[i].row();
                int posY = selectedListMain[i].column();
                if((posX<=miniX) && (posY<=miniY))
                {
                    miniX = posX;
                    miniY = posY;
                }
                if((posX>= maxX) && (posY>= maxY))
                {
                    maxX=posX;
                    maxY=posY;
                }
            }



        }

    }//区域坐标检测

    int numberRow=needSplit;
    //拆分为needSplit行,而队列里需要增加needSplit-1个对象
    numberRow=numberRow-1;

    cellListDescribe singleObj;
    int rwoWidthFixPosition=0;

    //QMessageBox::about(NULL,"发现要拆分的单元格",QString("miniX %1 maxX%2,").arg(miniX,0,10).arg(maxX,0,10));
    if(sizeMain==1)//选择的是基本单元格时
    {
        QList<cellListDescribe> tempCellListObj;//描述整个表格信息的list对象
        for(int i=0;i<cellListObj.count();i++)//表格坐标系统处理，平移，增加，不动3种清况
        {
            if(cellListObj[i].endRow < miniX)
            {
                ;
            }else  if( cellListObj[i].startRow<=miniX  && cellListObj[i].endRow>= maxX  )
            {
                bool okXY1=false;
                bool okXY2=false;

                if(cellListObj[i].startRow==miniX && cellListObj[i].startColmun==miniY)
                {
                    okXY1=true;
                }else
                {
                    okXY1=false;
                }

                if(cellListObj[i].endRow==maxX  && cellListObj[i].endColmun== maxY)
                {
                    okXY2=true;
                }else
                {
                    okXY2=false;
                }

                if(okXY1 && okXY2)//是当前单元格，需增加一个描述
                {   //目前numberRow=1，不需要循环，如果该数大于1，则需要进行循环生成列表
                    rwoWidthFixPosition=cellListObj[i].row;
                    for(int addCounter=0;addCounter<numberRow;addCounter++)
                    {
                        singleObj.row=cellListObj[i].row+addCounter+1;
                        singleObj.colmun=cellListObj[i].colmun;
                        singleObj.startRow=cellListObj[i].row+addCounter+1;
                        singleObj.startColmun=cellListObj[i].colmun;
                        singleObj.endRow=cellListObj[i].row+addCounter+1;
                        singleObj.endColmun=cellListObj[i].colmun;
                        singleObj.baseCell=1;
                        tempCellListObj.append(singleObj);
                    }
                   // QMessageBox::about(NULL,"发现要拆分的单元格",QString(" 已经找到该单元格，增加对象%1个").arg(tempCellListObj.count(),0,10));

                }else//其它所有单元格的row均要增加
                {
                     cellListObj[i].endRow=cellListObj[i].endRow+numberRow;//这里不需要循环，一次即可调整到位
                     cellListObj[i].baseCell=0;
                }

            }else  if(cellListObj[i].startRow > maxX)
            {
               cellListObj[i].row=cellListObj[i].row+numberRow;
               cellListObj[i].startRow=cellListObj[i].startRow+numberRow;
               cellListObj[i].endRow=cellListObj[i].endRow+numberRow;
            }
        }


        for(int i=0;i<tempCellListObj.count();i++)
        {
            cellListObj.append(tempCellListObj[i]);
        }

        //list内容保存至新list
        newCellListObj.clear();
        for(int k=0;k<cellListObj.count();k++)
        {
            singleObj.row=cellListObj[k].row;
            singleObj.colmun=cellListObj[k].colmun;
            singleObj.startRow=cellListObj[k].startRow;
            singleObj.startColmun=cellListObj[k].startColmun;
            singleObj.endRow=cellListObj[k].endRow;
            singleObj.endColmun=cellListObj[k].endColmun;
            singleObj.baseCell=cellListObj[k].baseCell;
            newCellListObj.append(singleObj);
        }

        rowHeight.clear();
        columnWidth.clear();

        for(int m=0;m<tableRow;m++)//取出行高对列
        {
            rowHeight.append(ui->tableView_Edit->rowHeight(m));
        }

        for(int n=0;n<tableColumn;n++)//取出列宽队列
        {
            columnWidth.append(ui->tableView_Edit->columnWidth(n));
        }

        int splitHeight=rowHeight[rwoWidthFixPosition]/needSplit;//计算分割后的行高
        rowHeight[rwoWidthFixPosition]=splitHeight;//设置分割行的行高
        for(int m=1;m<=numberRow;m++)//增加需要增加的行高至行高对列
        {
            rowHeight.insert(rwoWidthFixPosition,splitHeight);
        }
        //QMessageBox::about(NULL,"rowHeight",QString("row %1,column %2").arg(rowHeight.count(),0,10).arg(columnWidth.count(),0,10));
        tableRow=tableRow+numberRow;//更新表格的row和column，准备表格重置
        tableColumn=tableColumn;

    }//选择基本单元格处理结束


}

void editDialog::useNewCellListSetSpan()
{

    int size =newCellListObj.count();
    //QMessageBox::about(NULL,"newCellListObj",QString(" 新表的大小newCellListObj.count=%1 ").arg(size,0,10));
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
             ui->tableView_Edit->setSpan(miniX,miniY,maxX-miniX+1,maxY-miniY+1);
             removeCell(miniX,miniY,maxX,maxY);
         }

    }

}

quint16 editDialog::getTableSelectNumber()
{
    QItemSelectionModel *selectModelMain=ui->tableView_Edit->selectionModel();
    QModelIndexList selectedListMain=selectModelMain->selectedIndexes();

    int sizeMain=selectedListMain.size();
    return sizeMain;
}

void editDialog::columnSplit(qint16 needSplit)
{
    QItemSelectionModel *selectModelMain=ui->tableView_Edit->selectionModel();
        QModelIndexList selectedListMain=selectModelMain->selectedIndexes();

        int sizeMain=selectedListMain.size();
        int miniX=0;
        int miniY=0;
        int maxX=0;
        int maxY=0;
        QString str;
        QString strTemp;

        if(sizeMain >0)//获取应该增加的行的列表
        {

            for(int i=0;i<sizeMain;i++)
            {
                strTemp=QString("r%1 c%2,").arg(selectedListMain[i].row(),0,10).arg(selectedListMain[i].column(),0,10);
                str=str+strTemp;
                if(i==0)
                {
                    miniX = selectedListMain[i].row();
                    miniY = selectedListMain[i].column();
                    maxX = selectedListMain[i].row();
                    maxY = selectedListMain[i].column();
                }else
                {
                    int posX = selectedListMain[i].row();
                    int posY = selectedListMain[i].column();
                    if((posX<=miniX) && (posY<=miniY))
                    {
                        miniX = posX;
                        miniY = posY;
                    }
                    if((posX>= maxX) && (posY>= maxY))
                    {
                        maxX=posX;
                        maxY=posY;
                    }
                }



            }

        }//区域坐标检测

        int numberColmun=needSplit;
        //拆分为needSplit行,而队列里需要增加needSplit-1个对象
        numberColmun=numberColmun-1;

        cellListDescribe singleObj;
        int columnWidthFixPosition=0;

        //QMessageBox::about(NULL,"发现要拆分的单元格",QString("miniY %1 maxY%2,").arg(miniY,0,10).arg(maxY,0,10));
        if(sizeMain==1)//选择的是基本单元格时
        {
            QList<cellListDescribe> tempCellListObj;//temp描述表格信息的list对象
            for(int i=0;i<cellListObj.count();i++)
            {
                if(cellListObj[i].endColmun < miniY)
                {
                    ;
                }else  if( cellListObj[i].startColmun<=miniY  && cellListObj[i].endColmun>= maxY  )
                {
                    bool okXY1=false;
                    bool okXY2=false;

                    if(cellListObj[i].startRow==miniX && cellListObj[i].startColmun==miniY)
                    {
                        okXY1=true;
                    }else
                    {
                        okXY1=false;
                    }

                    if(cellListObj[i].endRow==maxX  && cellListObj[i].endColmun== maxY)
                    {
                        okXY2=true;
                    }else
                    {
                        okXY2=false;
                    }

                    if(okXY1 && okXY2)//是当前单元格，需增加一个描述
                    {   //目前numberColmun=1，不需要循环，如果该数大于1，则需要进行循环生成列表
                        columnWidthFixPosition=cellListObj[i].colmun;
                        for(int addCounter=0;addCounter<numberColmun;addCounter++)
                        {
                            singleObj.row=cellListObj[i].row;
                            singleObj.colmun=cellListObj[i].colmun+addCounter+1;
                            singleObj.startRow=cellListObj[i].row;
                            singleObj.startColmun=cellListObj[i].colmun+addCounter+1;
                            singleObj.endRow=cellListObj[i].row;
                            singleObj.endColmun=cellListObj[i].colmun+addCounter+1;
                            singleObj.baseCell=1;
                            tempCellListObj.append(singleObj);
                        }
                        //QMessageBox::about(NULL,"发现列拆分的单元格",QString(" 已经找到该单元格，增加对象%1个").arg(tempCellListObj.count(),0,10));

                    }else//其它所有单元格的row均要增加
                    {
                         cellListObj[i].endColmun=cellListObj[i].endColmun+numberColmun;//这里不需要循环，一次即可调整到位
                         cellListObj[i].baseCell=0;
                    }

                }else  if(cellListObj[i].startColmun > maxY)
                {
                   cellListObj[i].colmun=cellListObj[i].colmun+numberColmun;
                   cellListObj[i].startColmun=cellListObj[i].startColmun+numberColmun;
                   cellListObj[i].endColmun=cellListObj[i].endColmun+numberColmun;
                }
            }

            //cellListObj.append(singleObj);//增加需要增加的对象
            for(int i=0;i<tempCellListObj.count();i++)
            {
                cellListObj.append(tempCellListObj[i]);
            }

            //list内容保存至新list
            newCellListObj.clear();
            for(int k=0;k<cellListObj.count();k++)
            {
                singleObj.row=cellListObj[k].row;
                singleObj.colmun=cellListObj[k].colmun;
                singleObj.startRow=cellListObj[k].startRow;
                singleObj.startColmun=cellListObj[k].startColmun;
                singleObj.endRow=cellListObj[k].endRow;
                singleObj.endColmun=cellListObj[k].endColmun;
                singleObj.baseCell=cellListObj[k].baseCell;
                newCellListObj.append(singleObj);
            }


            //
            rowHeight.clear();
            columnWidth.clear();

            for(int m=0;m<tableRow;m++)//取出行高对列
            {
                rowHeight.append(ui->tableView_Edit->rowHeight(m));
            }

            for(int n=0;n<tableColumn;n++)//取出列宽队列
            {
                columnWidth.append(ui->tableView_Edit->columnWidth(n));
            }

            int splitWidth=columnWidth[columnWidthFixPosition]/needSplit;//计算分割后的行高
            columnWidth[columnWidthFixPosition]=splitWidth;//设置分割行的行高
            for(int m=1;m<=numberColmun;m++)//增加需要增加的行高至行高对列
            {
                columnWidth.insert(columnWidthFixPosition,splitWidth);
            }
            //
            tableRow=tableRow;//更新表格的row和column，准备表格重置
            tableColumn=tableColumn+numberColmun;

        }//选择基本单元格处理结束

}

void editDialog::on_pushButton_ToInit_clicked()
{
    QItemSelectionModel *selectModelMain=ui->tableView_Edit->selectionModel();
        QModelIndexList selectedListMain=selectModelMain->selectedIndexes();

        int sizeMain=selectedListMain.size();
        int miniX=0;
        int miniY=0;
        int maxX=0;
        int maxY=0;
        QString str;
        QString strTemp;

        if(sizeMain >0)//获取应该增加的行的列表
        {

            for(int i=0;i<sizeMain;i++)
            {
                strTemp=QString("r%1 c%2,").arg(selectedListMain[i].row(),0,10).arg(selectedListMain[i].column(),0,10);
                str=str+strTemp;
                if(i==0)
                {
                    miniX = selectedListMain[i].row();
                    miniY = selectedListMain[i].column();
                    maxX = selectedListMain[i].row();
                    maxY = selectedListMain[i].column();
                }else
                {
                    int posX = selectedListMain[i].row();
                    int posY = selectedListMain[i].column();
                    if((posX<=miniX) && (posY<=miniY))
                    {
                        miniX = posX;
                        miniY = posY;
                    }
                    if((posX>= maxX) && (posY>= maxY))
                    {
                        maxX=posX;
                        maxY=posY;
                    }
                }



            }

        }//区域坐标检测
      //QMessageBox::about(NULL,"还原区域",QString("miniX %1, miniY %2,maxX %3,maxY %4").arg(miniX,0,10).arg(miniY,0,10).arg(maxX,0,10).arg(maxY,0,10));

      if(sizeMain > 1)//基本单元格不需要还原操作
      {
          initAreaCell(miniX,miniY,maxX,maxY);//表格队列还原
          initTable(tableRow,tableColumn);//表格重建
          updateTableSize();
          useNewCellListSetSpan();//表格span重现
      }
}

void editDialog::initAreaCell(int startRow, int startColumn, int endRow, int endColumn)
{
    int size = cellListObj.count();
    bool notBaseCell=true;
    if((startRow==endRow) && (startColumn==endColumn))
    {
        notBaseCell=false;
    }else
    {
        notBaseCell=true;
    }

    bool findThisArea=false;
    int  findPosition=0;
    bool ok1;
    bool ok2;
    bool ok3;
    bool ok4;
    if(size > 0  && notBaseCell)
    {
        for(int i=0;i<size;i++)
        {
            if(cellListObj[i].startRow == startRow)
            {
                ok1=true;
            }else
            {
                ok1=false;
            }

            if(cellListObj[i].startColmun == startColumn)
            {
                ok2=true;
            }else
            {
                ok2=false;
            }

            if(cellListObj[i].endRow == endRow)
            {
                ok3=true;
            }else
            {
                ok3=false;
            }

            if(cellListObj[i].endColmun == endColumn)
            {
                ok4=true;
            }else
            {
                ok4=false;
            }

            if(ok1 && ok2 && ok3 && ok4)
            {
                findThisArea=true;
                findPosition=i;
                break;
            }else
            {
                ;
            }

        }
    }

    QList<cellListDescribe> tempCellListObj;//描述整个表格信息的list对象
    cellListDescribe tempObj;  //追加合并后的对象

    if(findThisArea)
    {
        cellListObj.removeAt(findPosition);
      //int addNumber=(endRow-startRow+1)*(endColumn-startColumn+1);
        for(int i=startRow;i<=endRow;i++)
        {
            for(int j=startColumn;j<=endColumn;j++)
            {
                  tempObj.row=i;
                  tempObj.colmun=j;
                  tempObj.startRow=i;
                  tempObj.startColmun=j;
                  tempObj.endRow=i;
                  tempObj.endColmun=j;
                  tempObj.baseCell=1;
                  cellListObj.append(tempObj);
            }
        }

    }

    //list内容保存至新list
    newCellListObj.clear();
    cellListDescribe singleObj;
    for(int k=0;k<cellListObj.count();k++)
    {
        singleObj.row=cellListObj[k].row;
        singleObj.colmun=cellListObj[k].colmun;
        singleObj.startRow=cellListObj[k].startRow;
        singleObj.startColmun=cellListObj[k].startColmun;
        singleObj.endRow=cellListObj[k].endRow;
        singleObj.endColmun=cellListObj[k].endColmun;
        singleObj.baseCell=cellListObj[k].baseCell;
        newCellListObj.append(singleObj);
    }


    rowHeight.clear();
    columnWidth.clear();

    for(int m=0;m<tableRow;m++)//取出行高对列
    {
        rowHeight.append(ui->tableView_Edit->rowHeight(m));
    }

    for(int n=0;n<tableColumn;n++)//取出列宽队列
    {
        columnWidth.append(ui->tableView_Edit->columnWidth(n));
    }

//    tableRow=tableRow;//更新表格的row和column，准备表格重置
//    tableColumn=tableColumn;

}

void editDialog::updateTableSize()
{

       //QMessageBox::about(NULL,"表格更新box",QString("run here tableRow%1 tableColumn%2").arg(tableRow,0,10).arg(tableColumn,0,10));


        for(int i=0;i<rowHeight.count();i++)
        {
          //  QMessageBox::about(NULL,"还原区域",QString("%1").arg(rowHeight[i],0,10));
            ui->tableView_Edit->setRowHeight(i,rowHeight[i]);
        }

        for(int j=0;j<columnWidth.count();j++)
        {
            ui->tableView_Edit->setColumnWidth(j,columnWidth[j]);
        }
}

void editDialog::areaCombination(int startRow, int startColumn, int endRow, int endColumn)
{
    ui->tableView_Edit->setSpan(startRow,startColumn,endRow-startRow+1,endColumn-startColumn+1);
    removeCell(startRow,startColumn,endRow,endColumn);
   //ui->tableView_Edit->setSpan(0,0,2,2);

    newCellListObj.clear();
    cellListDescribe singleObj;
    for(int k=0;k<cellListObj.count();k++)
    {
        singleObj.row=cellListObj[k].row;
        singleObj.colmun=cellListObj[k].colmun;
        singleObj.startRow=cellListObj[k].startRow;
        singleObj.startColmun=cellListObj[k].startColmun;
        singleObj.endRow=cellListObj[k].endRow;
        singleObj.endColmun=cellListObj[k].endColmun;
        singleObj.baseCell=cellListObj[k].baseCell;
        newCellListObj.append(singleObj);
    }

    rowHeight.clear();
    columnWidth.clear();

    for(int m=0;m<tableRow;m++)//取出行高对列
    {
        rowHeight.append(ui->tableView_Edit->rowHeight(m));
    }

    for(int n=0;n<tableColumn;n++)//取出列宽队列
    {
        columnWidth.append(ui->tableView_Edit->columnWidth(n));
    }
}

void editDialog::on_pushButton_AddColumn_clicked()
{
       //cellListObj列表增加
    rowHeight.clear();
    columnWidth.clear();

    for(int m=0;m<tableRow;m++)//取出行高对列
    {
        rowHeight.append(ui->tableView_Edit->rowHeight(m));
    }

    for(int n=0;n<tableColumn;n++)//取出列宽队列
    {
        columnWidth.append(ui->tableView_Edit->columnWidth(n));
    }

    if(tableRow > 0)
    {
        for(int i=0;i<=tableRow;i++)
        {
            cellListDescribe singleObj;
            singleObj.row=i;
            singleObj.colmun=tableColumn+1;
            singleObj.startRow=i;
            singleObj.startColmun=tableColumn+1;
            singleObj.endRow=i;
            singleObj.endColmun=tableColumn+1;
            singleObj.baseCell=1;
            cellListObj.append(singleObj);

        }
        tableColumn=tableColumn+1;//增加列宽
        columnWidth.append(initColumnWidth);//增加列宽描述
        initTable(tableRow,tableColumn);
        updateTableSize();
        useNewCellListSetSpan();//表格span重现
    }
}

void editDialog::on_pushButton_clicked()
{
        //cellListObj列表增加
    rowHeight.clear();
    columnWidth.clear();

    for(int m=0;m<tableRow;m++)//取出行高对列
    {
        rowHeight.append(ui->tableView_Edit->rowHeight(m));
    }

    for(int n=0;n<tableColumn;n++)//取出列宽队列
    {
        columnWidth.append(ui->tableView_Edit->columnWidth(n));
    }

     if(tableColumn> 0)
     {
         for(int i=0;i<tableColumn;i++)
         {
             cellListDescribe singleObj;
             singleObj.row=tableRow+1;
             singleObj.colmun=i;
             singleObj.startRow=tableRow+1;
             singleObj.startColmun=i;
             singleObj.endRow=tableRow+1;
             singleObj.endColmun=i;
             singleObj.baseCell=1;
             cellListObj.append(singleObj);

         }
         tableRow=tableRow+1;//增加列宽
         rowHeight.append(initRowHeight);//增加列宽描述
         initTable(tableRow,tableColumn);
         updateTableSize();
         useNewCellListSetSpan();//表格span重现
     }
}

void editDialog::on_pushButton_EditData_clicked()
{

    quint16 selectNumber=getTableSelectNumber();
    if(selectNumber > 0) //选择不为空时
    {
            QItemSelectionModel *selectModelMain=ui->tableView_Edit->selectionModel();
            QModelIndexList selectedListMain=selectModelMain->selectedIndexes();

            int sizeMain=selectedListMain.size();
            int miniX=0;
            int miniY=0;
            int maxX=0;
            int maxY=0;
            QString str;
            QString strTemp;

            if(sizeMain >0)//区域坐标检测;
            {

                for(int i=0;i<sizeMain;i++)
                {
                    strTemp=QString("r%1 c%2,").arg(selectedListMain[i].row(),0,10).arg(selectedListMain[i].column(),0,10);
                    str=str+strTemp;
                    if(i==0)
                    {
                        miniX = selectedListMain[i].row();
                        miniY = selectedListMain[i].column();
                        maxX = selectedListMain[i].row();
                        maxY = selectedListMain[i].column();
                    }else
                    {
                        int posX = selectedListMain[i].row();
                        int posY = selectedListMain[i].column();
                        if((posX<=miniX) && (posY<=miniY))
                        {
                            miniX = posX;
                            miniY = posY;
                        }
                        if((posX>= maxX) && (posY>= maxY))
                        {
                            maxX=posX;
                            maxY=posY;
                        }
                    }



                }

            }//区域坐标检测;
            int startRow=miniX;
            int startColumn=miniY;
            int endRow=maxX;
            int endColumn=maxY;

            int size = cellListObj.count();

            bool findThisArea=false;
            int  findPosition=0;
            bool ok1;
            bool ok2;
            bool ok3;
            bool ok4;
            if(size > 0)
            {
                for(int i=0;i<size;i++)
                {
                    if(cellListObj[i].startRow == startRow)
                    {
                        ok1=true;
                    }else
                    {
                        ok1=false;
                    }

                    if(cellListObj[i].startColmun == startColumn)
                    {
                        ok2=true;
                    }else
                    {
                        ok2=false;
                    }

                    if(cellListObj[i].endRow == endRow)
                    {
                        ok3=true;
                    }else
                    {
                        ok3=false;
                    }

                    if(cellListObj[i].endColmun == endColumn)
                    {
                        ok4=true;
                    }else
                    {
                        ok4=false;
                    }

                    if(ok1 && ok2 && ok3 && ok4)
                    {
                        findThisArea=true;
                        findPosition=i;
                        break;
                    }else
                    {
                        ;
                    }

                }
            }

            if(findThisArea)
            {
                editPosition=findPosition;
                EditDataDialog newDialog(this);
                newDialog.exec();
            }else
            {
                QMessageBox::about(NULL,"单元格数据编辑 警告","您选择的区域不在有效队列中");

            }
    }else
    {
        ;
    }


}

void editDialog::on_pushButton_SaveData_clicked()
{
     saveData();
     freeTableObject->saveHeightInfo();
     freeTableObject->saveWidthInfo();
     freeTableObject->saveCellListInfo();
     freeTableObject->saveNewCellListInfo();
     freeTableObject->needTranslateData=true;
}

void editDialog::saveData()
{
     freeTableObject->initColumnWidth=initColumnWidth;
     freeTableObject->initRowHeight=initRowHeight;
     freeTableObject->initTable(tableRow,tableColumn);
     freeTableObject->tableRow=tableRow;
     freeTableObject->tableColumn=tableColumn;

     int size=cellListObj.count();
//     freeTableObject->cellListObj.clear();
//     for(int i=0;i<size;i++)
//     {
//         freeTableObject->cellListObj.append(cellListObj[i]);
//         QString str=freeTableObject->cellListObj[i].data.fontColorRed;
//         str=str+" "+cellListObj[i].data.fontColorRed;
//         QMessageBox::about(NULL,"saveData 1,2",str);


//     }

     rowHeight.clear();
     columnWidth.clear();

     freeTableObject->rowHeight.clear();
     freeTableObject->columnWidth.clear();

     for(int m=0;m<tableRow;m++)//取出行高对列
     {
         rowHeight.append(ui->tableView_Edit->rowHeight(m));
         freeTableObject->rowHeight.append(ui->tableView_Edit->rowHeight(m));
     }

     for(int n=0;n<tableColumn;n++)//取出列宽队列
     {
         columnWidth.append(ui->tableView_Edit->columnWidth(n));
         freeTableObject->columnWidth.append(ui->tableView_Edit->columnWidth(n));
     }

     freeTableObject->updateTableSize();

     freeTableObject->newCellListObj.clear();
     for(int i=0;i<newCellListObj.count();i++)
     {
         freeTableObject->newCellListObj.append(newCellListObj[i]);
     }

     freeTableObject->useNewCellListSetSpan();


     freeTableObject->cellListObj.clear();
     for(int i=0;i<size;i++)
     {
         freeTableObject->cellListObj.append(cellListObj[i]);
     }


     freeTableObject->setCellString();

}


void editDialog::loadData()
{
    tableRow=freeTableObject->tableRow;
    tableColumn=freeTableObject->tableColumn;
    initTable(tableRow,tableColumn);

    //QMessageBox::about(NULL,"表格更新box 3",QString("run here tableRow%1 tableColumn%2").arg(tableRow,0,10).arg(tableColumn,0,10));

    rowHeight.clear();
    columnWidth.clear();

    int size=freeTableObject->rowHeight.count();
    for(int i=0;i<size;i++)
    {
        rowHeight.append(freeTableObject->rowHeight[i]);
    }

    size = freeTableObject->columnWidth.count();
    for(int i=0;i<size;i++)
    {
        columnWidth.append(freeTableObject->columnWidth[i]);
    }

    updateTableSize();

    newCellListObj.clear();

    size = freeTableObject->newCellListObj.count() ;
    for(int i=0;i<size;i++)
    {
        newCellListObj.append(freeTableObject->newCellListObj[i]);
    }

    useNewCellListSetSpan();

    cellListObj.clear();
    size=freeTableObject->cellListObj.count();
    for(int i=0;i<size;i++)
    {
        cellListObj.append(freeTableObject->cellListObj[i]);
    }

    setCellString();

}


void editDialog::setCellString()
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
            this->modelEdit->setItem(i,j,tempItem); //首先创建Item
        }else if(dataType==1)
        {
            tempItem=new QStandardItem(regStr);
            QPixmap pix(bgPhoto);
            tempItem->setForeground(QBrush(fontColor));
            pix=pix.scaled(sizeItem,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            tempItem->setBackground(pix);
            this->modelEdit->setItem(i,j,tempItem); //首先创建Item
        }

        if(bgPhoto.isEmpty())
        {
              this->modelEdit->item(i,j)->setForeground(QBrush(fontColor));//才能设置字体颜色
              this->modelEdit->item(i,j)->setBackground(QBrush(BGColor));//最后设置背景色
        }

        QString strFamily=cellListObj[m].data.fontFamily;
        int     fontSize=cellListObj[m].data.fontSize.toInt();
        font.setFamily(strFamily);
        font.setPointSize(fontSize);
        font.setBold(cellListObj[m].data.fontBold.toInt());
        font.setItalic(cellListObj[m].data.fontItalic.toInt());
        this->modelEdit->item(i,j)->setFont(font);


    }
}

void editDialog::getWidthAndHeight()
{
    rowHeight.clear();
    columnWidth.clear();



    for(int m=0;m<tableRow;m++)//取出行高对列
    {
        rowHeight.append(ui->tableView_Edit->rowHeight(m));

    }

    for(int n=0;n<tableColumn;n++)//取出列宽队列
    {
        columnWidth.append(ui->tableView_Edit->columnWidth(n));

    }
}

#ifndef TABLEFREE_H
#define TABLEFREE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QLayout>
#include <QSplitter>
#include <QColumnView>
#include <QList>
#include <QLayoutItem>
#include <QMessageBox>

#include <QRect>
#include <QStandardItemModel>
#include <QFont>
#include <QTimer>
#include <QDebug>

#include <QTableView>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QBrush>
#include <QColor>
#include <QTableWidget>
#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QPalette>
#include <QMessageBox>
#include <QPixmap>
#include <QtWidgets>

class cellDataDescribe  //每个单元格 属性 描述类
{
   public:
    QString dataType;// 0.自定义字符串  1.其它数据
                     //
    QString backGroundPhoto; //背景图片
    QString backGroundColorRed;//背景色
    QString backGroundColorBlue;//背景色
    QString backGroundColorGreen;//背景色
    QString  fontSize;//字体大小
    QString  fontFamily;//字体族
    QString  fontBold;//是否黑体
    QString  fontItalic;//是否斜体

    QString  fontColorRed;//字颜色
    QString  fontColorBlue;//字颜色
    QString  fontColorGreen;//字颜色
    QString  registerXPath;//注册的数据字符串，其它时使用
    QString  userString;//用户自定义的字符串
    QString  editFlag;//该单元是否被编辑过，


};

class cellListDescribe  //单元格队列cellListObj中，每个单元格x，y，row和colmun
{   //共22个成员
public:
    quint32  row;
    quint32  colmun;
    quint32 startRow ;//左上x，y，
    quint32 startColmun;
    quint32 endRow;//右下 x，y
    quint32 endColmun;
    quint32 baseCell;  //1,基本表格单元，0，合并过的表格单元

    cellDataDescribe data;
    cellListDescribe()
    {
        data.backGroundColorBlue="255";
        data.backGroundColorGreen="255";
        data.backGroundColorRed="255";
        data.fontColorGreen="0";
        data.fontColorRed="0";
        data.fontColorBlue="0";
        data.dataType="0";
        data.userString="no user str";
        data.registerXPath="no reg str";
//        data.fontFamily="Sans";
//        data.fontSize="11";
//        data.fontBold="0";
//        data.fontItalic="0";
        data.editFlag="0";
    }
};

class TableFree : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString freeTable_cellList READ getCellList WRITE setCellList)
    Q_PROPERTY(QString freeTable_newCellList READ getNewCellList WRITE setNewCellList)
    Q_PROPERTY(QString freeTable_rowHeight READ getRowHeightList WRITE setRowHeightList)
    Q_PROPERTY(QString freeTable_columnWidth READ getColumnWidthList WRITE setColumnWidthList)
public:
    QString getCellList() const { return cellListStr; }//组件属性
    void setCellList(QString needSaveStr);
    QString cellListStr; //组件属性

    QString getNewCellList() const { return newCellListStr; }//组件属性
    void setNewCellList(QString needSaveStr);
    QString newCellListStr; //组件属性

    QString getRowHeightList() const { return rowHeightListStr; }//组件属性
    void setRowHeightList(QString needSaveStr);
    QString rowHeightListStr; //组件属性

    QString getColumnWidthList() const { return columnWidthListStr; }//组件属性
    void setColumnWidthList(QString needSaveStr);
    QString columnWidthListStr; //组件属性

public:
    TableFree(QWidget *parent = 0);
    QStandardItemModel *modelLeft;//控制表显示的model
    QTableView *tableViewLeft;//表格对象
    QGridLayout *layout;

    quint32 tableRow,tableColumn;//表格的行数，列数
    QList<cellListDescribe> cellListObj;//描述整个表格信息的list对象
    QList<cellListDescribe> newCellListObj;//描述new表格信息的list对象,row  split

    QList<quint16> rowHeight;//行高
    QList<quint16> columnWidth;//列宽

    int  initRowHeight;//初始化时的行高
    int  initColumnWidth;//初始化时的列宽
    bool firstOpenFlag;//首次打开标识
    bool needTranslateData;//需要将ui中回复的数据传送至editDialog，在存储数据恢复时使用，true需要恢复，false不需要
                           //在reviewUITableFirstOpen函数中被修改


    void initTable(quint16 row,quint16 column); //初始化表格
    void updateTableSize();//更新表格大小，行高，列宽
    void removeCell(int startRow, int startColumn, int endRow, int endColumn);//根据合并坐标操作cellListObj

    void useNewCellListSetSpan();//使用新的描述对象列表对表格进行setspan
    void setCellString();//更新表格单元的显示字符

    void saveHeightInfo();//存储行高
    void saveWidthInfo();//存储列宽
    void saveCellListInfo();//保存cellListObj的信息
    void saveNewCellListInfo();//保存newCellListObj的信息

    void reviewUITableFirstOpen();//第一次打开存储过的ui时，将属性数据还原为表格数据
    void reviewCellListObj();//还原cellListObj
    void reviewNewCellListObj();//还原NewCellObj
};

#endif

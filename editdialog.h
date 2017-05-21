#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>

#include "tablefree.h"
#include "inittabledialog.h"
#include "dialog.h"
#include  "areasplitdialog.h"
#include "editdatadialog.h"

namespace Ui {
class editDialog;
}

class editDialog : public QDialog
{
    Q_OBJECT

public:
    //explicit editDialog(QWidget *parent = 0);
    explicit editDialog(TableFree *table=0,QWidget *parent = 0);
    ~editDialog();

    QStandardItemModel *modelEdit;//控制表显示的model

    quint32 tableRow,tableColumn;//初始化时表格的行数，列数
    QList<cellListDescribe> cellListObj;//描述整个表格信息的list对象
    QList<cellListDescribe> newCellListObj;//描述new表格信息的list对象,row  split
    QList<quint16> rowHeight;//行高
    QList<quint16> columnWidth;//列宽
    QList<quint16> newRowHeight;//行高
    QList<quint16> newColumnWidth;//列宽
    int areaMiniX;//选择区域的最小x
    int areaMiniY;//选择区域的最小y
    int areaMaxX;//选择区域的最大x
    int areaMaxY;//选择区域的最大y
    quint16 selectBaseCellNumber;//选择区域包含的基本单元格数量
    int initRowHeight;//初始化时的行高
    int initColumnWidth;//初始化时的列宽
    int editPosition;//要编辑的单元格位置
public:
    void initTable(quint16 row,quint16 column); //初始化表格
    void combinationTable();//表格合并
    void rowSplit(qint16 needSplit);//行拆分
    void columnSplit(qint16 needSplit);//列拆分
    void removeCell(int startRow, int startColumn, int endRow, int endColumn);//根据合并坐标操作cellListObj
    void initAreaCell(int startRow, int startColumn, int endRow, int endColumn);//根据还原坐标操作cellListObj
    void useNewCellListSetSpan();//使用新的描述对象列表对表格进行setspan
    void updateTableSize();//更新表格大小，行高，列宽
    quint16 getTableSelectNumber();//获取区域大小
    void areaCombination(int startRow, int startColumn, int endRow, int endColumn);//单区域合并函数
    void saveData();//保存数据
    void loadData();//重现数据
    void setCellString();//设置表格属性
    void getWidthAndHeight();//获取行高，列宽
private slots:
    void on_pushButton_Init_clicked();

    void on_pushButton_SetSpan_clicked();

    void on_pushButton_split_clicked();

    void on_pushButton_ToInit_clicked();

    void on_pushButton_AddColumn_clicked();

    void on_pushButton_clicked();

    void on_pushButton_EditData_clicked();

    void on_pushButton_SaveData_clicked();

private:
    Ui::editDialog *ui;
    TableFree *freeTableObject;
};

#endif // EDITDIALOG_H

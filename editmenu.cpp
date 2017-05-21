#include "editmenu.h"

 editMenu::editMenu(TableFree *TableFreeTranslate,QObject *parent):
     QObject(parent)
{
     tableFreeFirst=TableFreeTranslate;
     editAction = new QAction(tr("编辑表格"),this);
     setAction = new QAction(tr("设置表格"),this);
     connect(editAction, &QAction::triggered, this, &editMenu::editTable);
     connect(setAction, &QAction::triggered, this, &editMenu::setTable);
}
 void editMenu::editTable()
 {
      editDialog newDialog(tableFreeFirst);
      newDialog.exec();
 }

 void editMenu::setTable()
 {
     // setDialog  newDialog(freeTableFirst);
     // newDialog.exec();
 }

 QList<QAction *> editMenu::taskActions() const
 {
     QList<QAction *> list;
     list.append(editAction);
     list.append(setAction);
     return list;
 }


 EditMenuFactory::EditMenuFactory(QExtensionManager *parent)
     : QExtensionFactory(parent)
 {

 }

 QObject *EditMenuFactory::createExtension(QObject *object,
                                                    const QString &iid,
                                                    QObject *parent) const
 {
     if (iid != Q_TYPEID(QDesignerTaskMenuExtension))
         return 0;

     if (TableFree *tableFreeTranslate = qobject_cast<TableFree*>(object))
         return new editMenu(tableFreeTranslate, parent);

     return 0;
 }

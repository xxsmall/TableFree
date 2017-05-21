#ifndef EDITMENU_H
#define EDITMENU_H

#include <QObject>
#include <QDesignerTaskMenuExtension>
#include <QExtensionFactory>
#include <QAction>

#include <QtDesigner>
#include <QtWidgets>

#include "tablefree.h"
#include "editdialog.h"

class editMenu : public QObject, public QDesignerTaskMenuExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerTaskMenuExtension)
public:
   // explicit editMenu(QObject *parent = 0);
    editMenu(TableFree *TableFreeTranslate,QObject *parent);
    QList<QAction *> taskActions() const;

    TableFree  *tableFreeFirst;
    QAction *editAction;
    QAction *setAction;
signals:

public slots:

private slots :
    void editTable();
    void setTable();
};


class EditMenuFactory : public QExtensionFactory
{
    Q_OBJECT

public:
    EditMenuFactory(QExtensionManager *parent = 0);

protected:
    QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const;
};


#endif // EDITMENU_H

#include "tablefree.h"
#include "tablefreeplugin.h"

#include <QtPlugin>
#include <QtDesigner>
#include <QtGui>

#include "editmenu.h"

TableFreePlugin::TableFreePlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void TableFreePlugin::initialize(QDesignerFormEditorInterface *core )
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here
    QExtensionManager *manager = core->extensionManager();
    Q_ASSERT(manager != 0);

    manager->registerExtensions(new EditMenuFactory(manager),
                                Q_TYPEID(QDesignerTaskMenuExtension));
    m_initialized = true;
}

bool TableFreePlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *TableFreePlugin::createWidget(QWidget *parent)
{
    return new TableFree(parent);
}

QString TableFreePlugin::name() const
{
    return QLatin1String("TableFree");
}

QString TableFreePlugin::group() const
{
    return QLatin1String("");
}

QIcon TableFreePlugin::icon() const
{
    return QIcon();
}

QString TableFreePlugin::toolTip() const
{
    return QLatin1String("");
}

QString TableFreePlugin::whatsThis() const
{
    return QLatin1String("");
}

bool TableFreePlugin::isContainer() const
{
    return false;
}

QString TableFreePlugin::domXml() const
{
   // return QLatin1String("<widget class=\"TableFree\" name=\"tableFree\">\n</widget>\n");
    return QLatin1String("<widget class=\"TableFree\" name=\"tableFree\"> \
        <property name=\"freeTable_cellList\" > \
         <string></string> \
        </property> \
        <property name=\"freeTable_newCellList\" > \
         <string></string> \
        </property> \
        <property name=\"freeTable_rowHeight\" > \
         <string></string> \
        </property> \
        <property name=\"freeTable_columnWidth\" > \
         <string></string> \
        </property> </widget>\n");
}

QString TableFreePlugin::includeFile() const
{
    return QLatin1String("tablefree.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(tablefreeplugin, TableFreePlugin)
#endif // QT_VERSION < 0x050000

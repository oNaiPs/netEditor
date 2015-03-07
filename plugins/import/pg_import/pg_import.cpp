#include "pg_import.h"
#include "pgconnection_gui.h"

#include <QtGui>


QAction *pg_import::importAction()
{
   QAction* pgimport=new QAction("Import from Postgres...",this);
    pgimport->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_K));

    connect (pgimport,SIGNAL(triggered()),SLOT(import()));

    return pgimport;
}

void pg_import::import()
{
    pgconnection_gui *c=new pgconnection_gui(this);
    c->exec();
}

QAction *pg_import::importSettings()
{
    return NULL;
}


QString pg_import::importPluginDescription()
{
    return "Example Import Plugin";

}


Q_EXPORT_PLUGIN2(import_iface,pg_import);

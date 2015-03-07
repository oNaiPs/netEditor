#include "example_import.h"
#include <QtGui>


QAction *example_import::importAction()
{
    QAction *a=new QAction("Import Example...",this);
    //settings_qaction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    connect (a,SIGNAL(triggered()),SLOT(import()));

    return a;
}

void example_import::import()
{
    QString fileName = QFileDialog::getOpenFileName(0, tr("Open File"),QDir::currentPath(), tr("All Files (*.*)"));
    QMessageBox::information(0,"Example IMPORT",QString("You have selected the file") +fileName);

}

QAction *example_import::importSettings()
{
    return NULL;
}


QString example_import::importPluginDescription()
{
    return "Example Import Plugin";

}


Q_EXPORT_PLUGIN2(import_iface,example_import);

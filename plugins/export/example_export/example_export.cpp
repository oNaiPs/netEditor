#include "example_export.h"
#include <QtGui>


QAction *example_export::exportAction()
{
    QAction *b=new QAction("Export Example...",this);
    //settings_qaction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    connect (b,SIGNAL(triggered()),SLOT(export1()));

    return b;
}

QAction *example_export::exportSettings()
{
QAction *b=new QAction("Export Example Settings",this);

    connect (b,SIGNAL(triggered()),SLOT(settings()));

    return b;

}

QString example_export::exportPluginDescription()
{
    return "Example Export Plugin";
}

void example_export::export1()
{

    QString fileName = QFileDialog::getOpenFileName(0, tr("Open File"),QDir::currentPath(), tr("All Files (*.*)"));
    QMessageBox::information(0,"Example EXPORT",QString("You have selected the file") +fileName);
}


void example_export::settings()
{
QWidget a;
a.show();

}

Q_EXPORT_PLUGIN2(export_iface,example_export);
